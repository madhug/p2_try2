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
	computeNumberOfNeighbors();

	std::cout<<"Interior N: "<<this->interiorN<<"Boundary N: "<<this->boundaryN<<"\n";

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
}

void GeometryProject::computeNumberOfNeighbors()
{
	unsigned int count = 0;
	interiorN = 0;
	boundaryN = 0;
	for(int i=0; i<mesh.num_vertices; i++)
	{
		count = 0;
		for(int k=0; k<mesh.num_triangles; k++)
		{
			if(mesh.triangles[k].vertices[0] == i||
				mesh.triangles[k].vertices[1] == i||
				mesh.triangles[k].vertices[2] == i)
			{
				count++;
			}
		}
		if(interiorN < count) interiorN = count; 
		if(count < interiorN) boundaryN = count;
	}
}


LSVertex::LSVertex()
{
}

LSVertex::~LSVertex()
{
}



} /* _462 */

