// Include standard headers
#include <stdio.h>
//#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "scene.h"
#include "camera.h"

struct camera_context* camera;

GLFWwindow* create_window(int width, int height, const char* title) {
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
	return window;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
	camera_update_project_matrix(camera, float(width)/float(height));
}

int main( void )
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	GLFWwindow* window = create_window(1024, 768, "Shader Tutorial --- Cube");
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	// 执行这句后，OpenGL环境才就绪
	glfwMakeContextCurrent(window);

	// 初始化GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

    // print out some info about the graphics drivers
    printf("OpenGL version: %s\n", glGetString(GL_VERSION));
	printf("GLSL version: %s\n" , glGetString(GL_SHADING_LANGUAGE_VERSION));
    printf("Vendor: %s\n" ,glGetString(GL_VENDOR));
    printf("Renderer: %s\n" ,glGetString(GL_RENDERER));
	// 获取系统最大支持的顶点属性的数量，通常为16
	GLint num = 0;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &num);
	printf("Maximum vertex attributes supported: %d\n", num);



	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// 确保后续可以检测到ESC键被按下, Windows系统好像不需要?
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited mouvement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// 设置背景清除颜色为深蓝
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	SceneContext scene_context;

	// 创建场景
	if (setup_scene(&scene_context, "vertex_shader.txt", "fragment_shader.txt") < 0) {
		fprintf(stderr, "Failed to setup scene\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	camera = camera_create();
	if(nullptr == camera){
		fprintf(stderr, "Failed to create camera!\n");
		return -1;
	}

	if(camera_init(camera, window,scene_context.program) <0){
		fprintf(stderr, "Failed to init camera!\n");
		camera_destory(camera);
		return -1;
	};

	// 渲染循环
	do {
		// Clear the screen
		// glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		camera_handle_inputs(camera);
		camera_apply(camera);
		// 渲染场景
		render_scene(window, &scene_context);

		// Swap buffers
		glfwSwapBuffers(window);

		// 查询事件
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0 );
	
	// 销毁场景
	destroy_scene(&scene_context);

	camera_destory(camera);
	// 销毁窗口
	glfwDestroyWindow(window);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

int camera_destory(struct camera_context* context){
	if(context){
		delete context;
	}

	return 0;
}
