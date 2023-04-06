#include <stdio.h>
#include <math.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "scene.h"
#include "shader.h"
#include "texture.h"
#include "objloader.h"

static const GLfloat g_vertex_buffer_data[] = {
    -0.5f,  0.5f, // 左上
     0.5f,  0.5f, // 右上
     0.5f, -0.5f, // 右下
    -0.5f, -0.5f  // 左下
};

int setup_scene(SceneContext* scene_context, const char* vertexShader, const char* fragmentShader, const char* geometryShader) {

	// Read our .obj file
	//bool res = loadOBJ("suzanne.obj", scene_context->vertices, scene_context->uvs, scene_context->normals);
	//bool res = loadOBJ("cube.obj", scene_context->vertices, scene_context->uvs, scene_context->normals);

	// 创建VBO
	glGenBuffers(1, &scene_context->vbo_vertices);
	// 绑定VBO
	glBindBuffer(GL_ARRAY_BUFFER, scene_context->vbo_vertices);
	// 传递数据到VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data) , &g_vertex_buffer_data, GL_STATIC_DRAW);

	// 创建VAO
	glGenVertexArrays(1, &scene_context->vao);

	// 绑定VBO
	glBindVertexArray(scene_context->vao);
	
	printf("Reading vertex shader file %s\n", vertexShader);
	GLuint vertexShaderId = load_shader(GL_VERTEX_SHADER, vertexShader);

	printf("Reading geometry shader file %s\n", geometryShader);
	GLuint geometryShaderId = load_shader(GL_GEOMETRY_SHADER, geometryShader);

	printf("Reading fragment shader file %s\n", fragmentShader);
	GLuint fragmentShaderId = load_shader(GL_FRAGMENT_SHADER, fragmentShader);

	// Link the program
	printf("Linking program\n");
	scene_context->program =  glCreateProgram();

	glAttachShader(scene_context->program, vertexShaderId);
	glAttachShader(scene_context->program, geometryShaderId);
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
	glDetachShader(scene_context->program, geometryShaderId);
	glDetachShader(scene_context->program, fragmentShaderId);
	glDeleteShader(vertexShaderId);
	glDeleteShader(geometryShaderId);
	glDeleteShader(fragmentShaderId);
	glUseProgram(scene_context->program);

	glBindBuffer(GL_ARRAY_BUFFER, scene_context->vbo_vertices);
	// 设置顶点位置属性
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		2,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0 * sizeof(GL_FLOAT),   // stride
		(void*)0            // array buffer offset
	);

	// 启用顶点属性，在glVertexAttribPointer之前之后调用都可以
	glEnableVertexAttribArray(0);
	//glEnableVertexAttribArray(1);
	//glEnableVertexAttribArray(2);

/*
	glm::vec3 light_pos = glm::vec3(0,4,5);
	location = glGetUniformLocation(scene_context->program,"LightPosition_worldspace");
	glUniform3f(location,light_pos.x, light_pos.y, light_pos.z);
*/
	return 0;
}

int destroy_scene(SceneContext* scene_context) {

	// 禁用顶点属性
	glDisableVertexAttribArray(0);
//	glDisableVertexAttribArray(1);
//	glDisableVertexAttribArray(2);

	// Cleanup VBOs
	glDeleteBuffers(1, &scene_context->vbo_vertices);
//	glDeleteBuffers(1, &scene_context->vbo_uvs);
//	glDeleteBuffers(1, &scene_context->vbo_normals);
	// Cleanup VAO
	glDeleteVertexArrays(1, &scene_context->vao);
	// Cleanup Program
	glDeleteProgram(scene_context->program);

	return 0;
}

void update_uniform(GLFWwindow* window, GLuint program) {

	// Update uniform

	float timeValue = glfwGetTime();


	int uniform_location = glGetUniformLocation(program, "time_value");
	glUniform1f(uniform_location, timeValue);

	uniform_location = glGetUniformLocation(program, "light_position_worldspace");

	glUniform3f(uniform_location, 0.0, sin(timeValue*3.14)*3.0, 5.0);
}

int render_scene(GLFWwindow* window, SceneContext* scene_context) {
	// 更新_vbo
	//update_vbo(0);

	// 更新uniform
	update_uniform(window,scene_context->program);
	
	glBindBuffer(GL_ARRAY_BUFFER, scene_context->vbo_vertices);

	// 设置顶点位置属性
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		2,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0 * sizeof(GL_FLOAT),                  // stride
		(void*)0            // array buffer offset
	);

	// Draw the triangles!
	glDrawArrays(GL_POINTS, 0, 4);

	return 0;
}

