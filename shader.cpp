#include <stdio.h>
#include <stdlib.h>
#include "shader.h"
GLuint load_shader(GLuint type, const char * file_path) {

	// Read the Vertex Shader code from the file
	FILE* file = fopen(file_path, "rb");
	if(NULL == file){
		printf("Impossible to open %s.\n", file_path);
		return 0;
	}
	fseek(file, 0 , SEEK_END);
	const int file_size = ftell(file);
	printf("shader file %s size: %d\n", file_path, file_size);
	fseek(file, 0 , SEEK_SET);
	char* shader_code = (char* )malloc(file_size+1);
	if(NULL == shader_code){
		printf("Impossible to alloc memory for shader coed .\n");
		fclose(file);
		return 0;
	}
	const size_t read_size = fread(shader_code, file_size, 1, file);
	shader_code[file_size] = '\0';
	
	// Create the shaders
	GLuint shaderID = glCreateShader(type);

	// Compile Vertex Shader
	printf("Compiling shader: %s\n", file_path);
	char const * sourcePointer = shader_code;
	glShaderSource(shaderID, 1, &sourcePointer , NULL);
	glCompileShader(shaderID);
	free(shader_code);
	
	// Check Vertex Shader
	GLint Result = GL_FALSE;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &Result);
	
	int error_message_length = 0;
	glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &error_message_length);
	if ( error_message_length > 0 ){
		char* error_message = (char* )malloc(error_message_length+1);
		if(NULL == error_message){
			printf("Impossible to alloc memory for shader coed .\n");
			return 0;
		}		
		glGetShaderInfoLog(shaderID, error_message_length, NULL, error_message);
		printf("%s\n", error_message);
		free(error_message);
	}
	return shaderID;
}