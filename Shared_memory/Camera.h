#ifndef _CAMERA_H
#define _CAMERA_H


///*! \brief Information about the processed information of a camara
// *
// *  
// */
class Camera {
  private:
    ///*! \brief The X coordinate of an object in the image
    // *
    // *  
    // */
    int Focus_x;

    ///*! \brief The Y coordinate of an object in the image
    // *
    // *  
    // */
    int Focus_y;

    ///*! \brief The unique identifier of an object
    // *
    // *  
    // */
    int Visual_ID;

    ///*! \brief The height in pixels of the image
    // *
    // *  
    // */
    int Image_height;

    ///*! \brief The width in pixels of the image
    // *
    // *  
    // */
    int Image_width;


  public:
    ///*! \brief Gets the X coordinate of an object
    // *
    // *  Gets the X coordinate of the object of interest in the image captured by the camera
    // *  \return Focus_x
    // */
    int get_Focus_x();

    ///*! \brief Sets the X coordinate of an object
    // *
    // *  Sets the X coordinate of the object of interest in the image captured by the camera
    // */
    void set_Focus_x(int value);

    ///*! \brief Gets the Y coordinate of an object
    // *
    // *  Gets the Y coordinate of the object of interest in the image captured by the camera
    // *  \return Focus_y
    // */
    int get_Focus_y();

    ///*! \brief Sets the Y coordinate of an object
    // *
    // *  Sets the Y coordinate of the object of interest in the image captured by the camera
    // */
    void set_Focus_y(int value);

    ///*! \brief Gets the Visual_ID of an object
    // *
    // *  Gets the Visual_ID of the object of interest in the image captured by the camera
    // *  \return Visual_ID
    // */
    int get_Visual_ID();

    ///*! \brief Sets the Visual_ID of an object
    // *
    // *  Sets the Visual_ID of the object of interest in the image captured by the camera
    // */
    void set_Visual_ID(int value);

    ///*! \brief Gets the height in pixels of an image
    // *
    // *  Gets the height of the image captured by the camera  \return Image_height
    // */
    int get_Image_height();

    ///*! \brief Sets the height in pixels of an image
    // *
    // *  Sets the height of the image captured by the camera
    // */
    void set_Image_height(int value);

    ///*! \brief Gets the width in pixels of an image
    // *
    // *  Gets the width of the image captured by the camera  \return Image_width
    // */
    int get_Image_width();

    ///*! \brief Sets the width in pixels of an image
    // *
    // *  Sets the width of the image captured by the camera
    // */
    void set_Image_width(int value);

  friend class Robot;
};
#endif
