/***********************************************************
* Online map generation using CMetricMapBuilder class.
* Based on icp-slam source code.
* Rawlog file generation remains unchanged.
* It doesn't save final map.
* Results similar to offline version.
* It could be used for on-line path planning while explores area.
* Timmings appear to be acceptable and not interfere each other processes (rawlog file generation, display update,
* 	map generation, etc.
***********************************************************/

#include <mrpt/base.h>
#include <mrpt/slam.h>
#include <mrpt/opengl.h>
#include <mrpt/opengl/CPlanarLaserScan.h>  // This class lives in the lib [mrpt-maps] and must be included by hand
#include <mrpt/hwdrivers/CSickLaserSerial.h>
#include <mrpt/hwdrivers/CActivMediaRobotBase.h>
#include <mrpt/maps.h>
#include <mrpt/utils.h>
#include <mrpt/obs.h>
#include <mrpt/slam/CActionRobotMovement2D.h>
#include <mrpt/gui.h>


using namespace mrpt;
using namespace mrpt::utils;
using namespace mrpt::slam;
using namespace mrpt::opengl;
using namespace mrpt::hwdrivers;
using namespace mrpt::poses;
using namespace std;
using namespace mrpt::gui;
using namespace mrpt::system;
using namespace mrpt::math;


// ------------------------------------------------------
//	TestRawLogandLaser.
// ------------------------------------------------------
void TestRawLogandLaser() {

	CSickLaserSerial laser;
	CFileGZOutputStream f("my_dataset.rawlog");	
	CObservation2DRangeScanPtr myObs = CObservation2DRangeScan::Create();	// This will work. Defining pointer other way
																			// won't work: Create() must be used!
	CActivMediaRobotBase robot;

	static CObservationOdometry odom;
	odom.sensorLabel = "ODOMETRY";
	
	CMetricMapBuilderICP mapBuilder;
	
	CDisplayWindow3DPtr win3D;
	
	CObservationPtr observation;
	
	const CMultiMetricMap *mostLikMap;
	
	CTicTac StopWatch;
	
	CPose3D robotPose;
	


	string INI_FILENAME("GENRawICP.ini");
	string str;
	CConfigFile inifile(INI_FILENAME);

	

	CPose2D odo;
	bool thereIsObservation, hardError, flag;
	double TTimer, TTimer2, TTimer3, v, w;
	int64_t left_ticks, right_ticks;
	int counter = 0;
	int CountObs;
	unsigned long memUsage;
	
	
	
	// ICP map building initialization.
	// ------------------------------------------
	//			Load config from file:
	// ------------------------------------------
	const string RAWLOG_FILE = inifile.read_string("MappingApplication","rawlog_file","",  /*Force existence:*/ true);
	const unsigned int rawlog_offset = inifile.read_int("MappingApplication","rawlog_offset",0,  /*Force existence:*/ true);
	const string OUT_DIR_STD = inifile.read_string("MappingApplication","logOutput_dir","log_out",  /*Force existence:*/ true);
	const int LOG_FREQUENCY	= inifile.read_int("MappingApplication","LOG_FREQUENCY",5,  /*Force existence:*/ true);
	const bool SAVE_POSE_LOG = inifile.read_bool("MappingApplication","SAVE_POSE_LOG", false,  /*Force existence:*/ true);
	const bool SAVE_3D_SCENE = inifile.read_bool("MappingApplication","SAVE_3D_SCENE", false,  /*Force existence:*/ true);
	const bool CAMERA_3DSCENE_FOLLOWS_ROBOT = inifile.read_bool("MappingApplication","CAMERA_3DSCENE_FOLLOWS_ROBOT", true,  /*Force existence:*/ true);

	
	bool SHOW_PROGRESS_3D_REAL_TIME = false;
	int SHOW_PROGRESS_3D_REAL_TIME_DELAY_MS = 0;
	bool SHOW_LASER_SCANS_3D = true;

	MRPT_LOAD_CONFIG_VAR(SHOW_PROGRESS_3D_REAL_TIME, bool,  inifile, "MappingApplication");
	MRPT_LOAD_CONFIG_VAR(SHOW_LASER_SCANS_3D , bool,  inifile, "MappingApplication");
	MRPT_LOAD_CONFIG_VAR(SHOW_PROGRESS_3D_REAL_TIME_DELAY_MS, int, inifile, "MappingApplication");

	const char* OUT_DIR = OUT_DIR_STD.c_str();
	
	mapBuilder.ICP_options.loadFromConfigFile(inifile, "MappingApplication");
	mapBuilder.ICP_params.loadFromConfigFile (inifile, "ICP");
	
	mapBuilder.initialize();
	
	mapBuilder.options.verbose = true;
   	mapBuilder.options.alwaysInsertByClass.fromString(inifile.read_string("MappingApplication","alwaysInsertByClass",""));	
	
	// end ICP map building initialization.
	
	CSimpleMap finalMap;
	
	// Window to display ICP progress
	// Create 3D window if requested:
#if MRPT_HAS_WXWIDGETS
	if (SHOW_PROGRESS_3D_REAL_TIME)
	{
		win3D = CDisplayWindow3D::Create("VidICP", 600, 500);
		win3D->setCameraZoom(20);
		win3D->setCameraAzimuthDeg(-45);
	}
#endif
	// end of window creation
	


	// Robot initialization.
	robot.loadConfig(inifile, "ROBOT");
	robot.initialize();
	cout << "Robot initialized." << endl;
	
	// Laser initialization, warm up period.
	cout << "Waiting for laser to initialize..." << endl;
	laser.loadConfig(inifile, "LASER1");
	laser.initialize();					// This will raise an exception on error.
	cout << "Laser initialized." << endl;


	while (!mrpt::system::os::kbhit()) {
		
		StopWatch.Tic();

		try {
			laser.doProcessSimple(thereIsObservation, *myObs, hardError);
		}

		catch (std::exception &e) {
			cerr << e.what() << endl;
			hardError = true;
		}

		if (hardError)
			printf("[TEST] Hardware error=true!!\n");

		TTimer3 = StopWatch.Tac();

		if (thereIsObservation) {
			CountObs++;
			// if (!(CountObs % 2)) {		// Used to check timming and prevent data loss
			
				StopWatch.Tic();
				robot.getOdometryFull(odo, v, w, left_ticks, right_ticks);	// meters and degrees.
	
				odom.odometry = odo;
	
				odom.encoderLeftTicks = left_ticks;
				odom.encoderRightTicks = right_ticks;
				odom.timestamp = myObs -> timestamp;
	
				f << odom << *myObs;						// Guardar en archivo abierto f, en ese orden.
	
				counter++;
				
				TTimer2 = StopWatch.Tac();
	
				cout << "Saved " << counter << " times." << endl;
				// cout << "Laser doProcessSimple time: " << TTimer3 << " s" << endl;
				// cout << "rawlog Generation time: " << TTimer2 << " s" << endl;
				
				
				// Starting ICP building map processing
				StopWatch.Tic();
				observation.clear_unique();
				
				observation = CObservation2DRangeScanPtr(myObs);
				
				mapBuilder.processObservation(observation);	// A pointer (smart pointer as they call it) must be inserted
															// as an argument to processObservation()
				
				mostLikMap =  mapBuilder.getCurrentlyBuiltMetricMap();
				
				
				mapBuilder.getCurrentPoseEstimation() -> getMean(robotPose);
				COpenGLScenePtr scene = COpenGLScene::Create();
	
	            COpenGLViewportPtr view = scene -> getViewport("main");
	            ASSERT_(view);
	            
	            opengl::CSetOfObjectsPtr obj = opengl::CSetOfObjects::Create();
				mostLikMap -> getAs3DObject(obj);
				view -> insert(obj);
				
				CPose3DPDFPtr posePDF =  mapBuilder.getCurrentPoseEstimation();
				CPose3D  curRobotPose;
				posePDF -> getMean(curRobotPose);
				
				opengl::CSetOfObjectsPtr obj2 = opengl::stock_objects::RobotPioneer();
				obj2 -> setPose(curRobotPose);
				view -> insert(obj2);
				
				
				opengl::COpenGLScenePtr &ptrScene = win3D -> get3DSceneAndLock();
				ptrScene = scene;
	
				win3D -> unlockAccess3DScene();
				
				// Move camera:
				win3D -> setCameraPointingToPoint(curRobotPose.x(), curRobotPose.y(), curRobotPose.z());
	
				// Update:
				win3D -> forceRepaint();
				// End of ICP map building
				
				
				TTimer = StopWatch.Tac();
				cout << "Laser doProcessSimple time: " << TTimer3 * 1000 << " ms" << endl;
				cout << "rawlog Generation time: " << TTimer2 * 1000 << " ms" << endl;
				cout << "mapBuilder time: " << TTimer * 1000 << " ms" << endl;
				memUsage = getMemoryUsage();
				printf("Memory usage: (%.04fMb)\n\n", ((float)memUsage)/(1024*1024));
			// }			
		}

		mrpt::system::sleep(30);			// Loop period
	}

	mapBuilder.getCurrentlyBuiltMap(finalMap);
	str = format("%s/_finalmap_.simplemap",OUT_DIR);
    printf("Dumping final map in binary format to: %s\n", str.c_str() );
    mapBuilder.saveCurrentMapToFile(str);

	CMultiMetricMap  *finalPointsMap = mapBuilder.getCurrentlyBuiltMetricMap();
    str = format("%s/_finalmaps_.txt",OUT_DIR);
    printf("Dumping final metric maps to %s_XXX\n", str.c_str());
    finalPointsMap->saveMetricMapRepresentationToFile(str);
}



