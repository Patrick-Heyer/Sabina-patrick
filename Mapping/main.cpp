/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   plugin1.h
/// \author Martin Reddy
/// \brief  An example plugin built against the plugin API.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#include <iostream>
#include <map>
#include <pthread.h>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>

#include "../Gui/gui.h"
#include "../Gui/video.h"
#include "../Plugin_API/pluginapi.h"
#include "../Shared_memory/Robot.h"
#include "../Shared_memory/Location.h"
#include "../Shared_memory/Laser.h"

#include "../Gui/input_singleton.h"
#include "../Gui/gui.h"
#include "../Gui/tab.h"
#include "../Gui/console.h"

#include <cstdlib>
#include <iostream>

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

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

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


Robot *patrol;

#define PLUGIN_NAME "Mapping"

/**
 * @brief Mapping plugin
 *
 * This plugin is a network server that accepts client conections to comunicate with other programs (running on the same computer or over the network)
 * @author     Patrick Heyer, patrickhey@prodigy.net.mx, Juan Herrera juan_antonio_@hotmail.com, Manuel Oropeza zodiacanimations@msn.com
 * @date       jul 04, 2012
 * @version    1.0
 **/

void MapBuilding_ICP(void)
{
	MRPT_START

	string INI_FILENAME("../data/GENRawICP.ini");	
	CConfigFile iniFile(INI_FILENAME);

	// ------------------------------------------
	//			Load config from file:
	// ------------------------------------------
	const string RAWLOG_FILE = iniFile.read_string("MappingApplication","rawlog_file","",  /*Force existence:*/ true);
	const unsigned int rawlog_offset = iniFile.read_int("MappingApplication","rawlog_offset",0,  /*Force existence:*/ true);
	const string OUT_DIR_STD = iniFile.read_string("MappingApplication","logOutput_dir","log_out",  /*Force existence:*/ true);
	const int LOG_FREQUENCY	= iniFile.read_int("MappingApplication","LOG_FREQUENCY",5,  /*Force existence:*/ true);
	const bool SAVE_POSE_LOG = iniFile.read_bool("MappingApplication","SAVE_POSE_LOG", false,  /*Force existence:*/ true);
	const bool SAVE_3D_SCENE = iniFile.read_bool("MappingApplication","SAVE_3D_SCENE", false,  /*Force existence:*/ true);
	const bool CAMERA_3DSCENE_FOLLOWS_ROBOT = iniFile.read_bool("MappingApplication","CAMERA_3DSCENE_FOLLOWS_ROBOT", true,  /*Force existence:*/ true);

	
	bool SHOW_PROGRESS_3D_REAL_TIME = false;
	int SHOW_PROGRESS_3D_REAL_TIME_DELAY_MS = 0;
	bool SHOW_LASER_SCANS_3D = false;

	MRPT_LOAD_CONFIG_VAR(SHOW_PROGRESS_3D_REAL_TIME, bool,  iniFile, "MappingApplication");
	MRPT_LOAD_CONFIG_VAR(SHOW_LASER_SCANS_3D , bool,  iniFile, "MappingApplication");
	MRPT_LOAD_CONFIG_VAR(SHOW_PROGRESS_3D_REAL_TIME_DELAY_MS, int, iniFile, "MappingApplication");

	const char* OUT_DIR = OUT_DIR_STD.c_str();

	// ------------------------------------
	//		Constructor of ICP-SLAM object
	// ------------------------------------
	CMetricMapBuilderICP mapBuilder;

	mapBuilder.ICP_options.loadFromConfigFile(iniFile, "MappingApplication");
	mapBuilder.ICP_params.loadFromConfigFile (iniFile, "ICP");

	// Construct the maps with the loaded configuration.
	mapBuilder.initialize();

	// ---------------------------------
	//   CMetricMapBuilder::TOptions
	// ---------------------------------
	mapBuilder.options.verbose = true;
   	mapBuilder.options.alwaysInsertByClass.fromString(iniFile.read_string("MappingApplication","alwaysInsertByClass",""));


	// Print params:
	printf("Running with the following parameters:\n");
	printf(" RAWLOG file:'%s'\n", RAWLOG_FILE.c_str());
	printf(" Output directory:\t\t\t'%s'\n",OUT_DIR);
	printf(" matchAgainstTheGrid:\t\t\t%c\n", mapBuilder.ICP_options.matchAgainstTheGrid ? 'Y':'N');
	printf(" Log record freq:\t\t\t%u\n",LOG_FREQUENCY);
	printf("  SAVE_3D_SCENE:\t\t\t%c\n", SAVE_3D_SCENE ? 'Y':'N');
	printf("  SAVE_POSE_LOG:\t\t\t%c\n", SAVE_POSE_LOG ? 'Y':'N');
	printf("  CAMERA_3DSCENE_FOLLOWS_ROBOT:\t%c\n",CAMERA_3DSCENE_FOLLOWS_ROBOT ? 'Y':'N');

	printf("\n");

	mapBuilder.ICP_params.dumpToConsole();
	mapBuilder.ICP_options.dumpToConsole();


	// Checks:
	ASSERT_(RAWLOG_FILE.size()>0)
	ASSERT_FILE_EXISTS_(RAWLOG_FILE)

	CTicTac	tictac, tictacGlobal, tictac_JH;
	int step = 0;
	string str;
	CSimpleMap finalMap;
	float t_exec;
	COccupancyGridMap2D::TEntropyInfo entropy;

	size_t rawlogEntry = 0;
	CFileGZInputStream rawlogFile(RAWLOG_FILE.c_str());


	// Prepare output directory:
	// --------------------------------
	deleteFilesInDirectory(OUT_DIR);
	createDirectory(OUT_DIR);

	// Open log files:
	// ----------------------------------
	CFileOutputStream f_log(format("%s/log_times.txt",OUT_DIR));
	CFileOutputStream f_path(format("%s/log_estimated_path.txt",OUT_DIR));
	CFileOutputStream f_pathOdo(format("%s/log_odometry_path.txt",OUT_DIR));


	// ----------------------------------------------------------
	//						Map Building
	// ----------------------------------------------------------
	CPose2D odoPose(0,0,0);

	tictacGlobal.Tic();
	for (;;) {

		CActionCollectionPtr action;
		CSensoryFramePtr observations;
		CObservationPtr observation;

		if (os::kbhit()) {
			char c = os::getch();
			if (c==27)
				break;
		}

		// Load action/observation pair from the rawlog:
		// --------------------------------------------------
		if (! CRawlog::getActionObservationPairOrObservation( rawlogFile, action, observations, observation, rawlogEntry) )
			break; // file EOF

		const bool isObsBasedRawlog = observation.present();
		std::vector<mrpt::slam::CObservation2DRangeScanPtr> lst_current_laser_scans;   // Just for drawing in 3D views

		if (rawlogEntry>=rawlog_offset)
		{
			// Update odometry:
			if (isObsBasedRawlog)
			{
				static CPose2D lastOdo;
				static bool firstOdo = true;
				if (IS_CLASS(observation,CObservationOdometry))
				{
					CObservationOdometryPtr o = CObservationOdometryPtr(observation);
					if (!firstOdo)
						odoPose = odoPose + (o->odometry - lastOdo);

					lastOdo=o->odometry;
					firstOdo=false;
				}
			}
			else
			{
				CActionRobotMovement2DPtr act= action->getBestMovementEstimation();
				if (act)
					odoPose = odoPose + act->poseChange->getMeanVal();
			}


			// Execute:
			// ----------------------------------------
			tictac.Tic();
			if (isObsBasedRawlog)
					mapBuilder.processObservation( observation );
			else	mapBuilder.processActionObservation( *action, *observations );
			t_exec = tictac.Tac();
			printf("Map building executed in %.03fms\n", 1000.0f*t_exec );

			const CMultiMetricMap* mostLikMap =  mapBuilder.getCurrentlyBuiltMetricMap();


			// Save the memory usage:
			// ------------------------------------------------------------------
			{
				printf("Saving memory usage...");
				unsigned long	memUsage = getMemoryUsage();
				FILE		*f=os::fopen( format("%s/log_MemoryUsage.txt",OUT_DIR).c_str() ,"at");
				if (f)
				{
					os::fprintf(f,"%u\t%lu\n",step,memUsage);
					os::fclose(f);
				}
				printf("Ok! (%.04fMb)\n", ((float)memUsage)/(1024*1024) );
			}

			// Save the robot estimated pose for each step:
			f_path.printf("%i %f %f %f\n",
				step,
				mapBuilder.getCurrentPoseEstimation()->getMeanVal().x(),
				mapBuilder.getCurrentPoseEstimation()->getMeanVal().y(),
				mapBuilder.getCurrentPoseEstimation()->getMeanVal().yaw() );


			f_pathOdo.printf("%i %f %f %f\n",step,odoPose.x(),odoPose.y(),odoPose.phi());

		} // end of if "rawlog_offset"...

		step++;
		printf("\n---------------- STEP %u | RAWLOG ENTRY %u ----------------\n",step, (unsigned)rawlogEntry);
	};

	printf("\n---------------- END!! (total time: %.03f sec) ----------------\n",tictacGlobal.Tac());

	// Save map:
	mapBuilder.getCurrentlyBuiltMap(finalMap);

	str = format("%s/_finalmap_.simplemap",OUT_DIR);
	printf("Dumping final map in binary format to: %s\n", str.c_str() );
	mapBuilder.saveCurrentMapToFile(str);

	CMultiMetricMap  *finalPointsMap = mapBuilder.getCurrentlyBuiltMetricMap();
	str = format("%s/_finalmaps_.txt",OUT_DIR);
	printf("Dumping final metric maps to %s_XXX\n", str.c_str() );
	finalPointsMap->saveMetricMapRepresentationToFile( str );

	MRPT_END
}


