#include <stdio.h>
#include <math.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "scene.h"
#include "shader.h"
#include "texture.h"

static const GLfloat g_trangle_vertex_buffer_data[] = {
	-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.5f, 1.0f,
};

static const GLfloat g_cube_vertex_buffer_data[] = {
	-0.5f, -0.5f, -0.5f,  0.583f,  0.771f,  0.014f,   0.0f, 0.0f, 
	0.5f, -0.5f, -0.5f,    0.609f,  0.115f,  0.436f,   1.0f, 0.0f, 
	0.5f,  0.5f, -0.5f,    0.327f,  0.483f,  0.844f,   1.0f, 1.0f, 
	0.5f,  0.5f, -0.5f,    0.822f,  0.569f,  0.201f,   1.0f, 1.0f, 
	-0.5f,  0.5f, -0.5f,  0.435f,  0.602f,  0.223f,   0.0f, 1.0f, 
	-0.5f, -0.5f, -0.5f,  0.310f,  0.747f,  0.185f,   0.0f, 0.0f, 

	-0.5f, -0.5f,  0.5f,  0.597f,  0.770f,  0.761f,   0.0f, 0.0f, 
	0.5f, -0.5f,  0.5f,    0.559f,  0.436f,  0.730f,   1.0f, 0.0f, 
	0.5f,  0.5f,  0.5f,    0.359f,  0.583f,  0.152f,   1.0f, 1.0f, 
	0.5f,  0.5f,  0.5f,    0.483f,  0.596f,  0.789f,   1.0f, 1.0f, 
	-0.5f,  0.5f,  0.5f,  0.559f,  0.861f,  0.639f,   0.0f, 1.0f, 
	-0.5f, -0.5f,  0.5f,  0.195f,  0.548f,  0.859f,   0.0f, 0.0f, 

	-0.5f,  0.5f,  0.5f,  0.014f,  0.184f,  0.576f,   1.0f, 0.0f, 
	-0.5f,  0.5f, -0.5f,  0.771f,  0.328f,  0.970f,   1.0f, 1.0f, 
	-0.5f, -0.5f, -0.5f,  0.406f,  0.615f,  0.116f,   0.0f, 1.0f, 
	-0.5f, -0.5f, -0.5f,  0.676f,  0.977f,  0.133f,   0.0f, 1.0f, 
	-0.5f, -0.5f,  0.5f,  0.971f,  0.572f,  0.833f,   0.0f, 0.0f, 
	-0.5f,  0.5f,  0.5f,  0.140f,  0.616f,  0.489f,   1.0f, 0.0f, 

	0.5f,  0.5f,  0.5f,    0.997f,  0.513f,  0.064f,   1.0f, 0.0f, 
	0.5f,  0.5f, -0.5f,    0.945f,  0.719f,  0.592f,   1.0f, 1.0f, 
	0.5f, -0.5f, -0.5f,    0.543f,  0.021f,  0.978f,   0.0f, 1.0f, 
	0.5f, -0.5f, -0.5f,    0.279f,  0.317f,  0.505f,   0.0f, 1.0f, 
	0.5f, -0.5f,  0.5f,    0.167f,  0.620f,  0.077f,   0.0f, 0.0f, 
	0.5f,  0.5f,  0.5f,    0.347f,  0.857f,  0.137f,   1.0f, 0.0f, 

	-0.5f, -0.5f, -0.5f,  0.055f,  0.953f,  0.042f,   0.0f, 1.0f, 
	0.5f, -0.5f, -0.5f,    0.714f,  0.505f,  0.345f,   1.0f, 1.0f, 
	0.5f, -0.5f,  0.5f,    0.783f,  0.290f,  0.734f,   1.0f, 0.0f, 
	0.5f, -0.5f,  0.5f,    0.722f,  0.645f,  0.174f,   1.0f, 0.0f, 
	-0.5f, -0.5f,  0.5f,  0.302f,  0.455f,  0.848f,   0.0f, 0.0f, 
	-0.5f, -0.5f, -0.5f,  0.225f,  0.587f,  0.040f,   0.0f, 1.0f, 

	-0.5f,  0.5f, -0.5f,  0.517f,  0.713f,  0.338f,   0.0f, 1.0f, 
	0.5f,  0.5f, -0.5f,    0.053f,  0.959f,  0.120f,   1.0f, 1.0f, 
	0.5f,  0.5f,  0.5f,    0.393f,  0.621f,  0.362f,   1.0f, 0.0f, 
	0.5f,  0.5f,  0.5f,    0.673f,  0.211f,  0.457f,   1.0f, 0.0f, 
	-0.5f,  0.5f,  0.5f,  0.820f,  0.883f,  0.371f,   0.0f, 0.0f, 
	-0.5f,  0.5f, -0.5f,  0.982f,  0.099f,  0.879f,   0.0f, 1.0f, 
};

