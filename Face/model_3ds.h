#ifndef MODEL_3DS_H
#define MODEL_3DS_H

#include <string>
#include <GL/glut.h>

using namespace std;

#define MAX_VERTICES 8000 // Max number of vertices (for each object)
#define MAX_POLYGONS 8000 // Max number of polygons (for each object)

// Our vertex type
typedef struct {
    float x,y,z;
}vertex_type;

// The polygon (triangle), 3 numbers that aim 3 vertices
typedef struct {
    int a,b,c;
}polygon_type;

// The mapcoord type, 2 texture coordinates for each vertex
typedef struct {
    float u,v;
}mapcoord_type;


class Model_3ds
{
  public:
    Model_3ds();
    void LoadModel(const char * filename);
    void SetTexture (const char *filename);
    void Render();
    ~Model_3ds();


    string name;
    int vertices_qty;
    int polygons_qty;

    vertex_type *vertex;
    polygon_type *polygon;
    mapcoord_type *mapcoord;
    GLuint Texture;
};

class object
{
  public:
    object();
    ~object();
    void LoadMesh(const char * filename);
    void LoadImage(const char * texture);
    void setpos(float, float, float);
    void setrot(float, float, float);
    void render();

  private:
    float rot[3];
    float pos[3];
    Model_3ds *mesh;

};

#endif // MODEL_3DS_H