int cleanImage()
{
  cv::Mat img;
  img = cv::imread("/home/patrick/projects/Sabina/data/SlamOutGrid/_finalmaps_.txt_gridmap_no00.png",CV_LOAD_IMAGE_GRAYSCALE);
  
  if(img.empty())
  {
    std::cerr<<"ERROR: Could not read image" << std::endl;
    return 1;
  }
  
  cv::Mat binaryMat(img.size(),img.type());
  cv::threshold(img,binaryMat,150,255,cv::THRESH_BINARY);
  cv::imwrite("/home/patrick/projects/Sabina/data/SlamOutGrid/im_bw.png",binaryMat);
  cv::waitKey(0);
  
  return 0;
}

class Mapping : public IPlugin
{
public:
    void Main();
    void stop();
    void run();
};

PLUGIN_FUNC IPlugin *CreatePlugin()
{
    return new Mapping;
}

PLUGIN_FUNC void DestroyPlugin(IPlugin *r)
{
    delete r;
}

PLUGIN_DISPLAY_NAME(PLUGIN_NAME);

PLUGIN_INIT()
{
    // register our new plugin
    RegisterPlugin(PLUGIN_NAME, CreatePlugin, DestroyPlugin);
    return 0;
}

// ------------------------------------------------------
//	TestRawLogandLaser.
// ------------------------------------------------------
void TestRawLogandLaser() 
{
	// CSickLaserSerial laser;	
}

