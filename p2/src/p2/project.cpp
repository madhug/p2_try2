/**
 * @file project.cpp
 * @brief Geometry project
 *
 * @author H. Q. Bovik (hqbovik)
 * @bug Unimplemented
 */

#include "project.hpp"
#include "application/opengl.hpp"
#include "application\imageio.hpp"
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

	static GLuint texName;

	GLfloat light_ambient[] = { 0.3, 0.3, 0.3, 1.0 };
    GLfloat light_diffuse[] = { 0.6, 0.6, 0.6, 1.0 };
    GLfloat light_specular[] = { 0.6, 0.6, 0.6, 1.0 };
    GLfloat light_position[] = { 1.0, 100.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };

	glShadeModel(GL_SMOOTH);

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	gluPerspective(camera->get_fov_degrees(),camera->get_aspect_ratio(),camera->get_near_clip(), camera->get_far_clip());	

    // TODO opengl initialization code

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_TEXTURE_2D);

	if(texture_filename != NULL)
	{
		unsigned char* texdata = imageio_load_image( texture_filename, &texwidth, &texheight );
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glGenTextures(1, &texName);
		glBindTexture(GL_TEXTURE_2D, texName);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texwidth, texheight, 0, GL_RGBA, GL_UNSIGNED_BYTE,texdata);
	}


    return true;

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
	Vector3 pos = camera->get_position();
	Vector3 dir = camera->get_direction() + camera->get_position();
	Vector3 up = camera->get_up();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	gluLookAt(pos.x, pos.y, pos.z, dir.x, dir.y, dir.z, up.x, up.y, up.z);

    glEnableClientState( GL_VERTEX_ARRAY ); 
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer( 3, GL_DOUBLE, 8*sizeof(double), mesh.vertices);
	glNormalPointer(GL_DOUBLE, 8*sizeof(double), &(mesh.vertices[0].normal));
	glColor3f(1.0, 0, 0);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glTexCoordPointer(2, GL_DOUBLE,  8*sizeof(double), &(mesh.vertices[0].texture_coord));
	glDrawElements(GL_TRIANGLES, mesh.num_triangles*3 , GL_UNSIGNED_INT, mesh.triangles);
	glDisableClientState( GL_VERTEX_ARRAY ); 
	glDisableClientState(GL_NORMAL_ARRAY);
}


/**
 * Subdivide the mesh that we are rendering using Loop subdivison.
 */
