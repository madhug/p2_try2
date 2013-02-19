/**
 * @file project.cpp
 * @brief Geometry project
 *
 * @author H. Q. Bovik (hqbovik)
 * @bug Unimplemented
 */

#include "project.hpp"
#include "application/opengl.hpp"

/*
   A namespace declaration. All project files use this namespace.
   Add this declaration (and its closing) to all source/headers you create.
   Note that all #includes should be BEFORE the namespace declaration.
 */
namespace _462 {

// definitions of functions for the GeometryProject class

// constructor, invoked when object is allocated
GeometryProject::GeometryProject() { }

// destructor, invoked when object is de-allocated
GeometryProject::~GeometryProject() { }

/**
 * Initialize the project, doing any necessary opengl initialization.
 * @param camera An already-initialized camera.
 * @param mesh The mesh to be rendered and subdivided.
 * @param texture_filename The filename of the texture to use with the mesh.
 *  Is null if there is no texture data with the mesh or no texture filename
 *  was passed in the arguments, in which case textures should not be used.
 * @return true on success, false on error.
 */
bool GeometryProject::initialize( const Camera* camera, const MeshData* mesh, const char* texture_filename )
{
    this->mesh = *mesh;

    // TODO opengl initialization code
    return true;
}

/**
 * Clean up the project. Free any memory, etc.
 */
void GeometryProject::destroy()
{
  // TODO any cleanup code
}

/**
 * Clear the screen, then render the mesh using the given camera.
 * @param camera The logical camera to use.
 * @see scene/camera.hpp
 */
void GeometryProject::render( const Camera* camera )
{
  // TODO render code
}


/**
 * Subdivide the mesh that we are rendering using Loop subdivison.
 */
void GeometryProject::subdivide()
{
    // TODO perform a single subdivision.

	LSTriangle* ls_triangles = new LSTriangle[this->mesh.num_triangles];
	
	for(int i=0; i<mesh.num_triangles; i++)
	{

	}
	

}

LSTriangle::LSTriangle(int i, const MeshData* mesh)
{
	index = i;
	isSubdivided = false;

	//setting the edges
	edges[0].vertices[0] = mesh->triangles[i].vertices[0];
	edges[0].vertices[1] = mesh->triangles[i].vertices[1];
	edges[0].isSubdivided = false;
	edges[0].isBoundary = false;
	
	edges[1].vertices[0] = mesh->triangles[i].vertices[1];
	edges[1].vertices[1] = mesh->triangles[i].vertices[2];
	edges[1].isSubdivided = false;
	edges[1].isBoundary = false;

	edges[2].vertices[0] = mesh->triangles[i].vertices[2];
	edges[2].vertices[1] = mesh->triangles[i].vertices[0];
	edges[2].isSubdivided = false;
	edges[2].isBoundary = false;
	
	//setting neighbours
	neighbors[0] = neighbors[1] = neighbors[2] = -1;
	
	for(int j=0; j<3; j++)
	{
		if(hasEdge(&edges[j],index+j+1,mesh))
		{
			if(neighbors[j] == -1)
			{
				neighbors[j] = index+j+1;
			}
		}
	}
	
	if(neighbors[0] == -1)
	{
		for(int j=-3; j<0; j++)
		{
			if(hasEdge(&edges[j],index+j,mesh))
			{
				neighbors[0] = index+j;
			}
		}
		if(neighbors[0] == -1)
		{
			edges[0].isBoundary = true;
		}
	}

	if(neighbors[1] == -1)
	{
		for(int j=-3; j<0; j++)
		{
			if(hasEdge(&edges[j],index+j,mesh))
			{
				neighbors[1] = index+j;
			}
		}
		if(neighbors[1] == -1)
		{
			edges[1].isBoundary = true;
		}
	}

	if(neighbors[2] == -1)
	{
		for(int j=-3; j<0; j++)
		{
			if(hasEdge(&edges[j],index+j,mesh))
			{
				neighbors[2] = index+j;
			}
		}
		if(neighbors[2] == -1)
		{
			edges[2].isBoundary = true;
		}
	}

}

LSTriangle::~LSTriangle()
{

}

bool LSTriangle::hasVertex(int i, const MeshData* mesh)
{
	if(mesh->triangles[index].vertices[0] == i
		|| mesh->triangles[index].vertices[1] == i
		|| mesh->triangles[index].vertices[2] == i)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool LSTriangle::hasEdge(int u, int v, const MeshData* mesh)
{
	if(this->hasVertex(u,mesh) && this->hasVertex(v,mesh))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool LSTriangle::hasEdge(const LSEdge* edge, const MeshData* mesh)
{
	if(this->hasEdge(edge->vertices[0], edge->vertices[1],mesh))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool LSTriangle::hasEdge(const LSEdge* edge, unsigned int i, const MeshData* mesh)
{
	bool hasV1=false, hasV2=false;
	if(mesh->triangles[i].vertices[0] == edge->vertices[0]
	||mesh->triangles[i].vertices[1] == edge->vertices[0]
	||mesh->triangles[i].vertices[2] == edge->vertices[0])
	{
		hasV1 = true;
	}
	if(mesh->triangles[i].vertices[0] == edge->vertices[1]
	||mesh->triangles[i].vertices[1] == edge->vertices[2]
	||mesh->triangles[i].vertices[2] == edge->vertices[3])
	{
		hasV2 = true;
	}
	if(hasV1 && hasV2)
	{
		return true;
	}
	else
	{
		return false; 
	}
}


} /* _462 */

