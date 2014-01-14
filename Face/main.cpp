#include "model_3ds.h"
#include "vector3d.h"

#include "Md3Model.h"
#include "Texture_Manager.h"

#include "Face.h"
#include "../Shared_memory/Robot.h"


using namespace std;

int argc_global;
char **argv_global;


IplImage *frame = 0;
double headX,headY;

void cvInit ();
void cvFinish();

static CvMemStorage* storage = 0;
static CvHaarClassifierCascade* cascade = 0;

//this is an approximation, for FOV = 45 degrees, 640x480 pixels
double horizontalGradesPerPixel = 53.0/640.0;
double verticalGradesPerPixel = 40.0/480.0;
double headWidth = 0.12; //supposing head's width is 12 cm


Md3Model *head;

GLint head_tex;

Robot *patrolbot;

struct Image
{
	unsigned long sizeX;
	unsigned long sizeY;
	char *data;
};
typedef struct Image Image;

void detect_and_draw ( IplImage* image );

const char* cascade_name =
"../data/haarcascade_frontalface_default.xml";
/*    "haarcascade_profileface.xml";*/

void detect_and_draw ( IplImage* img )
{
	static CvScalar colors[] =
	{
		{{0,0,255}},
		{{0,128,255}},
		{{0,255,255}},
		{{0,255,0}},
		{{255,128,0}},
		{{255,255,0}},
		{{255,0,0}},
		{{255,0,255}}
	};
	
	double scale = 2.0;
	IplImage* gray = cvCreateImage ( cvSize ( img->width,img->height ), 8, 1 );
	IplImage* small_img = cvCreateImage ( cvSize ( cvRound ( img->width/scale ), cvRound ( img->height/scale ) ), 8, 1 );
	int i;
	
	cvCvtColor ( img, gray, CV_BGR2GRAY );
	cvResize ( gray, small_img, CV_INTER_LINEAR );
	cvEqualizeHist ( small_img, small_img );
	cvClearMemStorage ( storage );
	
	if ( cascade )
	{
		double t = ( double ) cvGetTickCount();
		CvSeq* faces = cvHaarDetectObjects ( small_img, cascade, storage,
											 1.2, 2, CV_HAAR_FIND_BIGGEST_OBJECT,
									   cvSize ( 40, 40 ) );
		t = ( double ) cvGetTickCount() - t;
		for ( i = 0; i < ( faces ? faces->total : 0 ); i++ )
		{
			CvRect* r = ( CvRect* ) cvGetSeqElem ( faces, i );
			CvPoint center;
			
			center.x = cvRound ( ( r->x + r->width*0.5 ) *scale );
			center.y = cvRound ( ( r->y + r->height*0.5 ) *scale );
			
			headX = center.x;
			headY = center.y;
			
			cvCircle ( img, center, 1, colors[i%8], 3, 8, 0 );
		}
	}
	
	cvReleaseImage ( &gray );
	cvReleaseImage ( &small_img );
}


void cvInit ()
{
	cascade_name = "../data/haarcascade_frontalface_default.xml";
	cascade = ( CvHaarClassifierCascade* ) cvLoad ( cascade_name, 0, 0, 0 );
	if ( !cascade )
	{
		fprintf ( stderr, "ERROR: Could not load classifier cascade\n" );
		fprintf ( stderr, "Usage: facedetect --cascade=\"<cascade_path>\" [filename|camera_index]\n" );
		exit(1);
	}
	storage = cvCreateMemStorage ( 0 );
}

void initFace()
{
	CTextureManager::getInstance()->Initialize();
	cvInit ( );
	head=new Md3Model("../data/Face/head.md3");
	head_tex=LoadTexture("../data/Face/face.tga");
	
}

void DrawFace ( void )
{
	frame=patrolbot->getInstance().Ptz.get_image();
	cvFlip ( frame, frame,1 );
	detect_and_draw ( frame );
	
	glLoadIdentity();
	glPushMatrix();
	glTranslatef(0.0f,-2.0f,-15.0f);
		
	glPushMatrix();
	Vector3D n, up;
	n=Vector3D (( headX-320 ) /500, ( -1* ( headY-240 ) /500 ) -.1,2  );
	up=Vector3D ( 0,1,0 );
	up.Normalize();
	n.Normalize();
	Vector3D u = up.Cross ( up,n );
	u.Normalize();
	Vector3D v = n.Cross ( n,u );
	v.Normalize();
	
	//create the viewmatrix
	double curmat[16];
	
	curmat[0]=u.X;
	curmat[1]=u.Y;
	curmat[2]=u.Z;
	curmat[3]=0;
	
	curmat[4]=v.X;
	curmat[5]=v.Y;
	curmat[6]=v.Z;
	curmat[7]=0;
	
	curmat[8]=n.X;
	curmat[9]=n.Y;
	curmat[10]=n.Z;
	curmat[11]=0;
	
	curmat[12]=0;
	curmat[13]=0;
	curmat[14]=0;
	curmat[15]=1;
	glMultMatrixd ( curmat );
	
	glColor3f(1,1,1);
	
	glEnable ( GL_TEXTURE_2D );
	glPolygonMode ( GL_FRONT, GL_FILL );
	glBindTexture(GL_TEXTURE_2D, head_tex);
	head->renderFrameImmediate(0);
	
	
	glPopMatrix();
	glPopMatrix();
	
	cvReleaseImage(&frame);
}

