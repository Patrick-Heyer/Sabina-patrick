/*
    <one line to give the program's name and a brief idea of what it does.>
    J. Irving Vasquez Gomez   ivasquez@ccc.inaoep.mx

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


#include "armkatana.h"

ArmKatana::ArmKatana()
{
    initiated = false;
    verbose = false;
}

ArmKatana::~ArmKatana()
{

}

int ArmKatana::testing()
{
    std::cout << "Hello, world katana arm!" << std::endl;
    return 0;
}

bool ArmKatana::init()
{


    //return vpRobot::init();
    //----------------------------------------------------------------//
    //open device: a serial port is opened in this case
    //----------------------------------------------------------------//

    try {

        port = 5566;

        std::strcpy(ip, "192.168.168.232");
        strcpy(cfAddress, "/home/irving/projects/KNI/configfiles450/katana6M180_G.cfg");

        device.reset(new CCdlSocket(ip, port));

        std::cout << "-------------------------------------------\n";
        std::cout << "success:  port " << port << " open\n";
        std::cout << "-------------------------------------------\n";

        //--------------------------------------------------------//
        //init protocol:
        //--------------------------------------------------------//

        protocol.reset(new CCplSerialCRC());
        std::cout << "-------------------------------------------\n";
        std::cout << "success: protocol class instantiated\n";
        std::cout << "-------------------------------------------\n";
        protocol->init(device.get()); //fails if no response from Katana
        std::cout << "-------------------------------------------\n";
        std::cout << "success: communication with Katana initialized\n";
        std::cout << "-------------------------------------------\n";


        //--------------------------------------------------------//
        //init robot:
        //--------------------------------------------------------//
        katana.reset(new CLMBase());
        katana->create(cfAddress, protocol.get());

    } catch(Exception &e) {
        std::cout << "ERROR: " << e.message() << std::endl;
        return -1;
    }
    std::cout << "-------------------------------------------\n";
    std::cout << "success: katana initialized\n";
    std::cout << "-------------------------------------------\n";

//	ready = true;

    //set protected members
    motorCount = katana->getNumberOfMotors();
    jointCount = motorCount -1;
    gripperId = motorCount -1;

    activated = true;
    gripperEndPoseReached = false;

//	ready = true;
    return true;
}


int ArmKatana::init(const char* host, const char* confFile)
{
    //----------------------------------------------------------------//
    //open device: a ethernet port is opened in this case
    //----------------------------------------------------------------//


    port = 5566;

    std::strcpy(ip, host);
    strcpy(cfAddress, confFile);

    device.reset(new CCdlSocket(ip, port));

    std::cout << "-------------------------------------------\n";
    std::cout << "success:  port " << port << " open\n";
    std::cout << "-------------------------------------------\n";

    //--------------------------------------------------------//
    //init protocol:
    //--------------------------------------------------------//

    protocol.reset(new CCplSerialCRC());
    std::cout << "-------------------------------------------\n";
    std::cout << "success: protocol class instantiated\n";
    std::cout << "-------------------------------------------\n";
    protocol->init(device.get()); //fails if no response from Katana
    std::cout << "-------------------------------------------\n";
    std::cout << "success: communication with Katana initialized\n";
    std::cout << "-------------------------------------------\n";


    //--------------------------------------------------------//
    //init robot:
    //--------------------------------------------------------//
    katana.reset(new CLMBase());
    katana->create(cfAddress, protocol.get());

    std::cout << "-------------------------------------------\n";
    std::cout << "success: katana initialized\n";
    std::cout << "-------------------------------------------\n";

    initiated = true;

    //set protected members
    motorCount = katana->getNumberOfMotors();
    jointCount = motorCount -1;
    gripperId = motorCount -1;

    activated = true;
    gripperEndPoseReached = false;

    return 0;
}

bool ArmKatana::calibrate()
{
    std::cout << std::endl;
    std::cout << "Calibrating Katana... ";
    katana->calibrate();
    std::cout << "finished." << std::endl;

    currentArmPosition = CALIBRATED;
    setVelocity(40);

    return 0;
}

void ArmKatana::kinToK4DRad(const std::vector< double >& kin, std::vector< double >& k4d) const
{
    k4d.resize(kin.size());

    for (uint i=0; i< kin.size(); i++)
    {
        k4d[i] = kinToK4DRad(i+1, (double) kin[i]);
    }
}

double ArmKatana::kinToK4DRad(int joint, const double angle) const
{
    if(joint <1 || joint > jointCount) {
        std::cout << "\n Error: The joint number is not correct";
        return 0;
    }

    switch(joint) {
    case 1:
        return (double) angle + M_PI;
        break;
    case 2:
        return (double) angle;
        break;
    case 3:
        return (double) angle + M_PI;
        break;
    case 4:
        return (double) M_PI/2 - angle;
        break;
    case 5:
        return (double) (3/2)*M_PI - angle;
        break;
    }
    return 0;
}


double ArmKatana::kinToK4DDeg(const int joint, const double angle) const
{
    if(joint <1 || joint > jointCount) {
        std::cout << "\n Error: The joint number is not correct";
        return 0;
    }

    switch (joint)
    {
    case 1:
        return (double) (angle + 180);
    case 2:
        return (double) (angle);
    case 3:
        return (double) (angle + 180);
    case 4:
        return (double) (90 - angle);
    case 5:
        return (double) (270 - angle);
    default:
        return 0;
    }
}

/*
void ArmKatana::kinToK4DDeg(const std::vector< double >& kin, std::vector< double >& k4d) const
{
      k4d.resize(kin.size());

      for (uint i=0; i< kin.size(); i++)
      {
	k4d[i] = kinToK4DDeg(i+1, (double) kin[i]);
      }
}*/

