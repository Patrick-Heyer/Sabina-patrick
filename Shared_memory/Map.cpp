
#include "Map.h"

bool Image_path_escribible=true;
bool Height_px_escribible=true;
bool Width_px_escribible=true;
bool Height_m_escribible=true;
bool Width_m_escribible=true;
bool Ratio_x_escribible=true;
bool Ratio_y_escribible=true;

///*! \brief Gets the height of the image in pixels
// *
// *   Gets the height of the image in pixels \return Height_px
// */
int Map::getHeightPx() {
    return Height_px;
}

///*! \brief Sets the height of the image in pixels
// *
// *   Sets the height of the image in pixels
// */
void Map::set_Height_px(int value) {
    {
    } while (!Height_px_escribible);
    Height_px_escribible=false;
    Height_px = value;
    Height_px_escribible=true;
}

///*! \brief Gets the width of the image in pixels
// *
// *   Gets the width of the image in pixels \return Width_px
// */
int Map::getWidthPx() {
    return Width_px;
}

///*! \brief Sets the width of the image in pixels
// *
// *   Sets the width of the image in pixels
// */
void Map::set_Width_px(int value) {
    do
    {
    } while (!Width_px_escribible);
    Width_px_escribible=false;
    Width_px = value;
    Width_px_escribible=true;
}

///*! \brief Gets the height of the image in meters
// *
// *   Gets the height of the image in meters \return Height_m
// */
float Map::get_Height_m() {
    return Height_m;
}

///*! \brief Sets the height of the image in meters
// *
// *   Sets the height of the image in meters
// */
void Map::set_Height_m(float value) {
    do
    {
    } while (!Height_m_escribible);
    Height_m_escribible=false;
    Height_m = value;
    Height_m_escribible=true;
}

///*! \brief Gets the width of the image in meters
// *
// *   Gets the width of the image in meters \return Width_m
// */
float Map::get_Width_m() {
    return Width_m;
}

///*! \brief Sets the width of the image in meters
// *
// *   Sets the width of the image in meters
// */
void Map::set_Width_m(float value) {
    do
    {
    } while (!Width_m_escribible);
    Width_m_escribible=false;
    Width_m = value;
    Width_m_escribible=true;
}

///*! \brief Gets the ratio in meters/pixels of the image on the X axis
// *
// *   Gets the ratio in meters/pixels of the image on the X axis \return Ratio_x
// */
float Map::get_Ratio_x() {
    return Ratio_x;
}

///*! \brief Sets the ratio in meters/pixels of the image on the X axis
// *
// *   Sets the ratio in meters/pixels of the image on the X axis
// */
void Map::set_Ratio_x(float value) {
    do
    {
    } while (!Ratio_x_escribible);
    Ratio_x_escribible=false;
    Ratio_x = value;
    Ratio_x_escribible=true;
}

///*! \brief Gets the ratio in meters/pixels of the image on the Y axis
// *
// *   Gets the ratio in meters/pixels of the image on the Y axis \return Ratio_y
// */
float Map::get_Ratio_y() {
    return Ratio_y;
}

///*! \brief Sets the ratio in meters/pixels of the image on the Y axis
// *
// *   Sets the ratio in meters/pixels of the image on the Y axis
// */
void Map::set_Ratio_y(float value) {
    do
    {
    } while (!Ratio_y_escribible);
    Ratio_y_escribible=false;
    Ratio_y = value;
    Ratio_y_escribible=true;
}

///*! \brief Gets the map image file name
// *
// *   Gets the map image file name \return Image_path
// */
string Map::get_Image_path() {
    return *Image_path;
}

///*! \brief Sets the map image file name
// *
// *   Sets the map image file name
// */
void Map::set_Image_path(string value) {
    do
    {
    } while (!Image_path_escribible);
    Image_path_escribible=false;
    *Image_path = value;
    Image_path_escribible=true;
}

