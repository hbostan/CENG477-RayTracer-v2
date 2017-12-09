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
	glShadeModel(GL_FLAT);

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

void drawElements()
{
    static bool meshFirstTime = true;

    static int meshVertexPosDataSizeInBytes;

    if (meshFirstTime)
	{
		meshFirstTime = false;

		glEnableClientState(GL_VERTEX_ARRAY);
		
        GLuint indices[] = {
			(GLuint)scene.meshes[0].faces[0].v0_id, (GLuint)scene.meshes[0].faces[0].v1_id, (GLuint)scene.meshes[0].faces[0].v2_id,
			(GLuint)scene.meshes[0].faces[1].v0_id, (GLuint)scene.meshes[0].faces[1].v1_id, (GLuint)scene.meshes[0].faces[1].v2_id,
		};

        GLfloat vertexPos[] = {
            17.717960, 0.023002, 16.940420,
            -17.698040, 0.023002, 16.940420,
            17.717960, 0.023002, -18.475580,
            -17.698040, 0.023002, -18.475580,
        };
        
        GLuint vertexAttribBuffer, indexBuffer;

		glGenBuffers(1, &vertexAttribBuffer);
		glGenBuffers(1, &indexBuffer);

		assert(vertexAttribBuffer > 0 && indexBuffer > 0);

		glBindBuffer(GL_ARRAY_BUFFER, vertexAttribBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

		meshVertexPosDataSizeInBytes = sizeof(vertexPos);
		int indexDataSizeInBytes = sizeof(indices);
		
		glBufferData(GL_ARRAY_BUFFER, meshVertexPosDataSizeInBytes, vertexPos, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexDataSizeInBytes, indices, GL_STATIC_DRAW);
	}

	glVertexPointer(3, GL_FLOAT, 0, 0);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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

	glLoadIdentity();
	glTranslatef(6, -6, deltaZ);
	glRotatef(angle / 8, 1, 1, 0);
	glScalef(5, 5, 5);
	drawCubeElementsVBO();*/

    //angle += 0.5;
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