void LaserOff(void) {

	CSerialPort myCOM;
	
	string ComName;
	bool flag;


	string INI_FILENAME("GENRawICP.ini");

	CConfigFile			inifile(INI_FILENAME);
	
	ComName = inifile.read_string("LASER", "COM_port_LIN", "ttyUSB1", flag);


	myCOM.open(ComName);
	myCOM.close();

	mrpt::system::sleep(500);
}



void Dislodge(void) {

	CSerialPort myCom1;
	CSerialPort myCom2;

	string ComName1, ComName2;
	bool flag1, flag2;

	string INI_FILENAME("GENRawICP.ini");
	CConfigFile	inifile(INI_FILENAME);

	ComName1 = inifile.read_string("ROBOT", "robotPort_LIN", "ttyUSB0", flag1);
	ComName2 = inifile.read_string("LASER", "COM_port_LIN", "ttyUSB1", flag2);

	myCom1.open(ComName1);
	myCom2.open(ComName2);

	myCom2.purgeBuffers();
	myCom2.purgeBuffers();

	myCom1.close();
	myCom2.close();

	mrpt::system::sleep(500);
}



int main(int argc, char **argv)
{
	try {
		Dislodge();		
		TestRawLogandLaser();
		LaserOff();

		return 0;

	} catch (std::exception &e) {
		std::cout << "EXCEPCION: " << e.what() << std::endl;
		return -1;
	}

	catch (...) {
		printf("Another exception!!");
		return -1;
	}

}

