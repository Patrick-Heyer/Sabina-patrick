
/** @mainpage SABINA
* @authors Dr. Enrique Sucar esucar@inaoep.mx, Dr Eduardo Morales emorales@inaoep.mx, Juan Herrera juan_antonio_@hotmail.com, Patrick Heyer patrickhey@prodigy.net.mx, Angel Oca&ntilde;a ahop_289@hotmail.com, Manuel Oropeza zodiacanimations@msn.com, Miguel Palacios mkpalacio@gmail.com, Irving V&aacute;squez ivasquez@inaoep.mx
*
* @section intro Introduction
* Welcome to the main documentation of the Sabina architecture developed
* by the Markovito team of 2012 for the "@home" category of RoboCup
*
* The Sabina project is aimed to develop a software platfotm for a service robot that can perform tasks defined by the rules for service robots for robocup. This tasks include recognizing people, objects, navigating safely,
* and comuncating using voice synthesis and recognition.\n \n \n For a brief tutorial on runing and configuring sabina check \ref Sample_Run
*
* The Architecture uses a modular aproach based on
*
* - Shared memory
* -# Main class of this system is Robot.
* -# Main way of comunication between modules and information repository.
* -# Uses a singleton GOD object aproach.
* -# Only contains information it does not process or validate this info.
* -# Is expandable using inheritance.
*
* - Plugin architecture
* -# Loads plugins to perform a particular action, and runs it (in the same thread or in its own thread).
* -# PluginManager is in charge of loading and running plugins dynamically.
* -# IPlugin is the base class for all modules.
* -# Most plugins after initializing run in a infinite loop (generaly a for(;;){};).
*
* - Modules
* -# Gestos. Recognizes a hand gesture using OpenNi and the kinect.
* -# Manipulacion. Is in charge of moving the arm (katana 6M in the case of our robot).
* -# Navigation_Planing. Using the robots position and a final destination plans a obstacle free route for the robot.
* -# Navegacion. Moves the robot detecting obstacles and avoiding them.
* -# Net_server. Is a plugin that starts a server and listens to any client you want to conect.
* -# Prueba_Dummy. This is a plugin that sloves acctions not solved by other modules
* -# Vision_Faces. Detects learns and recognizes face.
* -# Vision_Objects. Recognizes objects based on code developed at CIMAT. (Judith Esquivel judith@cimat.mx)
* -# Vision_Surf. Recognizes objects based on the OpenCV surf sample
* -# Voice_Recognition. Uses sphinx api to recognize voice.
* -# Voice_Synthesis. Makes voice synthesis using festival system calls.
*

* <hr>
* @section requirements Requirements: This is a list of the required system libraries to run the whole project (tu run a sub-set the requirements may be less)
*
* -# OpenCV 2.3
* -# OpenGL 1.4
* -# OpenNI 1.5.2.23
* -# Festival 1.96
* -# Sphinx2 0.6
* -# Aria 2.7.3
* -# KNI
* -# mrpt 0.9.6
* -# PCL
* -# SPUDD 3.6.2
*
* <hr>
* @section limitations Limitations: This is a list of things that we managed to get to work but that could break on other systems or architectures
*
* -# MDP for the moment depends on libraries found in the MDP/lib directory these libraries where not tested on other systems.
*
* <hr>
* @section todo TODO: this list is rather extensive.
* So we will limit it to corrections required at this moment to improve usability and will exclude all changes required by particular modules to work outside of a controled envoirement
*
* -# Create a wrapper for the shared memory so the acces is simpler (shorter).
* -# Automate the loading of modules from directory.
* -# Load and unload plugins just for the time they require to be in memory.
* -# Create a configuration plugin to edit configuration files.
* -# Control CPU load to focus on active plugin.
* -# Improve plugin initialization time (so they can be loaded and unloaded in *real-time).
* -# Convert MDP code to plugin.
*
*
*/
