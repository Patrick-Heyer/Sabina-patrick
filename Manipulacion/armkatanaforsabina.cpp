/*
    <one line to give the program's name and a brief idea of what it does.>
    2012  J. Irving Vasquez  ivasquez@ccc.inaoep.mx

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "armkatanaforsabina.h"


ArmKatanaForSabina::ArmKatanaForSabina(): ArmKatana()
{
  currentArmPosition = UNKNOWN;
}


void ArmKatanaForSabina::deliverObject()
{
  cout << "Delivering object" << endl;
  if(currentArmPosition == CARRYING){
    katana->moveMotorToEnc(3,23561,true);
    katana->openGripper(true);
    //katana->closeGripper(true, 15);
    katana->moveMotorToEnc(3,30500,true);
  }
}

void ArmKatanaForSabina::moveToCarriyingPos()
{
  cout << "Moving to carriying position ..." << endl;
  
  if(currentArmPosition == HANGING){
    //-6102 2400 -5270 30500 -6323 12240
    vector<int> encoders(6);
    encoders[0] = -6102;
    encoders[1] = 2400 ;
    encoders[2] = -5270 ;
    encoders[3] = 30500 ;
    encoders[4] = -6323 ;
    encoders[5] = 12240;
    katana->moveRobotToEnc(encoders, true);
    
    currentArmPosition = CARRYING;
  }
  
  //katana->moveRobotTo(poseCarriying, true);
}


void ArmKatanaForSabina::moveToHanging()
{
  
  if(currentArmPosition == HANGING)
    return;
  
  if(currentArmPosition == CALIBRATED){
    try {
      vector<int> encoders(6);
      
      encoders[0] = 6705;
      encoders[1] = -30500;
      encoders[2] = -30500;
      encoders[3] = 30500;
      encoders[4] = 30500;
      encoders[5] = 12240;
      katana->moveRobotToEnc(encoders, true);
      
      encoders[0] = 6370;
      encoders[1] = -4876;
      encoders[2] = -16604;
      encoders[3] = 15969;
      encoders[4] = 5447;
      encoders[5] = 12240;
      katana->moveRobotToEnc(encoders, true);
      
      currentArmPosition = HANGING;
    }
    catch(Exception &e) {
	std::cout << "ERROR: " << e.message() << std::endl;
	return;
    }
    
    return;
  }
  
  if(currentArmPosition == CARRYING){
    vector<int> encoders(6);
    
    try{
      katana->moveMotorToEnc(0,6370,true);
      
      encoders[0] = 6370;
      encoders[1] = -4876;
      encoders[2] = -16604;
      encoders[3] = 15969;
      encoders[4] = 5447;
      encoders[5] = 12240;
      katana->moveRobotToEnc(encoders, true);
      
      currentArmPosition = HANGING;
    } catch(Exception &e) {
	std::cout << "ERROR: " << e.message() << std::endl;
	return;
    }
    
    return;
  }
  
  if(currentArmPosition == UNKNOWN);
  {
    try {
      vector<int> encoders(6);
      
      encoders[0] = 6370;
      encoders[1] = -4876;
      encoders[2] = -16604;
      encoders[3] = 15969;
      encoders[4] = 5447;
      encoders[5] = 12240;
      katana->moveRobotToEnc(encoders, true);
      
      currentArmPosition = HANGING;
    }
    catch(Exception &e) {
	std::cout << "ERROR: " << e.message() << std::endl;
	return;
    }
    
    return;
  }

  if(currentArmPosition == STORED)
    return;

}


void ArmKatanaForSabina::store()
{
  if(currentArmPosition == STORED)
    return;
  
  if(currentArmPosition != HANGING)
    moveToHanging();
  
  if(currentArmPosition == HANGING){
    vector<int> encoders(6);
    
    try{
      // 1
      encoders[0] = 6370 ;
      encoders[1] = -11037 ;
      encoders[2] = -16604 ;
      encoders[3] = 15969 ;
      encoders[4] = 5447 ;
      encoders[5] = 12240 ;
      cout << "1/8" << endl;
      katana->moveRobotToEnc(encoders, true);
      
      // 2
      cout << "2/8" << endl;
      katana->moveMotorToEnc(3,7012,true);
      
      // 3
      cout << "3/8" << endl;
      katana->moveMotorToEnc(1,-17098,true);
      
      // 4
      cout << "4/8" << endl;
      katana->moveMotorToEnc(3,-418,true);
      
      // 5
      cout << "5/8" << endl;
      katana->moveMotorToEnc(1,-1887,true);
      
      // 6
      cout << "6/8" << endl;
      katana->moveMotorToEnc(3, 2574, true);
      
      // 7
      cout << "7/8" << endl;
      katana->moveMotorToEnc(1, 1085,true);
      
      // 8
      cout << "8/8" << endl;
      katana->moveMotorToEnc(3, 4263, true);
      
      katana->switchRobotOff();
      currentArmPosition = STORED;
    } catch(Exception &e) {
	std::cout << "ERROR: " << e.message() << std::endl;
	return;
    }
  }
}


bool ArmKatanaForSabina::equalVectors(std::vector< int > A, std::vector< int > B, int tolerance)
{
  if(A.size() != B.size())
    return false;
  
  for(int i = 0;i < A.size(); i++){
    if( abs(A[i] - B[i])>tolerance )
      return false;
  }
  
  return true;
}


int ArmKatanaForSabina::Grasping(double x, double y, double z)
{
  //asumption
  x = 0.0;
  
  //approaching point
  double h,ha,angulo;
  double distancia=100;
  angulo = atan2(y,z);
  h = sqrt(pow(z,2)+pow(y,2));
  ha = h - distancia;
  double za = ha * cos(angulo);
  double ya = ha * sin(angulo);
  double xa = x; 
  
  // increments to test different approaching angles, over theta
  int k = 10;
  int i = 0;
  double increment = M_PI /( 2* (double)k);
  double signo = -1;
  
  double phi = 0;
  double theta = 0;
  double psi = M_PI/2;
  
  vector<int> enc0, enc1; 
  
  try{
    
  katana->openGripper(true);
  /// Reach approaching position
  while(i<k){
    //signo = signo * -1;
    signo = 1;
    theta = signo * i * increment;
    
    cout << "Goal Approaching " << xa << " " << ya << " " << za << " " << phi << " " << theta << " " << psi << endl;
    
    enc0 = katana->getRobotEncoders(true);
    katana->moveRobotTo(xa,ya,za,phi,theta, psi, true);
    sleep(1);
    enc1 = katana->getRobotEncoders(true);
    
    if(!equalVectors(enc0,enc1,5)){
	cout << "Was a movement, I guess i reach the approaching position" << endl;
	break;
    }
    
    if(signo == 1){
      i++;
    } 
  }
  
  ///Reach grasping position
  i = 0;
  while(i<k){
    //signo = signo * -1;
    signo = 1;
    theta = signo * i * increment;
    
    cout << "Goal Grasping" << x << " " << y << " " << z << " " << phi << " " << theta << " " << psi << endl;
    
    enc0 = katana->getRobotEncoders(true);
    katana->moveRobotTo(x,y,z,phi,theta, psi, true);
    sleep(1);
    enc1 = katana->getRobotEncoders(true);
    
    if(!equalVectors(enc0,enc1,5)){
	cout << "Was a movement, I guess i reach the object" << endl;

	break;
    }
    
    if(signo == 1){
      i++;
    } 
  }
  
  katana->closeGripper(false);
  sleep(9);
  int grip_enc = katana->getMotorEncoders(5,true);
  katana->moveMotorToEnc(5,grip_enc,true);
  
  // return to carriying
  vector<int> encoders_end(5);
  encoders_end[0] = -6102 ;
  encoders_end[1] = 2400 ;
  encoders_end[2] = -5270 ;
  encoders_end[3] = 30500 ;
  encoders_end[4] = -6323 ;
  //encoders[5] = 12240 ;
  katana->moveRobotToEnc(encoders_end.begin(), encoders_end.end(), true);
  cout << "done grasping" << endl;
  currentArmPosition = CARRYING;
  
  } catch(Exception &e) {
	std::cout << "ERROR: " << e.message() << std::endl;
	//return -1;
	katana->unBlock();
	return 0;
  }
  
  return 0;
}


int ArmKatanaForSabina::testGrasping(double x, double y, double z)
{
  //asumption
  x = 0.0;
  
  //approaching point
  double h,ha,angulo;
  double distancia=100;
  angulo = atan2(y,z);
  h = sqrt(pow(z,2)+pow(y,2));
  ha = h - distancia;
  double za = ha * cos(angulo);
  double ya = ha * sin(angulo);
  double xa = x; 
  
  int k = 10;
  int i = 0;
  double increment = M_PI /( 2* (double)k);
  double signo = -1;
  double phi = 0;
  
  double theta = 0;
  // modification
  //double theta = asin(z/h);
  //double theta_a = asin(za / ha);
  
  double psi = M_PI/2;
  
  std::vector<int> solutionEncoders;
  
  katana->openGripper(true);
  
  while(i<k){
    //signo = signo * -1;
    theta = i * increment;
    
    //katana->IKCalculate(xa,ya,za,phi,theta,psi,solutionEncoders);
    cout << "Goal X " << xa << " " << ya << " " << za << " " << phi << " " << theta << " " << psi << endl;

    try{
      vector<int> enc0, enc1; 
      vector<int>::iterator it;
      //clock_t begin = clock();  
      enc0 = katana->getRobotEncoders(true);
      for(it=enc0.begin(); it!= enc0.end(); it++)
      {
	cout << *it << " ";
      } cout << endl;
      
      katana->moveRobotTo(xa,ya,za,phi,theta, psi, true);
      
      //sleep(1);
      
      enc1 = katana->getRobotEncoders(true);
      for(it=enc1.begin(); it!= enc1.end(); it++)
      {
	cout << *it << " ";
      }	cout << endl;
      
      //double time_threshold = 0.5;
      //TODO decide if there was a movement
      if(!equalVectors(enc0,enc1,5)){
	cout << "was a movement" << endl;
	
	// TODO tambien tratar varios antulos de theta
	theta = 0;
	katana->moveRobotTo(x,y,z,phi,theta,psi);
	
	// close the gripper
	//katana->
	
	katana->closeGripper(false);
	sleep(9);
	int grip_enc = katana->getMotorEncoders(5,true);
	katana->moveMotorToEnc(5,grip_enc,true);
	// return to carriying
	vector<int> encoders_end(5);
	encoders_end[0] = -6102 ;
	encoders_end[1] = 2400 ;
	encoders_end[2] = -5270 ;
	encoders_end[3] = 30500 ;
	encoders_end[4] = -6323 ;
	//encoders[5] = 12240 ;
	katana->moveRobotToEnc(encoders_end.begin(), encoders_end.end(), true);
	cout << "done grasping" << endl;
	currentArmPosition = CARRYING;
	// break
	return 0;
	//break;
      }
  
    } catch(Exception &e) {
	std::cout << "ERROR: " << e.message() << std::endl;
	//return -1;
	katana->unBlock();
	return 0;
    }
    //if(signo == -1)
    i++;
  }
  
  return 0;
}


int ArmKatanaForSabina::graspObjectAtKinectImage(double x, double y)
{
  double center = 640/2;
  int posx = x - center;
  testGrasping(0,100,600);
}


void ArmKatanaForSabina::bringOut()
{
  
  vector<int> encoders(6);
    
    try{
      katana->switchRobotOn();
 /*   cout << "8/8" << endl;
      katana->moveMotorToEnc(3, 4263, true);
      
      // 6
      cout << "6/8" << endl;
      katana->moveMotorToEnc(3, 2574, true);
      
      // 7
      cout << "7/8" << endl;
      katana->moveMotorToEnc(1, 1085,true);
      

      
      // 5
      cout << "5/8" << endl;
      katana->moveMotorToEnc(1,-1887,true);
      
      // 4
      cout << "4/8" << endl;
      katana->moveMotorToEnc(3,-418,true);
      
            // 3
      cout << "3/8" << endl;
      katana->moveMotorToEnc(1,-17098,true);
      
      // 2
      cout << "2/8" << endl;
      katana->moveMotorToEnc(3,7012,true);

      
      // 1
      encoders[0] = 6370 ;
      encoders[1] = -11037 ;
      encoders[2] = -16604 ;
      encoders[3] = 15969 ;
      encoders[4] = 5447 ;
      encoders[5] = 12240 ;
      cout << "1/8" << endl;
      katana->moveRobotToEnc(encoders, true);
 */     

      currentArmPosition = HANGING;
    } catch(Exception &e) {
	std::cout << "ERROR: " << e.message() << std::endl;
	return;
    }

}