int setup_scene(SceneContext* scene_context, const char* vertexShader, const char* fragmentShader) {

	// 创建VBO
	glGenBuffers(1, &scene_context->vbo);

	// 绑定VBO
	glBindBuffer(GL_ARRAY_BUFFER, scene_context->vbo);

	// 传输数据到VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_cube_vertex_buffer_data), g_cube_vertex_buffer_data, GL_STATIC_DRAW);

	// 创建VAO
	glGenVertexArrays(1, &scene_context->vao);

	// 绑定VBO
	glBindVertexArray(scene_context->vao);
	
	printf("Reading vertex shader file %s\n", vertexShader);
	GLuint vertexShaderId = load_shader(GL_VERTEX_SHADER, vertexShader);

	printf("Reading fragment shader file %s\n", fragmentShader);
	GLuint fragmentShaderId = load_shader(GL_FRAGMENT_SHADER, fragmentShader);

	// Link the program
	printf("Linking program\n");
	scene_context->program =  glCreateProgram();

	glAttachShader(scene_context->program, vertexShaderId);
	glAttachShader(scene_context->program, fragmentShaderId);
	glLinkProgram(scene_context->program);

	// Check the program
	GLint Result = GL_FALSE;
	glGetProgramiv(scene_context->program, GL_LINK_STATUS, &Result);

	int error_message_length = 0;
	glGetProgramiv(scene_context->program, GL_INFO_LOG_LENGTH, &error_message_length);
	if ( error_message_length > 0 ){
		char* error_message = (char* )malloc(error_message_length+1);
		if(NULL == error_message){
			printf("Impossible to alloc memory for shader coed .\n");
			return 0;
		}				
		glGetProgramInfoLog(scene_context->program, error_message_length, NULL, error_message);
		printf("%s\n", error_message);
	}

	glDetachShader(scene_context->program, vertexShaderId);
	glDetachShader(scene_context->program, fragmentShaderId);
	glDeleteShader(fragmentShaderId);
	glDeleteShader(fragmentShaderId);

	glUseProgram(scene_context->program);

	// 设置顶点位置属性
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		8 * sizeof(GL_FLOAT),                  // stride
		(void*)0            // array buffer offset
	);

	// 设置顶点颜色属性
	glVertexAttribPointer(
		1,                  // attribute 0. No particular reason for 1, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		8 * sizeof(GL_FLOAT),                  // stride
		(void*)(3 * sizeof(float))            // array buffer offset
	);

	// 设置顶点UV属性
	glVertexAttribPointer(
		2,                  // attribute 0. No particular reason for 1, but must match the layout in the shader.
		2,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		8 * sizeof(GL_FLOAT),                  // stride
		(void*)(6 * sizeof(float))            // array buffer offset
	);

	// 启用顶点属性，在glVertexAttribPointer之前之后调用都可以
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	// Load Texture
	const char* image_path = "texture.bmp";
	printf("Reading image %s\n", image_path);
	GLuint textureID = load_bmp_texture(image_path);
	glBindTexture(GL_TEXTURE_2D, textureID);
	return 0;
}

int destroy_scene(SceneContext* scene_context) {

	// 禁用顶点属性
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	// Cleanup VBO
	glDeleteBuffers(1, &scene_context->vbo);
	// Cleanup VAO
	glDeleteVertexArrays(1, &scene_context->vao);
	// Cleanup Program
	glDeleteProgram(scene_context->program);

	return 0;
}


void update_vbo(GLuint vbo) {
	// 绑定VBO,如果已经绑定则无需重复绑定
	//glBindBuffer(GL_ARRAY_BUFFER_ARB, vbo);

	// 映射VBO
	float* ptr = (float*)glMapBuffer(GL_ARRAY_BUFFER_ARB, GL_WRITE_ONLY_ARB);

	// 如果指针为空（映射后的），更新VBO
	if (ptr)
	{
		// 更新位置信息
		float timeValue = glfwGetTime();

		float color_delta = 0.25*cos(timeValue*3.14) + 0.75;

		for (int t = 0; t < 12*3; t++) {
			int index = t * 8 + 3;
			ptr[index++] = g_cube_vertex_buffer_data[index] * color_delta;
			ptr[index++] = g_cube_vertex_buffer_data[index] * color_delta;
			ptr[index] = g_cube_vertex_buffer_data[index] * color_delta;
		}		
		glUnmapBufferARB(GL_ARRAY_BUFFER_ARB); // 使用之后解除映射
	}
}

void update_uniform(GLFWwindow* window, GLuint program) {

	// Update uniform
	float timeValue = glfwGetTime();

	int uniform_location = glGetUniformLocation(program, "time_value");
	glUniform1f(uniform_location, timeValue);
}

int render_scene(GLFWwindow* window, SceneContext* scene_context) {
	// 更新_vbo
	update_vbo(scene_context->vbo);

	// 更新uniform
	update_uniform(window,scene_context->program);
	
	// Draw the triangles!
	glDrawArrays(GL_TRIANGLES, 0, 12*3); // 3 indices starting at 0 -> 1 triangle

	return 0;
}

