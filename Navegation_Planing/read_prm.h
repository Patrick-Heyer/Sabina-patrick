#include "CProbot.h"
#include <list>
#include "../Shared_memory/Location.h"
/*! \file
 *
 */
/* Longitud de maxima de las variables y sus valores, 50 caracteres */
#define MAX_STRING_LENGTH 50
/*! \fn void read_prm(CProbot *prm_p_a, int x_initial, int y_initial, int x_final, int y_final)
 * \brief Calculates a route
 *
 * from x_initial, y_initial to x_final, y_final using prm_p_a parameters
 *
 * \param prm_p_a The CProbot object that contains the parameters for the planer.
 * \param x_initial The x cordinate of the start position
 * \param y_initial The y cordinate of the start position
 * \param x_final The x cordinate of the end position
 * \param y_final The y cordinate of the end position
 */
list<Location> read_prm(CProbot *prm_p_a, int x_initial, int y_initial, int x_final, int y_final);
