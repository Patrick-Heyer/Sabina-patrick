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

/**
 * Encapsulates the communication with the arm katana 6M180
 */

#ifndef ARMKATANA_H
#define ARMKATANA_H

#include <iostream>
#include <cstdio>
#include <memory>
#include <vector>
#include <fstream>
#include <math.h>
#include <list>

#include <kniBase.h>

//#include <boost/numeric/ublas/matrix.hpp>
//#include <boost/numeric/ublas/io.hpp>

using namespace std;

enum  ArmPositions {CARRYING, STORED, HANGING, GRASPING, CALIBRATED, UNKNOWN};

class ArmKatana
{

public:
    ArmKatana();
    virtual ~ArmKatana();

    int testing();

    int init(const char* host, const char* confFile);

    virtual bool init();

    bool calibrate();

    void openGripper();

    void closerGripper();

    int getCurrentEncoders(std::vector<int> &encoders);

    int getCoordinatesFromEncoders(std::vector<int> &encoders, std::vector<double> &coordinates);

    void setVelocity(short int vel);

protected:
    std::auto_ptr<CLMBase> katana;

    /**
    * Number of motors (inclusive gripper)
    */
    int motorCount;

    /**
    * Number of joints (exclusive gripper)
    */
    int jointCount;

    /**
    * Id of the gripper
    */
    int gripperId;

    /**
    * Timeout in [ms] which should maximal be
    * wait for closing the gripper
    */
    int gripperTimeout;

    /**
    * Indicates if the Katana can be used (true).
    * If an exception like a collision happened it is set to false.
    */
    bool activated;

    /**
    * True if the Gripper has moved to his endpose.
    */
    bool gripperEndPoseReached;


    /**
    * Converts an angle in kinematics convention to K4D convention
    * @param joint joint number [1-n]
    * @param angle radians value
    */
    double kinToK4DRad(const int joint, const double angle) const;

    /**
    * Converts a vector of kin angles to a vector of K4D Angles.
    * @param kin Vector of joint angles which should be converted [rad]
    * @param k4d of converted joint angles
    */
    void kinToK4DRad(const std::vector<double> &kin, std::vector<double> &k4d) const;

    /**
    * Converts an angle in kinematics convention to K4D convention
    * @param joint joint number [1-n]
    * @param angle degrees value
    */
    double kinToK4DDeg(const int joint, const double angle) const;

    /**
    * Converts a vector of kin angles to a vector of K4D Angles.
    * @param kin Vector of joint angles which should be converted [rad]
    * @param k4d of converted joint angles
    */
    void kinToK4DDeg(const std::vector<double> &kin, std::vector<double> &k4d) const;


    void convertDegsToRads(std::vector<double> &degrees, std::vector<double> &radians) const;

    /**
    * Converts a vector of joint angles to a vector of joint encoders.
    * @param angles Vector of joint angles which should be converted [rad]
    * @param encoders Vector of joint encoders
    */
    void convertJointsRadToEnc(const std::vector<double> &angles, std::vector<int>& encoders) const;

    /**
    * Converts a joint angle value to a joint encoder value.
    * @param motorId Id of the joint
    * @param jointAngle Angle of the joint [rad]
    * @return Encoder value of the joint
    */
    int convertJointRadToEnc(uint32_t motorId, double jointAngle);

    /**
    * Convert a vector of joint encoders to a vector of joint angles.
    * @param angles Vector of joint angles [rad]
    * @param encoders Vector of joint encoders which should be converted
    */
    void convertJointsEncToRad(std::vector<double> &angles, const std::vector<int>& encoders) const;


    /**
    * Computes Eulers' phi parameter for a given position over the x-y plane.
    * @param x mm
    * @param y mm
    */
    double computePhi(double x, double y);

    /**
    * Computes Euler's theta parameter for a given posicion over the plane x-z
    */
    double computeTheta(double z, double x);

    /**
     * Computes Euler's phi and theta parameters for a given 3D point.
     * @param x
     * @param y
     * @param z
     * @param return value phi
     * @param return value theta
     */
    void computeOrientation(double x, double y, double z, double &phi, double &theta);

    /**
     * Computes the approaching position for graspObjectAt();
     */
    void computeApproachingPos(double x,double y, double z, double &xa, double &ya, double &za);

    int currentArmPosition;

private:
    //Katana obj.
    bool initiated;

    bool verbose;

    std::auto_ptr<CCdlSocket> device;

    std::auto_ptr<CCplSerialCRC> protocol;

    int port;
    char ip[16];
    char cfAddress[100];

};

#endif // ARMKATANA_H
