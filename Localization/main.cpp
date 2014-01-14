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
#include <pthread.h>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>

#include "../Gui/gui.h"
#include "../Gui/video.h"
#include "../Plugin_API/pluginapi.h"
#include "../Shared_memory/Robot.h"

#include "../Gui/input_singleton.h"
#include "../Gui/gui.h"
#include "../Gui/tab.h"
#include "../Gui/console.h"
#include "../Gui/video.h"



Robot *patrol;
Tab *pluginTab;

/***********************************************************
* Online localization using CMonteCarloLocalization and CParticleFilter classes.
* Based on pf-localization source code.
* No rawlog file generation.
* Results differ from offline version.
* Timmings depend on localization algorithm: up tp 500ms
***********************************************************/

#include <base/include/mrpt/base.h>
#include <base/include/mrpt/utils.h>
#include <slam/include/mrpt/slam.h>
#include <maps/include/mrpt/opengl/CPlanarLaserScan.h>
#include <maps/include/mrpt/maps.h>
#include <hwdrivers/include/mrpt/hwdrivers/CSickLaserSerial.h>
#include <hwdrivers/include/mrpt/hwdrivers/CActivMediaRobotBase.h>
#include <obs/include/mrpt/slam/CActionRobotMovement2D.h>
#include <obs/include/mrpt/obs.h>




using namespace mrpt;
using namespace mrpt::utils;
using namespace mrpt::slam;
using namespace mrpt::opengl;
using namespace mrpt::hwdrivers;
using namespace mrpt::poses;
using namespace std;
using namespace mrpt::system;
using namespace mrpt::math;
using namespace mrpt::random;

#include "../MDP/MDP.h"

#define PLUGIN_NAME "Localization"

/**
 * @brief Localization plugin
 *
 * This plugin uses a MRPT library to localize the robot in its envoirement (ParticleFilter)
 * @author     Patrick Heyer, patrickhey@prodigy.net.mx
 * @date       jul 04, 2012
 * @version    1.0
 **/
class Localization : public IPlugin
{
public:
    void Main();
	void stop();
    void run();
};

PLUGIN_FUNC IPlugin *CreatePlugin()
{
    return new Localization;
}

PLUGIN_FUNC void DestroyPlugin(IPlugin *r)
{
    delete r;
}

PLUGIN_DISPLAY_NAME(PLUGIN_NAME);

PLUGIN_INIT()
{
    // register our new plugin
    std::cout << "PLUGIN_INIT" << std::endl;
    RegisterPlugin(PLUGIN_NAME, CreatePlugin, DestroyPlugin);
    return 0;
}

