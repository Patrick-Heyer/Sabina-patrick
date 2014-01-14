/*! \file
 *
 */

int      initFaceDet(const char * haarCascadePath);
int      initEyeDet(const char * haarCascadePath);
int      initEyeDetI(const char * haarCascadePath);
int      initEyeDetD(const char * haarCascadePath);
void     closeFaceDet();
void     closeEyeDet();
void     closeEyesDet();

/*! \fn CvRect * detectFace(IplImage * pImg, int *how_many)
 * \brief Detects human faces in a image a route
 *
 * \param pImg Image to proccess
 * \param how_many number of faces detected
 * \returns CvRect surounding the first fave
 */
CvRect * detectFace(IplImage * pImg, int *how_many);
IplImage* detectEyes(IplImage * pImg,int pt[],int apun,int *detecte);
int detecteOjos(IplImage * pImg);
