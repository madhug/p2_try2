/**
 * @file project.hpp
 * @brief Geometry project
 *
 * @author H. Q. Bovik (hqbovik)
 * @bug Unimplemented
 */

#ifndef _462_GEOMETRY_PROJECT_HPP_
#define _462_GEOMETRY_PROJECT_HPP_

#include "math/camera.hpp"


/*
   A namespace declaration. All project files use this namespace.
   Add this declaration (and its closing) to all source/headers you create.
   Note that all #includes should be BEFORE the namespace declaration.
 */
namespace _462 {

struct Triangle
{
    // index into vertex list of the 3 vertices of this triangle
    unsigned int vertices[3];
};

struct Vertex
{
    // the position of the vertex
    Vector3 position;
    // the normal of the vertex
    Vector3 normal;
    // the texture coordinate of the vertex
    Vector2 texture_coord;
};

struct MeshData
{
    // array of vertices
    Vertex* vertices;
    // size of vertex array
    size_t num_vertices;

    // array of triangles
    Triangle* triangles;
    // size of triangle array
    size_t num_triangles;
};

struct LSEdge
{
	unsigned int vertices[2];
	bool isSubdivided;
	bool isBoundary;
	Vertex odd_vertex;
};


class LSTriangle
{
public:
	LSTriangle(int i, const MeshData* mesh);
	LSTriangle(){};
	~LSTriangle();

	bool hasVertex(int i, const MeshData* mesh);
	bool hasEdge(int u, int v, const MeshData* mesh);
	bool hasEdge(const LSEdge* edge, const MeshData* mesh);
	bool hasEdge(const LSEdge* edge, unsigned int triangleIndex, const MeshData* mesh);

private:
	unsigned int index;
	unsigned int neighbors[3];
	bool isSubdivided;
	LSEdge edges[3];
	Vertex even_vertex[3];
};




class GeometryProject
{
public:

    // constructor, invoked when object is created
    GeometryProject();
    // destructor, invoked when object is destroyed
    ~GeometryProject();

    // more detailed specifications for each function are in project.cpp.

    // Initialize the project, loading the mesh from the given filename.
    // Returns true on success.
    bool initialize( const Camera* camera, const MeshData* mesh, const char* texture_filename );
    // Clean up the project, free any memory, etc.
    void destroy();
    // Render the mesh using the given camera.
    void render( const Camera* camera );
    // Subdivide the mesh
    void subdivide();


private:

    MeshData mesh;

    // TODO add any other private members/functions here.

    // since this has no meaningful assignment/copy, prevent the compiler from
    // automatically generating those functions
    GeometryProject( const GeometryProject& );
    GeometryProject& operator=( const GeometryProject& );
};

} /* _462 */

#endif /* _462_GEOMETRY_PROJECT_HPP_ */