std::string accion;

void Mapping::Main()
{
  CFileGZOutputStream f("dataset.rawlog");	
  CObservation2DRangeScanPtr myObs = CObservation2DRangeScan::Create();	// This will work. Defining pointer other way
																			// won't work: Create() must be used!
  // CActivMediaRobotBase robot;

  static CObservationOdometry odom;
  odom.sensorLabel = "ODOMETRY";	
  CMetricMapBuilderICP mapBuilder;	
  CDisplayWindow3DPtr win3D;	
  CObservationPtr observation;	
  const CMultiMetricMap *mostLikMap;	
  CTicTac StopWatch;	
  CPose3D robotPose;
  string INI_FILENAME("../data/GENRawICP.ini");
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
  //	Load config from file:
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
  // #if MRPT_HAS_WXWIDGETS
  // 	if (SHOW_PROGRESS_3D_REAL_TIME)
  // 	{
  // 		win3D = CDisplayWindow3D::Create("VidICP", 600, 500);
  // 		win3D->setCameraZoom(20);
  // 		win3D->setCameraAzimuthDeg(-45);
  // 	}
  // #endif
	  // end of window creation
  // Robot initialization.
  // robot.loadConfig(inifile, "ROBOT");
  // robot.initialize();
  // cout << "Robot initialized." << endl;

  // Laser initialization, warm up period.
  // cout << "Waiting for laser to initialize..." << endl;
  // laser.loadConfig(inifile, "LASER1");
  // laser.initialize();					// This will raise an exception on error.
  cout << "loop" << endl;


bool times = true;
  while (true) 
  {		
    accion= patrol->getInstance().get_Action();
    
  if(accion=="Mapping")
  {
    
    StopWatch.Tic();
// 		try {
// 			laser.doProcessSimple(thereIsObservation, *myObs, hardError);
// 		}
// 
// 		catch (std::exception &e) {
// 			cerr << e.what() << endl;
// 			hardError = true;
// 		}

    if (hardError)
      printf("[TEST] Hardware error=true!!\n");
    TTimer3 = StopWatch.Tac();
     std::cout << "Mapping" << patrol->getInstance().laser->getLaserScan().size() << std::endl;
    if (patrol->getInstance().laser->getLaserScan().size() > 0) 
    {
      CountObs++;
    // if (!(CountObs % 2)) {		// Used to check timming and prevent data loss
      StopWatch.Tic();
    // robot.getOdometryFull(odo, v, w, left_ticks, right_ticks);	// meters and degrees.
    // odom.odometry = odo;
    // odom.encoderLeftTicks = left_ticks;
    // odom.encoderRightTicks = right_ticks;
    // odom.timestamp = myObs -> timestamp;
      myObs->scan.clear();
      myObs->scan = patrol->getInstance().laser->getLaserScan();
      
      myObs->validRange.resize(myObs->scan.size());
                for (size_t i = 0; i < myObs->scan.size(); i++)
                    myObs->validRange[i] = (myObs->scan[i] <= myObs->maxRange);
      
      printf("TAMAÑO laser from shared-memory to myObs: %d \n",myObs->scan.size());
      f << *myObs;						// Guardar en archivo abierto f, en ese orden.
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
      mostLikMap =  mapBuilder.getCurrentlyBuiltMetricMap();
      mapBuilder.getCurrentPoseEstimation() -> getMean(robotPose);
  // 				COpenGLScenePtr scene = COpenGLScene::Create();
  // 	
  // 	            COpenGLViewportPtr view = scene -> getViewport("main");
  // 	            ASSERT_(view);
  // 	            
  // 	            opengl::CSetOfObjectsPtr obj = opengl::CSetOfObjects::Create();
  // 				mostLikMap -> getAs3DObject(obj);
  // 				view -> insert(obj);
  // 				
  // 				CPose3DPDFPtr posePDF =  mapBuilder.getCurrentPoseEstimation();
  // 				CPose3D  curRobotPose;
  // 				posePDF -> getMean(curRobotPose);
  // 				
  // 				opengl::CSetOfObjectsPtr obj2 = opengl::stock_objects::RobotPioneer();
  // 				obj2 -> setPose(curRobotPose);
  // 				view -> insert(obj2);
  // 				
  // 				
  // 				opengl::COpenGLScenePtr &ptrScene = win3D -> get3DSceneAndLock();
  // 				ptrScene = scene;
  // 	
  // 				win3D -> unlockAccess3DScene();
  // 				
  // 				// Move camera:
  // 				win3D -> setCameraPointingToPoint(curRobotPose.x(), curRobotPose.y(), curRobotPose.z());
  // 	
  // 				// Update:
  // 				win3D -> forceRepaint();
  // 				// End of ICP map building
  // 				
      TTimer = StopWatch.Tac();
      cout << "Laser doProcessSimple time: " << TTimer3 * 1000 << " ms" << endl;
      cout << "rawlog Generation time: " << TTimer2 * 1000 << " ms" << endl;
      cout << "mapBuilder time: " << TTimer * 1000 << " ms" << endl;
      memUsage = getMemoryUsage();
      printf("Memory usage: (%.04fMb)\n\n", ((float)memUsage)/(1024*1024));
  // }			
    }
    mrpt::system::sleep(30);			
    // Loop period
  }	
//     if(accion!="seguir" && !)
//     {
//       while(times == true){
// 	/*mapBuilder.getCurrentlyBuiltMap(finalMap);
// 	str = format("%s/_finalmap_.simplemap",OUT_DIR);
// 	printf("Dumping final map in binary format to: %s\n", str.c_str() );
// 	mapBuilder.saveCurrentMapToFile(str);
// 
// 	CMultiMetricMap  *finalPointsMap = mapBuilder.getCurrentlyBuiltMetricMap();
// 	str = format("%s/_finalmaps_.txt",OUT_DIR);
// 	printf("Dumping final metric maps to %s_XXX\n", str.c_str());
// 	finalPointsMap->saveMetricMapRepresentationToFile(str);*/
// 
// 	//std::system("icp-slam ../data/GENRawICP.ini dataset.rawlog");
// 	
// 	cout << "Creating image" << endl;
// 	MapBuilding_ICP();
// 	
// 	sleep(2);
// 	cout << "Cleanning image" << endl;
// 	cleanImage();	
// 	sleep(1);
// 	times = false;
// 	
//       }
if(accion =="cleaningMap")
    {
     	cout << "Creating image" << endl;
	MapBuilding_ICP();
	sleep(2);
	cout << "Cleanning image" << endl;
	cleanImage();	
	sleep(1);
	times = false;
	
	patrol->getInstance().set_Current_destination("CHECK2");
	patrol->getInstance().set_Action("none");
    }
  }
}

void Mapping::run()
{
    pthread_create(&thread_id, NULL, &IPlugin::IncWrapper, this);
}

void Mapping::stop()
{
}