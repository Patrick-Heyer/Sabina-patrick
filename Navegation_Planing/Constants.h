
#if !defined(CONSTANTS)
#define CONSTANTS 1

/*** class ***
name :	   PRM
derived :  
author :   Valerie Boor
date :     08/30/1999
see also :
description :
  consists of an enumeration of the
  parameters used for motion planning.
*** end of class ***/
class PRM
{
  public:

  enum
  {
    TRY_NOT_ALL        = 0,             // Determines whether it should be tried not to connect to all...
    TRY_ALL            = 1,             // ...or all

    RUN_NODES          = 0x00000000,	// PPP_Runner runs a specified Number of Samples
    RUN_TIME           = 0x00000001,	// PPP_Runner runs for a specified Period
    RUN_CONNECT        = 0x00000010,	// PPP_Runner runs until start and goal are connected
    /*
    RANDOM_S            = 0x00000000,	// Use RandomSampling method
    GAUSSIAN_S          = 0x00000001,	// Use resampling with gaussian distribution
    GAUSSIAN_UNIFORM_S  = 0x00000010,
    GAUSSIAN_RESTATIC_S = 0x00000011,
    VERTEX_S            = 0x00000100,    // Use the Vertex sampling method
    GAUSSIAN_TRIPLE_S   = 0x00000101,	// Use triple resampling with gaussian distribution
    GAUSSIAN_CONCAVE_S  = 0x00000110,    // Use the Gaussian Concave sampling method
    */
    RANDOM_S            = 0,	// Use RandomSampling method
    GAUSSIAN_S          = 1,	// Use resampling with gaussian distribution
    GAUSSIAN_UNIFORM_S  = 2,
    GAUSSIAN_RESTATIC_S = 3,
    VERTEX_S            = 4,    // Use the Vertex sampling method
    DANGERZONES_S       = 5,    // Use the DangerZone sampling method

    LP_SMOOTHING       = 0x00000000,   // Standard smoothing method
    PROJ_SMOOTHING     = 0x00000001,   // Projective smoothing method

    STRAIGHT_LP        = 0,	// Use StraigtLinePlanner LocalPlanner
    POTFIELD_LP        = 1,	// Use PotentialFieldPlanner
    TRANSLATE_LP       = 2,	// Use TranslateOnly LocalPlanner
    DANGERZONE_LP      = 3,     // use the dangerzone localplanner

    FREE_FLYING        = 0x11111110,	// Configuration type XYZQ
    CAR_LIKE           = 0x11000010,	// Configuration type XYtheta
    CAR_LIKISH         = 0x11100010,	// Configuration type XYZtheta
    TRANSLATING        = 0x11100000,	// Configuration type XYZ
    ARTICULATED        = 0x00000001,	// Configuration type articulated
    // Usage: if ( conftype == (PRM::CAR_LIKE | PRM::ARTICULATED) )

    NR_OF_TYPES_OF_SAMPLES = 6 , 

    NR_OF_COUNTERS     = 11,
    FREE               = 0,             // Index in uu_PPPdata.Samples and Counters vectors
    FAILED             = 1,             // " "
    MIXEDFREE          = 2,
    MIXEDFAILED        = 3,
    PAIRFREE           = 4,
    PAIRFAILED         = 5,
    SAMPLES            = 6,             // " "
    RESAMPLES          = 7,             // " "
    NR_SAMPLES         = 8,             // Index in uu_PPPdata.Counters vector, see: RobotConfig
    BEGIN_NR_NODES     = 9,              // Index in uu_PPPdata.Counters vector, see: RobotConfig
    TEST               = 10             // " "
  } ;
} ;

#endif
