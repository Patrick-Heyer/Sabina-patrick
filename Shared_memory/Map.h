#ifndef _MAP_H
#define _MAP_H


#include <string>
using namespace std;

///*! \brief  Information regarding the image used as map
// *
// *  
// */
class Map {
  private:
    ///*! \brief File path of the image used as a map
    // *
    // *  
    // */
    string * Image_path;

    ///*! \brief Height in pixels of the image used as map
    // *
    // *  
    // */
    int Height_px;

    ///*! \brief Width in pixels of the image used as map
    // *
    // *  
    // */
    int Width_px;

    ///*! \brief Height in meters of the image used as map
    // *
    // *  
    // */
    float Height_m;

    ///*! \brief Width in meters of the image used as map
    // *
    // *  
    // */
    float Width_m;

    ///*! \brief  Relation of pixels to meters of the image on the X axis
    // *
    // *  
    // */
    float Ratio_x;

    ///*! \brief  Relation of pixels to meters of the image on the Y axis
    // *
    // *  
    // */
    float Ratio_y;


  public:
    ///*! \brief Gets the height of the image in pixels
    // *
    // *   Gets the height of the image in pixels \return Height_px
    // */
    int getHeightPx();

    ///*! \brief Sets the height of the image in pixels
    // *
    // *   Sets the height of the image in pixels
    // */
    void set_Height_px(int value);

    ///*! \brief Gets the width of the image in pixels
    // *
    // *   Gets the width of the image in pixels \return Width_px
    // */
    int getWidthPx();

    ///*! \brief Sets the width of the image in pixels
    // *
    // *   Sets the width of the image in pixels
    // */
    void set_Width_px(int value);

    ///*! \brief Gets the height of the image in meters
    // *
    // *   Gets the height of the image in meters \return Height_m
    // */
    float get_Height_m();

    ///*! \brief Sets the height of the image in meters
    // *
    // *   Sets the height of the image in meters
    // */
    void set_Height_m(float value);

    ///*! \brief Gets the width of the image in meters
    // *
    // *   Gets the width of the image in meters \return Width_m
    // */
    float get_Width_m();

    ///*! \brief Sets the width of the image in meters
    // *
    // *   Sets the width of the image in meters
    // */
    void set_Width_m(float value);

    ///*! \brief Gets the ratio in meters/pixels of the image on the X axis
    // *
    // *   Gets the ratio in meters/pixels of the image on the X axis \return Ratio_x
    // */
    float get_Ratio_x();

    ///*! \brief Sets the ratio in meters/pixels of the image on the X axis
    // *
    // *   Sets the ratio in meters/pixels of the image on the X axis
    // */
    void set_Ratio_x(float value);

    ///*! \brief Gets the ratio in meters/pixels of the image on the Y axis
    // *
    // *   Gets the ratio in meters/pixels of the image on the Y axis \return Ratio_y
    // */
    float get_Ratio_y();

    ///*! \brief Sets the ratio in meters/pixels of the image on the Y axis
    // *
    // *   Sets the ratio in meters/pixels of the image on the Y axis
    // */
    void set_Ratio_y(float value);

    ///*! \brief Gets the map image file name
    // *
    // *   Gets the map image file name \return Image_path
    // */
    string get_Image_path();

    ///*! \brief Sets the map image file name
    // *
    // *   Sets the map image file name
    // */
    void set_Image_path(string value);

  friend class Robot;
};
#endif
