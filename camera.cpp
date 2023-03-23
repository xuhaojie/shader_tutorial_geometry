#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "camera.h"

struct camera_context
{

	// Initial Field of View
	const float initial_FoV = 45.0f;

	const float horizontal_start_angle = 3.14f;
	const float vertical_start_angle = 0.0f;
	glm::mat4 model_matrix = glm::mat4(1.0f);
	glm::mat4 view_matrix = glm::lookAt(
		glm::vec3(4, 3, 3), // Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)	// Head is up (set to 0,-1,0 to look upside-down)
	);
	glm::mat4 projection_matrix = glm::perspective(initial_FoV, 4.0f / 3.0f, 0.1f, 100.0f);
	glm::mat4 MVP = projection_matrix * view_matrix * model_matrix;

	GLuint mvp_matrix_uniform_location;
	GLuint model_matrix_uniform_location;
	GLuint view_matrix_uniform_location;

	double scale = 1.0f;

	// Initial position : on +Z
	glm::vec3 position = glm::vec3(0, 0, 5);

	// Initial horizontal angle : toward -Z
	float horizontal_angle = horizontal_start_angle;

	// Initial vertical angle : none
	float vertical_angle = vertical_start_angle;



	GLFWwindow *window;
	GLuint program;
};

int camera_update_project_matrix(struct camera_context *camera, float ratio)
{
	float FoV = camera->initial_FoV - camera->scale;
	camera->projection_matrix = glm::perspective(glm::radians(FoV), ratio, 0.1f, 100.0f);
	camera_update(camera, 0.0f, 0.0f, 0.0f, 0.0f);
	return 0;
}

struct camera_context *camera_create()
{
	struct camera_context *camera = new camera_context;
	return camera;
}

int camera_init(struct camera_context *camera, GLFWwindow *window, GLuint program)
{

	// Get mouse position
	// glfwGetCursorPos(window, &camera->pre_cursor_pos_x, &camera->pre_cursor_pos_y);
	/*
	glfwSetCursorPos(window,camera->drag_start_pos_x, camera->drag_start_pos_y);
	glfwSetWindowUserPointer(window, camera);
	glfwSetScrollCallback(window, mouse_wheel_scroll_callback);
	glfwSetCursorPosCallback(window, mouse_cursor_pos_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	*/
	float FoV = camera->initial_FoV - camera->scale;
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	camera->projection_matrix = glm::perspective(glm::radians(FoV), float(width) / float(height), 0.1f, 100.0f);

	// Get a handle for our "MVP" uniform
	camera->mvp_matrix_uniform_location = glGetUniformLocation(program, "MVP");
	camera->model_matrix_uniform_location = glGetUniformLocation(program, "M");
	camera->view_matrix_uniform_location = glGetUniformLocation(program, "V");
	camera->window = window;
	camera->program = program;
	camera_update(camera, 0.0f, 0.0f, 0.0f, 0.0f);
	return 0;
}

int camera_update(struct camera_context *context, float dx, float dy, float dha, float dva)
{

	context->horizontal_angle += dha;
	context->vertical_angle += dva;
	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(context->vertical_angle) * sin(context->horizontal_angle),
		sin(context->vertical_angle),
		cos(context->vertical_angle) * cos(context->horizontal_angle));

	// Right vector
	glm::vec3 right = glm::vec3(
		sin(context->horizontal_angle - 3.14f / 2.0f),
		0,
		cos(context->horizontal_angle - 3.14f / 2.0f));

	// Up vector
	glm::vec3 up = glm::cross(right, direction);

	float FoV = context->initial_FoV - context->scale;

	// glfwGetCursorPos(window, &camera->pre_cursor_pos_x, &camera->pre_cursor_pos_y);
	int width, height;
	glfwGetWindowSize(context->window, &width, &height);
	// Projection matrix : 45 Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	context->projection_matrix = glm::perspective(glm::radians(FoV), float(width) / float(height), 0.1f, 100.0f);

	context->position += direction * dx;
	context->position += right * dy;

	// Camera matrix
	context->view_matrix = glm::lookAt(
		context->position,			   // Camera is here
		context->position + direction, // and looks here : at the same position, plus "direction"
		up							   // Head is up (set to 0,-1,0 to look upside-down)
	);

	context->MVP = context->projection_matrix * context->view_matrix * context->model_matrix;
	return 0;
}

int camera_scale(struct camera_context *context, float scale)
{
	context->scale = scale;
	camera_update(context, 0.0f, 0.0f, 0.0f, 0.0f);
	return 0;
}

int camera_apply(struct camera_context *context)
{
	// Send our transformation to the currently bound shader, in the "MVP" uniformV
	glUniformMatrix4fv(context->mvp_matrix_uniform_location, 1, GL_FALSE, &context->MVP[0][0]);

	glUniformMatrix4fv(context->model_matrix_uniform_location, 1, GL_FALSE, &context->model_matrix[0][0]);

	glUniformMatrix4fv(context->view_matrix_uniform_location, 1, GL_FALSE, &context->view_matrix[0][0]);
	return 0;
}

int camera_destory(struct camera_context *context)
{
	if (nullptr != context)
	{
		delete context;
	}
	return 0;
}