void GeometryProject::subdivide()
{
    // TODO perform a single subdivision.
	LSVertex *vertex_mesh = new LSVertex[mesh.num_vertices];
	for(int i=0; i<mesh.num_vertices; i++)
	{
		vertex_mesh[i].initialize(i, &mesh);
	}
	for(int i=0; i<mesh.num_vertices; i++)
	{
		vertex_mesh[i].isBoundary(vertex_mesh,mesh.num_vertices);
	}

	/*for(int i=0; i<mesh.num_vertices; i++)
	{
		std::cout<<"\nVertex:"<<i<<": ("<<mesh.vertices[vertex_mesh[i].index].position<<") Boundary:"<<vertex_mesh[i].boundary
			<<"\nNeighbors: \n";
		for(int j=0; j<vertex_mesh[i].num_neighbors;j++)
		{
			std::cout<<j<<": ("<<mesh.vertices[vertex_mesh[j].index].position<<")\n";
		}
	}*/


	//find odd vertices
	for(int i=0; i<mesh.num_vertices; i++)
	{
		if(vertex_mesh[i].boundary == true)
		{
			for(int j=0; j<vertex_mesh[i].num_neighbors; j++)
			{
				//calculate position
				vertex_mesh[i].odd_neighbors[j].position = 0.5 * (mesh.vertices[vertex_mesh[i].index].position + 
					mesh.vertices[vertex_mesh[i].neighbors[j]].position);

				//calculate normals
				vertex_mesh[i].odd_neighbors[j].normal = 0.5 * (mesh.vertices[vertex_mesh[i].index].normal + 
					mesh.vertices[vertex_mesh[i].neighbors[j]].normal);
				vertex_mesh[i].odd_neighbors[j].normal = normalize(vertex_mesh[i].odd_neighbors[j].normal);

				//calculate texture coordinates
				vertex_mesh[i].odd_neighbors[j].texture_coord = 0.5 * (mesh.vertices[vertex_mesh[i].index].texture_coord + 
					mesh.vertices[vertex_mesh[i].neighbors[j]].texture_coord);
			}
		}
		else
		{
			for(int j=0; j<vertex_mesh[i].num_neighbors; j++)
			{
				//finding the a, b, c and d vertices to perform the weighted average
				// a is the current vertex
				// b is the neighbor
				// c is the prev neighbor
				// d is the next neighbor

				int prev = j - 1;
				if(j == 0)
				{
					prev = vertex_mesh[i].num_neighbors - 1;
				}

				int next = j + 1;
				if(j == vertex_mesh[i].num_neighbors - 1)
				{
					next = 0;
				}

				//calculate position
				vertex_mesh[i].odd_neighbors[j].position = 0.375 * mesh.vertices[vertex_mesh[i].index].position 
					+ 0.375 * mesh.vertices[vertex_mesh[i].neighbors[j]].position
					+ 0.125 * mesh.vertices[prev].position
					+ 0.125 * mesh.vertices[next].position;

				//calculate normals
				vertex_mesh[i].odd_neighbors[j].normal = 0.375 * mesh.vertices[vertex_mesh[i].index].normal 
					+ 0.375 * mesh.vertices[vertex_mesh[i].neighbors[j]].normal
					+ 0.125 * mesh.vertices[prev].normal
					+ 0.125 * mesh.vertices[next].normal;
				vertex_mesh[i].odd_neighbors[j].normal = normalize(vertex_mesh[i].odd_neighbors[j].normal);

				//calculate texture coordinates
				vertex_mesh[i].odd_neighbors[j].texture_coord = 0.375 * mesh.vertices[vertex_mesh[i].index].texture_coord 
					+ 0.375 * mesh.vertices[vertex_mesh[i].neighbors[j]].texture_coord
					+ 0.125 * mesh.vertices[prev].texture_coord
					+ 0.125 * mesh.vertices[next].texture_coord;
			}
		}
	}

	//find even vertices

	//rebuild mesh
}

void LSVertex::initialize(int i, MeshData *mesh)
{
	index = i; 
	this->isSubdivided = false;

	//get neighbor data
	unsigned int count = 0;
	//arbitrary size
	int *neigh = new int[100];
	for(int k=0; k<mesh->num_triangles; k++)
	{
		if(mesh->triangles[k].vertices[0] == index||
			mesh->triangles[k].vertices[1] == index||
			mesh->triangles[k].vertices[2] == index)
		{
			neigh[count] = k;
			count++;
		}
	}
	//ensure only the required amt of memory is used
	num_neighbors = count;
	neighbors = new int[count];
	for(int i=0; i<count; i++)
	{
		neighbors[i] = neigh[i];
	}
	delete[] neigh;

	//initializing odd neighbors
	num_odd_vertices_calculated = 0;
	odd_neighbors = new Vertex[num_neighbors];

	//is subdivided or not. true only when odd vertices and even vertices have been calculated
	isSubdivided = false;

}

void LSVertex::isBoundary(LSVertex *mesh, int count)
{
	if(num_neighbors <=2 )
	{
		boundary = true;
	}
	else
	{
		for(int i=0; i<num_neighbors; i++)
		{
			int prev = i - 1;
			if(i == 0)
			{
				prev = num_neighbors - 1;
			}
			if(mesh[i].isNeighbor(prev) != true)
			{
				boundary = true;
			}
		}
		boundary = false;
	}
}

bool LSVertex::isNeighbor(int n)
{
	for(int i=0; i<num_neighbors; i++)
	{
		if(neighbors[i] == n)
		{
			return true;
		}
	}
	return false;
}


LSVertex::LSVertex()
{
}

LSVertex::~LSVertex()
{
}


} /* _462 */

