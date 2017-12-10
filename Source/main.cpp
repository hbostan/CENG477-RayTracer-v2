#include <iostream>
#include "parser.h"
#include "Vector.h"
#include <cassert>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//////-------- Global Variables -------/////////

GLuint gpuVertexBuffer;
GLuint gpuNormalBuffer;
GLuint gpuIndexBuffer;

// Sample usage for reading an XML scene file
parser::Scene scene;
static GLFWwindow* win = NULL;

static void errorCallback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void reshape(GLFWwindow* win, int w, int h)   // Create The Reshape Function (the viewport)
{
    /*w = w < 1 ? 1 : w;
    h = h < 1 ? 1 : h;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glOrtho(-10, 10, -10, 10, -10, 10);
    //gluPerspective(45, 1, 1, 500);
    glFrustum(scene.camera.near_plane.x, scene.camera.near_plane.y,
                scene.camera.near_plane.z, scene.camera.near_plane.w,
                scene.camera.near_distance, scene.camera.far_distance);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();*/
}

void init(int w, int h)
{
    glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

    //reshape(win, w, h);
}

void drawCubeElementsVBO()
{
	static bool firstTime = true;

	static int vertexPosDataSizeInBytes;
	
	if (firstTime)
	{
		firstTime = false;

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		GLuint indices[] = {
			0, 1, 2, // front
			3, 0, 2, // front
			4, 7, 6, // back
			5, 4, 6, // back
			0, 3, 4, // left
			3, 7, 4, // left
			2, 1, 5, // right
			6, 2, 5, // right
			3, 2, 7, // top
			2, 6, 7, // top
			0, 4, 1, // bottom
			4, 5, 1  // bottom
		};

		GLfloat vertexPos[] = {
			-0.5, -0.5,  0.5, // 0: bottom-left-front
			0.5, -0.5,  0.5, // 1: bottom-right-front
			0.5,  0.5,  0.5, // 2: top-right-front
			-0.5,  0.5,  0.5, // 3: top-left-front
			-0.5, -0.5, -0.5, // 4: bottom-left-back
			0.5, -0.5, -0.5, // 5: bottom-right-back
			0.5,  0.5, -0.5, // 6: top-right-back
			-0.5,  0.5, -0.5, // 7: top-left-back
		};

		GLfloat vertexCol[] = {
			1.0, 1.0, 1.0, // 0: unused
			0.0, 1.0, 1.0, // 1: bottom
			1.0, 0.0, 0.0, // 2: front
			1.0, 1.0, 1.0, // 3: unused
			1.0, 1.0, 0.0, // 4: left
			0.0, 0.0, 1.0, // 5: right
			0.0, 1.0, 0.0, // 6: back 
			1.0, 0.0, 1.0, // 7: top
		};

		GLuint vertexAttribBuffer, indexBuffer;

		glGenBuffers(1, &vertexAttribBuffer);
		glGenBuffers(1, &indexBuffer);

		assert(vertexAttribBuffer > 0 && indexBuffer > 0);

		glBindBuffer(GL_ARRAY_BUFFER, vertexAttribBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

		vertexPosDataSizeInBytes = sizeof(vertexPos);
		int vertexColDataSizeInBytes = sizeof(vertexCol);
		int indexDataSizeInBytes = sizeof(indices);
		
		glBufferData(GL_ARRAY_BUFFER, vertexPosDataSizeInBytes + vertexColDataSizeInBytes, 0, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertexPosDataSizeInBytes, vertexPos);
		glBufferSubData(GL_ARRAY_BUFFER, vertexPosDataSizeInBytes, vertexColDataSizeInBytes, vertexCol);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexDataSizeInBytes, indices, GL_STATIC_DRAW);
	}

	glVertexPointer(3, GL_FLOAT, 0, 0);
	glColorPointer(3, GL_FLOAT, 0, (void*)vertexPosDataSizeInBytes);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}


void doTransformations(std::vector<parser::Transformation> transformations)
{
	for(auto t: transformations)
	{
		if(t.transformation_type == "Translation")
		{
			auto translation = scene.translations[t.id];
			glTranslatef(translation.x, translation.y, translation.z);
		}
		else if(t.transformation_type == "Rotation")
		{
			auto rotation = scene.rotations[t.id];
			glRotatef(rotation.x,rotation.y, rotation.z, rotation.w);
		}
		else if(t.transformation_type == "Scaling")
		{
			auto scaling = scene.translations[t.id];
			glScalef(scaling.x, scaling.y, scaling.z);
		}
	}
}


