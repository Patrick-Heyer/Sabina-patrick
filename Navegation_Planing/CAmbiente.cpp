#include <stdlib.h>
#include <string.h>


#include "CPersonajeConfig.h"

#include "CAmbiente.h"

#include <iostream>

using namespace std;

CAmbiente::CAmbiente()
{

    Workspace_p_m = new CWorkspace;
    sizeRobot_m = 10;

    width = 0;
    height = 0;
    color_type = 0;
    bit_depth = 0;
    number_of_passes = 0;
    row_pointers = NULL;

}

CAmbiente::~CAmbiente()
{

    if ( Workspace_p_m != NULL )
        delete Workspace_p_m;

    if ( row_pointers != NULL )
    {

        for (int y=0; y<height; y++)
            free(row_pointers[y]);

        free (row_pointers);
        row_pointers = NULL;

    }

}



void CAmbiente::CalcularWorkspace()
{
    float mini[3],maxi[3];


    mini[0] = 0;
    mini[1] = 0;
    mini[2] = 0;
    maxi[0] = (float)width;
    maxi[1] = (float)height;
    maxi[2] = 0;

    if ( Workspace_p_m == NULL)
        Workspace_p_m = new CWorkspace;

    Workspace_p_m->PonerMinMax((double)mini[0],(double)mini[1],(double)mini[2],
                               (double)maxi[0],(double)maxi[1],(double)maxi[2] );

}


const CWorkspace * CAmbiente::Workspace() const
{
    return (Workspace_p_m);
}


int CAmbiente::setMapaFileName(std::string *file_a)
{
    char header[8];	// 8 is the maximum size that can be checked

    if (file_a != NULL)
    {


        /* open file and test for it being a png */
        FILE *fp = fopen(file_a->c_str(), "rb");
        if (!fp) {
            cout<<"[read_png_file] File could not be opened for reading -  " << file_a->c_str() <<endl;
            return -1;
        }
        fread(header, 1, 8, fp);
        if (png_sig_cmp((png_byte*)header, 0, 8)) {
            cout<<"[read_png_file] File is not recognized as a PNG file -  "<< file_a->c_str()<<endl;
            return -1;
        }

        /* initialize stuff */
        png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

        if (!png_ptr) {
            cout<<"[read_png_file] png_create_read_struct failed"<<endl;
            return -1;
        }
        info_ptr = png_create_info_struct(png_ptr);

        if (!info_ptr) {
            cout<<"[read_png_file] png_create_info_struct failed"<<endl;
            return -1;
        }
        if (setjmp(png_jmpbuf(png_ptr))) {
            cout<<"[read_png_file] Error during init_io"<<endl;
            return -1;
        }
        png_init_io(png_ptr, fp);
        png_set_sig_bytes(png_ptr, 8);

        png_read_info(png_ptr, info_ptr);

        width = png_get_image_width(png_ptr, info_ptr);
        height = png_get_image_height(png_ptr, info_ptr);
        color_type = png_get_color_type(png_ptr, info_ptr);
        bit_depth = png_get_bit_depth(png_ptr, info_ptr);

        number_of_passes = png_set_interlace_handling(png_ptr);
        png_read_update_info(png_ptr, info_ptr);


        /* read file */
        if (setjmp(png_jmpbuf(png_ptr))) {
            cout<<"[read_png_file] Error during read_image"<<endl;
            return -1;
        }
        row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
        for (int y=0; y<height; y++)
            row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(png_ptr, info_ptr));

        png_read_image(png_ptr, row_pointers);

        fclose(fp);
        CalcularWorkspace();

//dump image
        /*	      for (int y=0; y<height; y++)
        		{
         	  	  png_byte* row = row_pointers[y];
        		  for (int x=0; x<width; x++)
        			printf("Pixel at position [ %d - %d ] has the following RGBA values: %d \n", x , y, row[x]);
        		}
        */
    }
    return 0;
}


void CAmbiente::setsizeRobot(int size_a)
{
    sizeRobot_m = size_a;
}


bool CAmbiente::Colision( const CPersonajeConfig &config) const
{
    int x,y,sizeR, i,j;
    png_byte* row  =  NULL ;


    x = (int)config.Position().X();
    y = (int)config.Position().Y();
    sizeR = (int)sizeRobot_m/2;
    if ( (row_pointers != NULL) && (Workspace_p_m != NULL))
        for(j = y - sizeR; j < y + sizeR; j++)
        {
            if ( (j < height) && (j>=0) )
            {
                row = row_pointers[j];
                for(i= x - sizeR ; i< x + sizeR ; i++)
                {
                    if ( (i<width) && (i>=0) )
                    {
                        if ( !row[i] )
                        {
                            //cout<<"colision:  "<< i << "  "<< j <<endl;
                            return 1;
                        }

                    }
                }
            }
        }

    return 0 ;
}



void CAmbiente::Limpiar()
{
//	if (Robot_p_m!= NULL)
//		Robot_p_m->Limpiar();
}