void Localization::Main() {

    //Gui::getInstance();
    //pluginTab = new Tab("Localization"); 
    
    

	//CSickLaserSerial laser;

	CObservationOdometryPtr odom = CObservationOdometry::Create();
    odom ->sensorLabel = "ODOMETRY";

	//CActivMediaRobotBase robot;

	

	CPose3D robotPose;

	string INI_FILENAME("../data/Localization/GENRawPF.ini");
	CConfigFile iniFile(INI_FILENAME);

	CPose2D odo;
	bool thereIsObservation, hardError, flag;
	double v, w;
	int64_t left_ticks, right_ticks;

	int repetition = 0;
	int PARTICLE_COUNT = 1000;

	vector_int particles_count; // Number of initial particles (if size>1, run the experiments N times)

	// Load configuration:
	// -----------------------------------------
	string iniSectionName ("LocalizationExperiment");
	string laserSection("LASER1");

	// Mandatory entries:
	iniFile.read_vector(iniSectionName, "particles_count", vector_int(1,0), particles_count, /*Fail if not found*/true );
	string		OUT_DIR_PREFIX		= iniFile.read_string(iniSectionName,"logOutput_dir","", /*Fail if not found*/true );



	// Non-mandatory entries:
	string MAPA_ARCHIVO = iniFile.read_string(iniSectionName,"map_file","" );
	size_t rawlog_offset = iniFile.read_int(iniSectionName,"rawlog_offset",0);
	string GT_FILE = iniFile.read_string(iniSectionName,"ground_truth_path_file","");
	int NUM_REPS = iniFile.read_int(iniSectionName,"experimentRepetitions",1);
	int SCENE3D_FREQ = iniFile.read_int(iniSectionName,"3DSceneFrequency",10);
	bool SCENE3D_FOLLOW = iniFile.read_bool(iniSectionName,"3DSceneFollowRobot",true);
	unsigned int testConvergenceAt = iniFile.read_int(iniSectionName,"experimentTestConvergenceAtStep",-1);

	bool SAVE_STATS_ONLY = iniFile.read_bool(iniSectionName,"SAVE_STATS_ONLY",false);

	bool SHOW_PROGRESS_3D_REAL_TIME = iniFile.read_bool(iniSectionName,"SHOW_PROGRESS_3D_REAL_TIME",false);
	int SHOW_PROGRESS_3D_REAL_TIME_DELAY_MS = iniFile.read_int(iniSectionName,"SHOW_PROGRESS_3D_REAL_TIME_DELAY_MS",1);
	double STATS_CONF_INTERVAL = iniFile.read_double(iniSectionName,"STATS_CONF_INTERVAL",0.2);

#if !MRPT_HAS_WXWIDGETS
	SHOW_PROGRESS_3D_REAL_TIME = false;
#endif


	// Default odometry uncertainty parameters in "dummy_odom_params" depending on how fast the robot moves, etc...
	//  Only used for observations-only rawlogs:
	CActionRobotMovement2D::TMotionModelOptions dummy_odom_params;
	dummy_odom_params.modelSelection = CActionRobotMovement2D::mmGaussian;
	dummy_odom_params.gausianModel.minStdXY  = iniFile.read_double("DummyOdometryParams","minStdXY",0.04);
	dummy_odom_params.gausianModel.minStdPHI = DEG2RAD(iniFile.read_double("DummyOdometryParams","minStdPHI", 2.0));


	// PF-algorithm Options:
	// ---------------------------
	CParticleFilter::TParticleFilterOptions pfOptions;
	pfOptions.loadFromConfigFile( iniFile, "PF_options" );

	// PDF Options:
	// ------------------
	TMonteCarloLocalizationParams pdfPredictionOptions;
	pdfPredictionOptions.KLD_params.loadFromConfigFile( iniFile, "KLD_options");

	// Metric map options:
	// -----------------------------
	TSetOfMetricMapInitializers mapList;
	mapList.loadFromConfigFile( iniFile,"MetricMap");

	cout<< "-------------------------------------------------------------\n"
		<< "\t MAPA_ARCHIVO = \t "      << MAPA_ARCHIVO << endl
		<< "\t GT_FILE = \t "       << GT_FILE << endl
		<< "\t OUT_DIR_PREFIX = \t "<< OUT_DIR_PREFIX << endl
		<< "\t #particles = \t "    << particles_count << endl
		<< "-------------------------------------------------------------\n";
	pfOptions.dumpToConsole();
	mapList.dumpToConsole();


	// --------------------------------------------------------------------
	//						EXPERIMENT PREPARATION
	// --------------------------------------------------------------------
	CTicTac tictac;
	CSimpleMap simpleMap;
	CParticleFilter::TParticleFilterStats PF_stats;

	// Load the set of metric maps to consider in the experiments:
	CMultiMetricMap metricMap;
	metricMap.setListOfMaps(&mapList);
	mapList.dumpToConsole();


	// Load the map (if any):
	// -------------------------
	if (MAPA_ARCHIVO.size()) {
		ASSERT_( fileExists(MAPA_ARCHIVO) );

		// Detect file extension:
		// -----------------------------
		string mapExt = lowerCase( extractFileExtension( MAPA_ARCHIVO, true ) ); // Ignore possible .gz extensions

		if ( !mapExt.compare("simplemap")) {
			// It's a ".simplemap":
			// -------------------------
			printf("Loading '.simplemap' file...");
			CFileGZInputStream(MAPA_ARCHIVO) >> simpleMap;
			printf("Ok\n");

			ASSERT_( simpleMap.size()>0 );

			// Build metric map:
			// ------------------------------
			printf("Building metric map(s) from '.simplemap'...");
			metricMap.loadFromProbabilisticPosesAndObservations(simpleMap);
			printf("Ok\n");
		}
		else if ( !mapExt.compare("gridmap") ) {
			// It's a ".gridmap":
			// -------------------------
			printf("Loading gridmap from '.gridmap'...");
			ASSERT_( metricMap.m_gridMaps.size()==1 );
			CFileGZInputStream(MAPA_ARCHIVO) >> (*metricMap.m_gridMaps[0]);
			printf("Ok\n");
		}
		else {
			THROW_EXCEPTION_CUSTOM_MSG1("Map file has unknown extension: '%s'",mapExt.c_str());
		}
	}

	COccupancyGridMap2D::TEntropyInfo gridInfo;

	// The gridmap:
	if (metricMap.m_gridMaps.size()) {
		metricMap.m_gridMaps[0]->computeEntropy( gridInfo );
		printf("The gridmap has %.04fm2 observed area, %u observed cells\n", gridInfo.effectiveMappedArea, (unsigned) gridInfo.effectiveMappedCells );

	}


	int M = PARTICLE_COUNT;
	CMonteCarloLocalization2D  pdf(M);

	// PDF Options:
	pdf.options = pdfPredictionOptions;

	pdf.options.metricMap = &metricMap;

	// Create the PF object:
	CParticleFilter PF;
	PF.m_options = pfOptions;

	size_t step = 0;
	size_t rawlogEntry = 0;


	// Initialize the PDF:
	// -----------------------------
	tictac.Tic();
	if (!iniFile.read_bool(iniSectionName,"init_PDF_mode",false, /*Fail if not found*/true))
		pdf.resetUniformFreeSpace(
			metricMap.m_gridMaps[0].pointer(),
			0.7f,
			PARTICLE_COUNT ,
			iniFile.read_float(iniSectionName,"init_PDF_min_x",0,true),
			iniFile.read_float(iniSectionName,"init_PDF_max_x",0,true),
			iniFile.read_float(iniSectionName,"init_PDF_min_y",0,true),
			iniFile.read_float(iniSectionName,"init_PDF_max_y",0,true),
			DEG2RAD(iniFile.read_float(iniSectionName,"init_PDF_min_phi_deg",-180)),
			DEG2RAD(iniFile.read_float(iniSectionName,"init_PDF_max_phi_deg",180))
			);
	else
		pdf.resetUniform(
			iniFile.read_float(iniSectionName,"init_PDF_min_x",0,true),
			iniFile.read_float(iniSectionName,"init_PDF_max_x",0,true),
			iniFile.read_float(iniSectionName,"init_PDF_min_y",0,true),
			iniFile.read_float(iniSectionName,"init_PDF_max_y",0,true),
			DEG2RAD(iniFile.read_float(iniSectionName,"init_PDF_min_phi_deg",-180)),
			DEG2RAD(iniFile.read_float(iniSectionName,"init_PDF_max_phi_deg",180)),
			PARTICLE_COUNT
			);


	printf("PDF of %u particles initialized in %.03fms\n", M, 1000*tictac.Tac());

	CPose2D pdfEstimation, odometryEstimation(4.4, -1.9, 2.76);
	CMatrixDouble cov;
	bool end = false;

	TTimeStamp cur_obs_timestamp;
	// odometryEstimation(4.4, -1.9, 158.0);
	// end pf-localization initialization.


// // 	CFileOutputStream dataFile_est, dataFile_odo;
// // 	dataFile_est.open("./data_est.txt");
// // 	dataFile_odo.open("./data_odo.txt");
// // 	dataFile_est.printf("est = [");
// // 	dataFile_odo.printf("odo = [");


	//Robot initialization.
	
	//robot.loadConfig(iniFile, "ROBOT");
	//robot.initialize();
	//cout << "Robot initialized." << endl;

	//Laser initialization, warm up period.
	//cout << "Waiting for laser to initialize..." << endl;
	//laser.loadConfig(iniFile, "LASER1");
	//laser.initialize();					// This will raise an exception on error.
	//cout << "Laser initialized." << endl;

	CPose2D ForceLocPose(iniFile.read_float(iniSectionName, "x_loc", 0, true),
								iniFile.read_float(iniSectionName, "y_loc", 0, true),
								iniFile.read_float(iniSectionName, "phi_loc", 0, true) * 3.1416 / 180);
	
	int intFLocCounter = iniFile.read_int(iniSectionName, "FLocTries", 0, true);
	int FLocCounter = intFLocCounter;
	
	
	CPose3D m_sensorPose = CPose3D(
		iniFile.read_float(laserSection,"pose_x",0),
		iniFile.read_float(laserSection,"pose_y",0),
		iniFile.read_float(laserSection,"pose_z",0),
		DEG2RAD(iniFile.read_float(laserSection,"pose_yaw",0) ),
		DEG2RAD(iniFile.read_float(laserSection,"pose_pitch",0) ),
		DEG2RAD(iniFile.read_float(laserSection,"pose_roll",0) )
		);
		
	bool FTime = true;
	bool primera=true;

	for(;;) {

Location punto_temp;
		try {

		if(patrol->getInstance().get_Action()=="localizar" && primera==true)
			{
				primera=false;
			}
			
			
			if (patrol->getInstance().laser->getLaserScan().size()>0 && primera==false ) {

				CObservation2DRangeScanPtr myObs= CObservation2DRangeScan::Create();
				CActionCollectionPtr action;
				CSensoryFramePtr observations;
				CSensoryFrame SF;
				CObservationPtr obs;


				observations = CSensoryFrame::Create();
				observations -> insert(obs);

				
				std::vector<float>::const_iterator it;
				int cont=1;
				myObs->scan.clear();			
				myObs->scan = patrol->getInstance().laser->getLaserScan();
				
				//printf("TAMAÑO laser from shared-memory to myObs: %d \n",myObs->scan.size());
				
				myObs->timestamp = mrpt::system::now();

				myObs->sensorLabel  = "SICKLMS";	// Set label

				// Extract the timestamp of the sensor:

				// And the scan ranges:
				myObs->rightToLeft = true;
				myObs->aperture = M_PIf;
				myObs->maxRange	= 81.0;
				myObs->stdError = 0.003f;
				myObs->sensorPose = m_sensorPose;
				

	
				myObs->validRange.resize(myObs->scan.size());
				for (size_t i = 0; i < myObs->scan.size(); i++)
					myObs->validRange[i] = (myObs->scan[i] <= myObs->maxRange);
				
//  				for (it = myObs->scan.begin(); it != myObs->scan.end(); ++it)
//  				{
//  					printf("%d,%f ",cont++, (*it));
//  				}	
// 				
				SF.insert(myObs);
				
				

				// ActionCollection: Just one action with a dummy odometry
				action = CActionCollection::Create();

				CActionRobotMovement2D dummy_odom;

				// TODO: Another good idea would be to take CObservationOdometry objects and use that information, if available.
				dummy_odom.computeFromOdometry(CPose2D(0,0,0),dummy_odom_params);
				action->insert(dummy_odom);


				CPose2D expectedPose; // Ground truth

				if (observations->size() > 0)
					//cur_obs_timestamp = observations->getObservationByIndex(0)->timestamp;
					cur_obs_timestamp = SF.getObservationByIndex(0)->timestamp;

				if (step >= rawlog_offset) {
					// Do not execute the PF at "step=0", to let the initial PDF to be
					//   reflected in the logs.
					if (step > rawlog_offset) {
						// Show 3D?
						// ----------------------------------------
						// RUN ONE STEP OF THE PARTICLE FILTER:
						// ----------------------------------------
						tictac.Tic();
						//printf("Step %u -- Executing ParticleFilter on %u particles....", (unsigned int)step, (unsigned int)pdf.particlesCount());

						// 

						// Main localization class method
						// PF.executeOn(pdf, action.pointer(), observations.pointer(), &PF_stats);
						PF.executeOn(pdf, action.pointer(), &SF, &PF_stats);
						//printf(" Done! in %.03fms, ESS=%f\n", 1000.0f * tictac.Tac(), pdf.ESS());
						
					}

					pdf.getMean(pdfEstimation);
					pdf.getCovariance(cov);
					

					// Text output:
// 					cout << "    Odometry est: " << odometryEstimation << "\n";
					//cout << "         PDF est: " << pdfEstimation << ", ESS (B.R.)= " << PF_stats.ESS_beforeResample << "\n";
// 					cout << "FLocCounter: " << FLocCounter << endl;
// 					cout << "Distance: " << pdfEstimation.distanceTo(ForceLocPose) << endl;
					//cout << "Determinant. " << cov.det() << endl;
					
					
					
					
					
 					if (FLocCounter > 0 && FTime)
						FLocCounter--;
					else if (FLocCounter == 0 && cov.det() < 1 && FTime) {
						
						cout << "Primer if..." << endl;
						
							if (pdfEstimation.distanceTo(ForceLocPose) < 0.5) {
								cout << "Reached" << endl;
								FTime = false;
								punto_temp;
								punto_temp.set(pdfEstimation.x()*1000,pdfEstimation.y()*1000,pdfEstimation.phi());
								patrol->getInstance().setEstimatedPosition(punto_temp);
								usleep(500);
								patrol->getInstance().set_Action(cambiar_estado("localizado", "si"));
								cout << "Force localization deactivated." << endl;
								cout << "*****LOCALIZADO*****" << endl;
								patrol->getInstance().localized = true;
								FLocCounter = intFLocCounter;
							}
							else {
								cout << "Not reached" << endl;
								pdf.resetUniformFreeSpace(		// This section resets particles in order to start another
										metricMap.m_gridMaps[0].pointer(),	//search. Argument values obtained from ini archive.
										0.7f,
										PARTICLE_COUNT,
										iniFile.read_float(iniSectionName,"init_PDF_min_x",0,true),
										iniFile.read_float(iniSectionName,"init_PDF_max_x",0,true),
										iniFile.read_float(iniSectionName,"init_PDF_min_y",0,true),
										iniFile.read_float(iniSectionName,"init_PDF_max_y",0,true),
										DEG2RAD(iniFile.read_float(iniSectionName,"init_PDF_min_phi_deg",-180)),
										DEG2RAD(iniFile.read_float(iniSectionName,"init_PDF_max_phi_deg",180))
								);
								cout << "Force localization operating..." << endl;
								FLocCounter = intFLocCounter;
							}
						}
					
	
					
					
					// cout << observations->getObservationByIndex(0)->sensorLabel << endl;
// 					cout << SF.getObservationByIndex(0)->sensorLabel << endl;
// 					cout << "Covariance: " << "\n" << cov;
// 					cout << "Determinant:"<< "\n" << cov.det() << "\n";
// 					cout << "Distance: " << pdfEstimation.distanceTo(odometryEstimation) << endl;
// 					cout << odo.x() << "	" << odo.y() << "	" << odo.phi() * 180 / 3.1416 << endl;

//   					patrol->getInstance().Position.set_X(pdfEstimation.x()*1000/50);
//   					patrol->getInstance().Position.set_Y(pdfEstimation.y()*1000/50);
// 					
					
					//TODO mofied
			
						punto_temp.set(pdfEstimation.x()*1000,pdfEstimation.y()*1000,0);
						patrol->getInstance().setEstimatedPosition(punto_temp);
	




					//cout << endl;

					// DataFile.printf("%f %f  %f %f  %f %f\n", pdfEstimation.x(), odo.x(), pdfEstimation.y(), odo.y(),
						//				pdfEstimation.phi() *180 / 3.1416, odo.phi() * 180 / 3.1416);

// 					dataFile_est.printf("%f, %f, %f;\n", pdfEstimation.x(), pdfEstimation.y(), pdfEstimation.phi() * 180 / 3.1416);
// 					dataFile_odo.printf("%f, %f, %f;\n", odo.x(), odo.y(), odo.phi() * 180 / 3.1416);

				} // end if step >= rawlog_offset

				step++;
				mrpt::system::sleep(10);
			}

			// dataFile_est.printf("];");
			// dataFile_odo.printf("];");

		}

		catch (std::exception &e) {
			cout << "Localization#"<<endl;
			cerr << e.what() << endl;
			hardError = true;
		}

// 		if (hardError)
// 			printf("[TEST] Hardware error=true!!\n");

		mrpt::system::sleep(10);			// Main loop period

	} // while

} // function

void Localization::run()
{
    pthread_create(&thread_id, NULL, &IPlugin::IncWrapper, this);
}

void Localization::stop()
{

}


