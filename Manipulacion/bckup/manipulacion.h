using namespace PlayerCc;
#include <libplayerc++/playerc++.h>


// define a class to do interaction with Player
class Client
{
private:
    // these are the proxies we create to access the devices
    PlayerClient *player;
    Position2dProxy *pp;
    Position3dProxy *pp3;

    struct timeval lastcommand;

public:
    Client(const char* host, int port ); // constructor

    void Read( void ); // get data from Player
    void Update( struct controller* cont ); // send commands to Player
};

// type for a list of pointers to Client objects
typedef std::list<Client*> ClientList;

#define XAXIS 0
#define YAXIS 1
#define XAXIS2 2
#define YAXIS2 3
#define PAN    4
#define TILT   5
#define XAXIS4 6
#define YAXIS4 7

// define the speed limits for the robot

// at full joystick depression you'll go this fast
double max_speed = 0.500; // m/second
double max_turn = DTOR(60); // rad/second
char jsdev[PATH_MAX]; // joystick device file

#define DEFAULT_DEV "/dev/input/js0"

// this is the speed that the camera will pan when you press the
// hatswitches in degrees/sec
/*
#define PAN_SPEED 2
#define TILT_SPEED 2
*/


// normalizing macros
#define AXIS_MAX            32767.0
#define PERCENT(X)          ((X) * 100.0 / AXIS_MAX)
#define PERCENT_POSITIVE(X) (((PERCENT(X)) + 100.0) / 2.0)
#define KNORMALIZE(X)       ((((X) * 1024.0 / AXIS_MAX)+1024.0) / 2.0)
#define NORMALIZE_SPEED(X)  ( (X) * max_speed / AXIS_MAX )
#define NORMALIZE_TURN(X)  ( (X) * max_turn / AXIS_MAX )

struct controller
{
    double speed, turnrate;
    bool dirty; // use this flag to determine when we need to send commands
};