void drawElements()
{
    static bool meshFirstTime = true;

    static int meshVertexPosDataSizeInBytes;

	static int faces_size;	
	static int vertex_data_size;

    if (meshFirstTime)
	{
		meshFirstTime = false;
		faces_size = 0;	
		vertex_data_size = 0;

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnable(GL_LIGHTING);

		for(auto mesh: scene.meshes)
		{
			faces_size += mesh.faces.size();
		}

		GLuint* indices = new GLuint[faces_size*3];
        
		int idx =0;
		for(auto mesh: scene.meshes)
		{
			for(int i=0; i<mesh.faces.size(); i++)
			{
				indices[idx] = (GLuint) mesh.faces[i].v0_id;
				indices[++idx] = (GLuint)mesh.faces[i].v1_id;
				indices[++idx] = (GLuint)mesh.faces[i].v2_id;
				idx++;
			}
		}


		vertex_data_size = scene.vertex_data.size();


		GLfloat* vertexPos = new GLfloat[vertex_data_size*3];

		for(int i=0, j=0; i<vertex_data_size; j++, i++)
		{
			vertexPos[j] = scene.vertex_data[i].x;
			vertexPos[++j] = scene.vertex_data[i].y;
			vertexPos[++j] = scene.vertex_data[i].z;
		}


		//
		//	CALCULATE NORMALS
		//
		Vec3f* normalArray = new Vec3f[vertex_data_size];

		for(int i = 0; i < vertex_data_size; i++)
		{
			normalArray[i] = Vec3f(0);
		}

		for(auto mesh: scene.meshes)
		{
			for(int j = 0; j < mesh.faces.size(); j++)
			{
				int v0 = mesh.faces[j].v0_id;
				int v1 = mesh.faces[j].v1_id;
				int v2 = mesh.faces[j].v2_id;

				Vec3f a = scene.vertex_data[v0];
				Vec3f b = scene.vertex_data[v1];
				Vec3f c = scene.vertex_data[v2];

				Vec3f n = (b-a).cross(c-a).normalized();

				normalArray[v0] += n;
				normalArray[v1] += n;
				normalArray[v2] += n;
			}
		}

		GLfloat* vertexNormals = new GLfloat[vertex_data_size*3];

		for(int i=0, j=0; i<vertex_data_size; j++, i++)
		{
			normalArray[i].normalize();
			vertexNormals[j] = normalArray[i].x;
			vertexNormals[++j] = normalArray[i].y;
			vertexNormals[++j] = normalArray[i].z;
		}

		delete[] normalArray;

		
		GLfloat ambient[] = {scene.ambient_light.x,scene.ambient_light.y,scene.ambient_light.z, 1.0f};

		for(int i=0; i<scene.point_lights.size(); i++)
		{
			glEnable(GL_LIGHT0 + i);
			GLfloat color[] = {scene.point_lights[i].intensity.x,scene.point_lights[i].intensity.y,scene.point_lights[i].intensity.z, 1.0f};
			GLfloat position[] = {scene.point_lights[i].position.x,scene.point_lights[i].position.y,scene.point_lights[i].position.z, 1.0f};

			glLightfv(GL_LIGHT0 + i,GL_POSITION, position);
			glLightfv(GL_LIGHT0 + i,GL_DIFFUSE, color);
			glLightfv(GL_LIGHT0 + i,GL_SPECULAR, color);
		}

		/*glEnable(GL_LIGHT0);
		GLfloat color[] = {1.0,0,0,1};
		GLfloat position[] = {2,3,4,1};
		GLfloat ambient[] = {0.05, 0.05, 0.05, 1};
		glLightfv(GL_LIGHT0,GL_POSITION, position);
		glLightfv(GL_LIGHT0,GL_DIFFUSE, color);
		glLightfv(GL_LIGHT0,GL_SPECULAR, color);
		glLightfv(GL_LIGHT0,GL_AMBIENT, ambient);
		

		
		glEnable(GL_LIGHT1);
		GLfloat color1[] = {0,0,1.0,1};
		GLfloat position1[] = {-2,3,4,1};
		GLfloat ambient1[] = {0.05, 0.05, 0.05, 1};
		glLightfv(GL_LIGHT1,GL_POSITION, position1);
		glLightfv(GL_LIGHT1,GL_DIFFUSE, color1);
		glLightfv(GL_LIGHT1,GL_SPECULAR, color1);
		glLightfv(GL_LIGHT1,GL_AMBIENT, ambient1);*/



		std::cout << faces_size << std::endl;
		std::cout << vertex_data_size << std::endl;
        
        GLuint vertexAttribBuffer, indexBuffer;

		glGenBuffers(1, &vertexAttribBuffer);
		glGenBuffers(1, &indexBuffer);

		assert(vertexAttribBuffer > 0 && indexBuffer > 0);

		glBindBuffer(GL_ARRAY_BUFFER, vertexAttribBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

		meshVertexPosDataSizeInBytes = sizeof(GLfloat)*vertex_data_size*3;
		int normalDataSizeInBytes = sizeof(GLfloat)*vertex_data_size*3;
		int indexDataSizeInBytes = sizeof(GLuint)*faces_size*3;
		
		glBufferData(GL_ARRAY_BUFFER, meshVertexPosDataSizeInBytes+normalDataSizeInBytes, vertexPos, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, meshVertexPosDataSizeInBytes, vertexPos);
		glBufferSubData(GL_ARRAY_BUFFER, meshVertexPosDataSizeInBytes, normalDataSizeInBytes, vertexNormals);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexDataSizeInBytes, indices, GL_STATIC_DRAW);

		delete[] vertexPos;
		delete[] indices;
	}
	
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glNormalPointer(GL_FLOAT,0,(void*)meshVertexPosDataSizeInBytes);

	int faces_drawn = 0;
	for(auto mesh: scene.meshes)
	{
		int face_count = mesh.faces.size();
		glPushMatrix();
		doTransformations(mesh.transformations);//glRotatef(135, 0, 1, 0);
		glDrawElements(GL_TRIANGLES, face_count*3, GL_UNSIGNED_INT,(const void*)faces_drawn);// (const void*)(sizeof(GLfloat)*6));
		glPopMatrix();
		faces_drawn += sizeof(GLfloat)*face_count*3;
	}

}

