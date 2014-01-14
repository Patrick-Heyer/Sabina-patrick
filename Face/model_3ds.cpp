#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <GL/glut.h>

#include "model_3ds.h"
#include "Texture_Manager.h"

long filelength(int f)
{
    struct stat buf;

    fstat(f, &buf);

    return(buf.st_size);
};

void Model_3ds::LoadModel(const char * filename)
{
    int i=0; //Index variable

    FILE *l_file; //File pointer

    unsigned short l_chunk_id=0; //Chunk identifier
    unsigned int l_chunk_lenght=0; //Chunk lenght

    unsigned char l_char=0; //Char variable
    unsigned short l_qty=0; //Number of elements in each chunk

    unsigned short l_face_flags=0; //Flag that stores some face information

    l_file=fopen (filename, "rb"); //Open the file
    if (l_file == NULL)
    {
        printf("no sucsddsdsddsdh file");
    }

    while (ftell (l_file) < filelength (fileno (l_file))) //Loop to scan the whole file
        //while(!EOF)
    {
        //getch(); //Insert this command for debug (to wait for keypress for each chuck reading)

        fread (&l_chunk_id, 2, 1, l_file); //Read the chunk header
        //printf("ChunkID: %x\n",l_chunk_id);
        fread (&l_chunk_lenght, 4, 1, l_file); //Read the lenght of the chunk
        //printf("ChunkLenght: %x\n",l_chunk_lenght);

        switch (l_chunk_id)
        {
            //----------------- MAIN3DS -----------------
            // Description: Main chunk, contains all the other chunks
            // Chunk ID: 4d4d
            // Chunk Lenght: 0 + sub chunks
            //-------------------------------------------
        case 0x4d4d:
            break;

            //----------------- EDIT3DS -----------------
            // Description: 3D Editor chunk, objects layout info
            // Chunk ID: 3d3d (hex)
            // Chunk Lenght: 0 + sub chunks
            //-------------------------------------------
        case 0x3d3d:
            break;

            //--------------- EDIT_OBJECT ---------------
            // Description: Object block, info for each object
            // Chunk ID: 4000 (hex)
            // Chunk Lenght: len(object name) + sub chunks
            //-------------------------------------------
        case 0x4000:
            i=0;
            do
            {
                fread (&l_char, 1, 1, l_file);
                name[i]=l_char;
                i++;
            } while (l_char != '\0' && i<20);
            break;

            //--------------- OBJ_TRIMESH ---------------
            // Description: Triangular mesh, contains chunks for 3d mesh info
            // Chunk ID: 4100 (hex)
            // Chunk Lenght: 0 + sub chunks
            //-------------------------------------------
        case 0x4100:
            break;

            //--------------- TRI_VERTEXL ---------------
            // Description: Vertices list
            // Chunk ID: 4110 (hex)
            // Chunk Lenght: 1 x unsigned short (number of vertices)
            //             + 3 x float (vertex coordinates) x (number of vertices)
            //             + sub chunks
            //-------------------------------------------
        case 0x4110:
            fread (&l_qty, sizeof (unsigned short), 1, l_file);
            this->vertices_qty = l_qty;
            //printf("Number of vertices: %d\n",l_qty);
            this->vertex=new vertex_type[this->vertices_qty];
            for (i=0; i<l_qty; i++)
            {
                fread (&this->vertex[i].x, sizeof(float), 1, l_file);
                //printf("Vertices list x: %f\n",vertex[i].x);
                fread (&this->vertex[i].y, sizeof(float), 1, l_file);
                //printf("Vertices list y: %f\n",vertex[i].y);
                fread (&this->vertex[i].z, sizeof(float), 1, l_file);
                //printf("Vertices list z: %f\n",vertex[i].z);
            }
            break;

            //--------------- TRI_FACEL1 ----------------
            // Description: Polygons (faces) list
            // Chunk ID: 4120 (hex)
            // Chunk Lenght: 1 x unsigned short (number of polygons)
            //             + 3 x unsigned short (polygon points) x (number of polygons)
            //             + sub chunks
            //-------------------------------------------
        case 0x4120:
            fread (&l_qty, sizeof (unsigned short), 1, l_file);
            this->polygons_qty = l_qty;
            //printf("Number of polygons: %d\n",l_qty);
            this->polygon=new polygon_type[this->polygons_qty];
            for (i=0; i<l_qty; i++)
            {
                fread (&this->polygon[i].a, sizeof (unsigned short), 1, l_file);
                //printf("Polygon point a: %d\n",polygon[i].a);
                fread (&this->polygon[i].b, sizeof (unsigned short), 1, l_file);
                //printf("Polygon point b: %d\n",polygon[i].b);
                fread (&this->polygon[i].c, sizeof (unsigned short), 1, l_file);
                //printf("Polygon point c: %d\n",polygon[i].c);
                fread (&l_face_flags, sizeof (unsigned short), 1, l_file);
                //printf("Face flags: %x\n",l_face_flags);
            }
            break;

            //------------- TRI_MAPPINGCOORS ------------
            // Description: Vertices list
            // Chunk ID: 4140 (hex)
            // Chunk Lenght: 1 x unsigned short (number of mapping points)
            //             + 2 x float (mapping coordinates) x (number of mapping points)
            //             + sub chunks
            //-------------------------------------------
        case 0x4140:
            fread (&l_qty, sizeof (unsigned short), 1, l_file);
            this->mapcoord= new mapcoord_type[l_qty];

            for (i=0; i<l_qty; i++)
            {
                fread (&this->mapcoord[i].u, sizeof (float), 1, l_file);
                //printf("Mapping list u: %f\n",mapcoord[i].u);
                fread (&this->mapcoord[i].v, sizeof (float), 1, l_file);
                //printf("Mapping list v: %f\n",mapcoord[i].v);
            }
            break;

            //----------- Skip unknow chunks ------------
            //We need to skip all the chunks that currently we don't use
            //We use the chunk lenght information to set the file pointer
            //to the same level next chunk
            //-------------------------------------------
        default:
            fseek(l_file, l_chunk_lenght-6, SEEK_CUR);
        }
    }

    //fclose (l_file); // Closes the file stream

}