/*
void ArmKatana::convertDegsToRads(std::vector< double >& degrees, std::vector< double >& radians) const
{
  radians.resize(degrees.size());

  for(uint i =0; i<degrees.size(); i++){
    radians[i] = (double) (M_PI * degrees[i]) / 180;
  }
}*/
/*
void ArmKatana::convertJointsRadToEnc(const std::vector< double >& angles, std::vector< int >& encoders) const
{
  // make the encoder vector the same size as the angles vector
  encoders.resize(angles.size());

  CMotBase* mArray = katana->GetBase()->GetMOT()->arr;
  const TMotInit* param = NULL;

  for (size_t i = 0; i < angles.size(); ++i) {
    // get parameters for the specific joint
    param = mArray[i].GetInitialParameters();

    // convert rad value to encoder value
    encoders[i] = ((param->angleOffset - angles[i]) * param->encodersPerCycle * param->rotationDirection) / (2* M_PI) + param->encoderOffset;
  }
}*/
/*
int ArmKatana::convertJointRadToEnc(uint32_t motorId, double jointAngle)
{
  CMotBase* mArray = katana->GetBase()->GetMOT()->arr;
  const TMotInit* param = mArray[motorId].GetInitialParameters();

  // convert rad value to encoder value
  return ((param->angleOffset - jointAngle) * param->encodersPerCycle * param->rotationDirection) / (2 * M_PI) + param->encoderOffset;
}*/
/*
int ArmKatana::executeEncodersIncrementPath(std::list< std::vector< int > > path)
{
  //TODO BUG de alguna forma tienen problemas para leer

  std::list< std::vector< int > >::iterator it;
  std::vector<int>::iterator it_increment;
  short int i;
  int count = 0;
  int steps =0;
  //int stepsCount;

  if (verbose)
    std::cout << "Executing Path with increments on encoders...\n";

  if (path.size() == 0)
    return 0;


  if (path.begin()->size() != motorCount){
    std::cout << "Warning!: Encoders increment do not have the right size.\n";
  }

  steps = path.size();
  cout << "Number of steps:" << steps << endl;

  for(it = path.begin(); it != path.end(); it++){
    i = 0;
    //cout << "a\n";
    for(it_increment = it->begin(); it_increment != it->end(); it_increment ++){

      //cout << "b\n";
      if(*it_increment != 0){
	std::cout << "Movement:" << count << " motor:" << i << " encoder:" << *it_increment << endl;
	count ++;
	katana->moveMotorByEnc(i, *it_increment, false);
	// esperar a que el robot ajuste sus motores
	sleep(1);
	//cout << " done." << endl ;
      }
      i++;
      //cout << "i:" << i << endl;
    }
  }

  if (verbose)
    std::cout << "Done.\n";

  return 1;
} */
/*
void ArmKatana::convertJointsEncToRad(std::vector< double >& angles, const std::vector< int >& encoders) const
{
	// make the angle vector the same size as the encoder vector
	angles.resize(encoders.size());

	CMotBase* mArray = katana->GetBase()->GetMOT()->arr;
	const TMotInit* param = NULL;

	for (size_t i = 0; i < encoders.size(); ++i) {
		// get parameters for the specific joint
		param = mArray[i].GetInitialParameters();

		// convert encoder value to rad value
		angles[i] = param->angleOffset - ((encoders[i] - param->encoderOffset) * 2 * M_PI)
				/ (double) (param->encodersPerCycle * param->rotationDirection);
	}
}*/



