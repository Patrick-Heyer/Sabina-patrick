

#ifndef CAmbienteH
#define CAmbienteH

#include <png.h>


#include <vector>
#include <string>

#include "Workspace.h"

	
class CPersonajeConfig;
	
class CAmbiente
{

  private:
	  int sizeRobot_m;
	  CWorkspace * Workspace_p_m;

	  int width, height;
	  png_byte color_type;
	  png_byte bit_depth;

	  png_structp png_ptr;
	  png_infop info_ptr;
	  int number_of_passes;
	  png_bytep * row_pointers;

  	
  public:
      CAmbiente();
      ~CAmbiente();

  	  void setsizeRobot(int);
      int setMapaFileName(std::string *);
  	  void CalcularWorkspace();
      const CWorkspace * Workspace() const ;
      bool Colision( const CPersonajeConfig &) const ;
  	  int sizeRobot(){return sizeRobot_m;}
	  void Limpiar();
  		
    
};


#endif
