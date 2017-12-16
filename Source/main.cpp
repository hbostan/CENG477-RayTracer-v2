#include <iostream>
#include "parser.h"
#include "Vector.h"
#include <cassert>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//////-------- Global Variables -------/////////

#define PI 3.14159265

GLuint gpuVertexBuffer;
GLuint gpuNormalBuffer;
GLuint gpuIndexBuffer;

// Sample usage for reading an XML scene file
parser::Scene scene;

bool lightsUp = true;
bool lightState[9] = { true, true, true, true, true, true, true, true, true};

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
    glMatrixMode(GL_MODELVIEW); 
    glLoadIdentity();
    gluLookAt(scene.camera.position.x, scene.camera.position.y, scene.camera.position.z,
    m.x, m.y, m.z,
    scene.camera.up.x, scene.camera.up.y, scene.camera.up.z);
    /* Set projection frustrum */
    glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();
    glFrustum(left, right, bottom, top, scene.camera.near_distance, scene.camera.far_distance);

	glMatrixMode(GL_MODELVIEW);
}

void updateCameraPosition()
{
	Vec3f m = scene.camera.position + scene.camera.gaze * scene.camera.near_distance;

	glMatrixMode(GL_MODELVIEW); 
    glLoadIdentity();
	gluLookAt(scene.camera.position.x, scene.camera.position.y, scene.camera.position.z,
    m.x, m.y, m.z,
    scene.camera.up.x, scene.camera.up.y, scene.camera.up.z);
}

void rotateVector(float angle, Vec3f& original, Vec3f direction)
{
	float m[16];

	angle = angle * PI / 180;
    float cosine = cos(angle);
	float sine = sin(angle);

	Vec3f u = direction.normalized();

	float x = u.x;
	float y = u.y;
	float z = u.z;
	float t = 1 - cosine;

	m[0] = t * x * x + cosine;
	m[1] = t * x * y - sine * z;
	m[2] = t * x * z + sine * y;
	m[3] = 0.0;

	m[4] = t * x * y + sine * z;
	m[5] = t * y * y + cosine;
	m[6] = t * y * z - sine * x;
	m[7] = 0.0;

	m[8] = t * x * z - sine * y;
	m[9] = t * y * z + sine * x;
	m[10] = t * z * z + cosine;
	m[11] = 0.0;

	m[12] = 0.0;
	m[13] = 0.0;
	m[14] = 0.0;
	m[15] = 1.0;

	Vec3f result = Vec3f(0);
    result.x = m[0] * original.x + m[1] * original.y + m[2] * original.z;
    result.y = m[4] * original.x + m[5] * original.y + m[6] * original.z;
    result.z = m[8] * original.x + m[9] * original.y + m[10] * original.z;
    original = result;

}

static GLFWwindow* win = NULL;

static void errorCallback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
	else if(key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		scene.camera.position += scene.camera.gaze * 0.05;
		updateCameraPosition();
	}
	else if(key == GLFW_KEY_S && action == GLFW_PRESS)
	{
		scene.camera.position -= scene.camera.gaze * 0.05;
		updateCameraPosition();
	}
	else if(key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		rotateVector(-0.5, scene.camera.gaze, scene.camera.up);
		setCamera();
	}
	else if(key == GLFW_KEY_A && action == GLFW_PRESS)
	{
		rotateVector(0.5, scene.camera.gaze, scene.camera.up);
		setCamera();
	}
	else if(key == GLFW_KEY_U && action == GLFW_PRESS)
	{
		rotateVector(0.5, scene.camera.gaze, scene.camera.right_vector);
		setCamera();
	}
	else if(key == GLFW_KEY_J && action == GLFW_PRESS)
	{
		rotateVector(-0.5, scene.camera.gaze, scene.camera.right_vector);
		setCamera();
	}
	else if(key == GLFW_KEY_0 && action == GLFW_PRESS)
	{
		for(int i=0; i<scene.point_lights.size(); i++)
		{
			if(lightsUp)
				glDisable(GL_LIGHT0 + i);
			else
				glEnable(GL_LIGHT0 + i);
		}
		lightsUp = !lightsUp;
	}
	else if(key > 48 && key < 58 && action == GLFW_PRESS)
	{
		int i = key - 49;
		if(lightState[i])
			glDisable(GL_LIGHT0 + i);
		else
			glEnable(GL_LIGHT0 + i);
		lightState[i] = !lightState[i];
	}
}

void init(int w, int h)
{
    glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
}

void doTransformations(std::vector<parser::Transformation> transformations)
{
	for(auto it = transformations.rbegin(); it!=transformations.rend(); it++)
	{
		auto t = *it;
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

		for(int i=0; i<scene.point_lights.size(); i++)
		{
			glEnable(GL_LIGHT0 + i);
		}
        
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

	GLfloat ambient[] = {scene.ambient_light.x,scene.ambient_light.y,scene.ambient_light.z, 1.0f};


	for(int i=0; i<scene.point_lights.size(); i++)
	{
		GLfloat color[] = {scene.point_lights[i].intensity.x,scene.point_lights[i].intensity.y,scene.point_lights[i].intensity.z, 1.0f};
		GLfloat position[] = {scene.point_lights[i].position.x,scene.point_lights[i].position.y,scene.point_lights[i].position.z, 1.0f};

		glLightfv(GL_LIGHT0 + i,GL_POSITION, position);
		glLightfv(GL_LIGHT0 + i,GL_AMBIENT, ambient);
		glLightfv(GL_LIGHT0 + i,GL_DIFFUSE, color);
		glLightfv(GL_LIGHT0 + i,GL_SPECULAR, color);
	}

	int faces_drawn = 0;
	for(auto mesh: scene.meshes)
	{
		int face_count = mesh.faces.size();
		glPushMatrix();
		doTransformations(mesh.transformations);

		auto mat = scene.materials[mesh.material_id];
		
		GLfloat amb[] ={mat.ambient.x, mat.ambient.y, mat.ambient.z, 1.0};
		GLfloat diff[] ={mat.diffuse.x, mat.diffuse.y, mat.diffuse.z, 1.0};
		GLfloat spec[] = {mat.specular.x, mat.specular.y, mat.specular.z, 1.0};
		GLfloat shininess[] = {mat.phong_exponent};

		glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diff);
		glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
		glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

		if(mesh.mesh_type == "Solid")
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		else if (mesh.mesh_type == "Wireframe")
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}

		glDrawElements(GL_TRIANGLES, face_count*3, GL_UNSIGNED_INT,(const void*)faces_drawn);
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

	drawElements();
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
    
    init(scene.camera.image_width, scene.camera.image_height);

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