void display()
{
	glClearColor(0, 0, 0, 1);
	glClearDepth(1.0f);
	glClearStencil(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	static float angle = 0;

	GLfloat deltaZ = -30;

	/*glLoadIdentity();
	glTranslatef(-6, -6, deltaZ);
	glRotatef(angle, 1, 0, 0);
	glScalef(5, 5, 5);
	drawCubeElementsVBO();

	glLoadIdentity();
	glTranslatef(-6, 6, deltaZ);
	glRotatef(angle / 2, 0, 1, 0);
	glScalef(5, 5, 5);
	drawCubeElementsVBO();

	glLoadIdentity();
	glTranslatef(6, 6, deltaZ);
	glRotatef(angle / 4, 0, 0, 1);
	glScalef(5, 5, 5);
	drawCubeElementsVBO();
*/
	
	angle += 0.5;
    drawElements();
}

void setCamera()
{
    scene.camera.gaze = scene.camera.gaze.normalized();
    scene.camera.right_vector = (scene.camera.up.cross(-scene.camera.gaze)).normalized();
    scene.camera.up = (scene.camera.right_vector.cross(scene.camera.gaze)).normalized();

    float left = scene.camera.near_plane.x;
    float right = scene.camera.near_plane.y;
    float bottom = scene.camera.near_plane.z;
    float top = scene.camera.near_plane.w;
        
    Vec3f m = scene.camera.position + scene.camera.gaze * scene.camera.near_distance;

    /* Set camera position */ 
    //glMatrixMode(GL_MODELVIEW); 
    //glLoadIdentity();
    gluLookAt(scene.camera.position.x, scene.camera.position.y, scene.camera.position.z,
    m.x, m.y, m.z,
    scene.camera.up.x, scene.camera.up.y, scene.camera.up.z);
    /* Set projection frustrum */
    glMatrixMode(GL_PROJECTION); glLoadIdentity( );
    glFrustum(left, right, bottom, top, scene.camera.near_distance, scene.camera.far_distance);

    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char* argv[]) {
    scene.loadFromXml(argv[1]);

    glfwSetErrorCallback(errorCallback);

    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    win = glfwCreateWindow(scene.camera.image_width, scene.camera.image_height, "CENG477", NULL, NULL);
    if (!win) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(win);

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(win, keyCallback);
    glfwSetWindowSizeCallback(win, reshape);
    
    init(scene.camera.image_width, scene.camera.image_height);
    //init(640, 480);

    setCamera();

    while(!glfwWindowShouldClose(win)) {
        glfwPollEvents();

        display();

        glfwSwapBuffers(win);
    }

    glfwDestroyWindow(win);
    glfwTerminate();

    exit(EXIT_SUCCESS);

    return 0;
}