Model_3ds::Model_3ds()
{

}

void Model_3ds::SetTexture(const char* filename)
{
    this->Texture=LoadTexture(filename);
}


Model_3ds::~Model_3ds()
{
    delete this->vertex;

    delete this->polygon;

    delete this->mapcoord;
    CTextureManager::getInstance()->DeleteTexture(this->Texture);
}


void Model_3ds::Render()
{
    glEnable(GL_TEXTURE_2D);

    glColor4f(1,1,1,1);

    glBindTexture(GL_TEXTURE_2D, this->Texture);
    glBegin(GL_TRIANGLES); // glBegin and glEnd delimit the vertices that define a primitive (in our case triangles)
    for (int i=0; i<this->polygons_qty; i++)
    {
        //----------------- FIRST VERTEX -----------------
        // Texture coordinates of the first vertex
        glTexCoord2f(this->mapcoord[ this->polygon[i].a ].u,
                     this->mapcoord[ this->polygon[i].a ].v);
        // Coordinates of the first vertex
        glVertex3f( this->vertex[ this->polygon[i].a ].x,
                    this->vertex[ this->polygon[i].a ].y,
                    this->vertex[ this->polygon[i].a ].z); //Vertex definition

        //----------------- SECOND VERTEX -----------------
        // Texture coordinates of the second vertex
        glTexCoord2f( this->mapcoord[ this->polygon[i].b ].u,
                      this->mapcoord[ this->polygon[i].b ].v);
        // Coordinates of the second vertex
        glVertex3f( this->vertex[ this->polygon[i].b ].x,
                    this->vertex[ this->polygon[i].b ].y,
                    this->vertex[ this->polygon[i].b ].z);

        //----------------- THIRD VERTEX -----------------
        // Texture coordinates of the third vertex
        glTexCoord2f( this->mapcoord[ this->polygon[i].c ].u,
                      this->mapcoord[ this->polygon[i].c ].v);
        // Coordinates of the Third vertex
        glVertex3f( this->vertex[ this->polygon[i].c ].x,
                    this->vertex[ this->polygon[i].c ].y,
                    this->vertex[ this->polygon[i].c ].z);
    }
    glEnd();
}

object::object()
{
    mesh =new Model_3ds();
    setpos(0,0,0);
    setrot(0,0,0);

}

object::~object()
{
    mesh->~Model_3ds();
}


void object::LoadMesh(const char* filename)
{
    mesh->LoadModel(filename);
}

void object::LoadImage(const char* texture)
{
    mesh->SetTexture(texture);
}



void object::setpos(float x, float y, float z)
{
    pos[0]=x;
    pos[1]=y;
    pos[2]=z;
}

void object::setrot(float x, float y, float z)
{
    rot[0]=x;
    rot[1]=y;
    rot[2]=z;
}

void object::render()
{

    glPushMatrix();
    glTranslatef(pos[0], pos[1], pos[2]);
    glRotatef(rot[0],1,0,0);
    glRotatef(rot[1],0,1,0);
    glRotatef(rot[2],0,0,1);
    mesh->Render();
    glPopMatrix();

}