double ArmKatana::computePhi(double x, double y)
{
    double resultadoPhi,n1;
    resultadoPhi = atan2(y,x)+(90*(M_PI/180));

    if(resultadoPhi > M_PI) {
        n1 = ((2*M_PI) - (resultadoPhi))*-1;
        return n1;
    }
    else {
        return resultadoPhi;
    }
}

double ArmKatana::computeTheta(double z, double x)
{
    double resultadoTheta,n2;
    resultadoTheta = (90*(M_PI/180))-atan2(z,x);
    return resultadoTheta;
}


void ArmKatana::computeOrientation(double x, double y, double z, double &phi, double &theta)
{
    phi = computePhi(x,y);
    theta = computeTheta(z,x);
}


void ArmKatana::computeApproachingPos(double x, double y, double z, double& xa, double& ya, double& za)
{
    double h,ha,angulo;
    double distancia=100;
    angulo = atan2(y,x);
    h = sqrt(pow(x,2)+pow(y,2));
    ha = h - distancia;
    xa = ha * cos(angulo);
    ya = ha * sin(angulo);
    za = z;
}

/*
void ArmKatana::getHTMPositionAndOrientation(mrpt::poses::CPoint3D &pos, mrpt::poses::CPose3D& orientation)
{
  double x,y,z;
  double phi, theta, psi;
  katana->getCoordinates(x , y , z, phi, theta, psi);
  mrpt::poses::CPose3D rotation;

  //z rotation by phi;
  orientation.setFromValues( 0, 0, 0, phi, 0, 0);

  //x rotation by theta;
  rotation.setFromValues(0,0,0,0,0,theta);
  orientation = rotation + orientation;

  //z rotation by psi;
  rotation.setFromValues(0,0,0,psi,0,0);
  orientation = rotation + orientation;

  pos.m_coords[0] = x;
  pos.m_coords[1] = y;
  pos.m_coords[2] = z;
}*/
/*
void ArmKatana::getHTMPositionAndOrientation(mrpt::poses::CPoint3D& pos, mrpt::poses::CPose3D& orientation, std::vector< int > encoders)
{
  double x,y,z;
  double phi, theta, psi;
  std::vector<double> pose;
  katana->getCoordinatesFromEncoders(pose, encoders);

  x= pose[0];
  y = pose[1];
  z = pose[2];
  phi = pose[3];
  theta = pose[4];
  psi = pose[5];

  mrpt::poses::CPose3D rotation;

  //z rotation by phi;
  orientation.setFromValues( 0, 0, 0, phi, 0, 0);

  //x rotation by theta;
  rotation.setFromValues(0,0,0,0,0,theta);
  orientation = rotation + orientation;

  //z rotation by psi;
  rotation.setFromValues(0,0,0,psi,0,0);
  orientation = rotation + orientation;

  pos.m_coords[0] = x;
  pos.m_coords[1] = y;
  pos.m_coords[2] = z;
}*/
/*
void ArmKatana::getPose3DFromEncoders(mrpt::poses::CPose3D& pose3D, std::vector< int > encoders)
{
  double x,y,z;
  double phi, theta, psi;
  std::vector<double> pose;
  katana->getCoordinatesFromEncoders(pose, encoders);

  x= pose[0];
  y = pose[1];
  z = pose[2];
  phi = pose[3];
  theta = pose[4];
  psi = pose[5];

  mrpt::poses::CPose3D rotation;

  //z rotation by phi;
  pose3D.setFromValues( 0, 0, 0, phi, 0, 0);

  //x rotation by theta;
  rotation.setFromValues(0,0,0,0,0,theta);
  pose3D = rotation + pose3D;

  //z rotation by psi;
  rotation.setFromValues(0,0,0,psi,0,0);
  pose3D = rotation + pose3D;

  //translation;
  rotation.setFromValues(x,y,z,0,0,0);
  pose3D = rotation + pose3D;

}*/


int ArmKatana::getCurrentEncoders(std::vector< int >& encoders)
{
    encoders = katana->getRobotEncoders();
}

int ArmKatana::getCoordinatesFromEncoders(std::vector< int >& encoders, std::vector< double >& coordinates)
{
    katana->getCoordinatesFromEncoders(coordinates, encoders);
    return 0;
}

void ArmKatana::openGripper()
{
    katana->openGripper();
}

void ArmKatana::closerGripper()
{
    katana->closeGripper();
}


void ArmKatana::setVelocity(short int vel)
{
    short velocity = vel;

    //std::cout << "\n\nSet maximum velocity for motors to: \n";
    for(short motorNumber = 0; motorNumber < katana->getNumberOfMotors(); ++motorNumber) {
        //std::cout << motorNumber+1 << ": ";
        //std::cin >> velocity;
        katana->setMotorVelocityLimit(motorNumber, velocity);
    }
}

