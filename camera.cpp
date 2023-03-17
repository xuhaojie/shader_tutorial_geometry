#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "camera.h"
struct camera_context {
	GLuint mvp_uniform_location;

	glm::mat4 view_matrix;
	glm::mat4 projection_matrix;
	glm::mat4 MVP;

	double scale = 1.0f;

	double pre_cursor_pos_x = 0.0f;
	double pre_cursor_pos_y = 0.0f;

	// Initial position : on +Z
	glm::vec3 position = glm::vec3(0, 0, 5);

	// Initial horizontal angle : toward -Z
	float horizontal_angle = 3.14f;
	float horizontal_start_angle = 3.14f;
	// Initial vertical angle : none
	float vertical_angle = 0.0f;
	float vertical_start_angle = 0.0f;

	// Initial Field of View
	const float initial_FoV = 45.0f;

	const float move_speed = 3.0f; // 3 units / second

	const float mouse_speed = 0.001;

	double last_time = 0;
	bool draging = false;

};

static void mouse_wheel_scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	void* p= glfwGetWindowUserPointer(window);
	struct camera_context* camera = (struct camera_context*)p;
	if (yoffset > 0) {
		camera->scale += 1.0f;
	}
	else
	{
		camera->scale -= 1.0f;
	}
}

static void mouse_cursor_pos_callback (GLFWwindow *window, double xpos, double ypos){
	void* p= glfwGetWindowUserPointer(window);
	struct camera_context* camera = (struct camera_context*)p;
	if(camera->draging){
		// Compute new orientation
		camera->horizontal_angle = camera->horizontal_start_angle + camera->mouse_speed * float(xpos - camera->pre_cursor_pos_x);
		camera->vertical_angle =  camera->vertical_start_angle + camera->mouse_speed * float(ypos - camera->pre_cursor_pos_y);

	} else{
		//camera->pre_cursor_pos_x = xpos;
		//camera->pre_cursor_pos_y = ypos;
	}
}

static void mouse_button_callback (GLFWwindow *window,  int button, int action, int mods){
	void* p= glfwGetWindowUserPointer(window);
	struct camera_context* camera = (struct camera_context*)p;
	if(camera->draging){
		if(action == GLFW_RELEASE){
			camera->draging = false;
		}

	}else{
		if(action == GLFW_PRESS ){
			camera->draging = true;
			glfwGetCursorPos(window, &camera->pre_cursor_pos_x, &camera->pre_cursor_pos_y);
		}
	}
}


struct camera_context* camera_create() {
	struct camera_context* camera = new camera_context;
	return camera;
}

int camera_init(struct camera_context* camera, GLFWwindow* window, GLuint program) {

	// Get mouse position
//	int xpos, ypos;
//	glfwGetMousePos(&xpos, &ypos);

	// Reset mouse position for next frame
//	glfwSetMousePos(1024/2, 768/2);
	int width = 1024;
	int height = 768;
	glfwGetWindowSize(window, &width, &height);

//	camera->pre_cursor_pos_x = width / 2.0;
//	camera->pre_cursor_pos_x = height / 2.0;
	camera->last_time = glfwGetTime();
	
	// Get mouse position
	//glfwGetCursorPos(window, &camera->pre_cursor_pos_x, &camera->pre_cursor_pos_y);
	glfwSetCursorPos(window, camera->pre_cursor_pos_x, camera->pre_cursor_pos_y);
	glfwSetWindowUserPointer(window, camera);
	glfwSetScrollCallback(window, mouse_wheel_scroll_callback);
	glfwSetCursorPosCallback(window, mouse_cursor_pos_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
   // Get a handle for our "MVP" uniform
	camera->mvp_uniform_location = glGetUniformLocation(program, "MVP");

	// Projection matrix : 45 Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 projection_matrix = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	// Or, for an ortho camera :
	//glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates

	// Camera matrix
	glm::mat4 view_matrix = glm::lookAt(
		glm::vec3(4, 3, 3), // Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);

	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 model_matrix = glm::mat4(1.0f);

	// Our ModelViewProjection : multiplication of our 3 matrices
	camera->MVP = projection_matrix * view_matrix * model_matrix; // Remember, matrix multiplication is the other way around

	return 0;
}

int camera_update(struct camera_context* context, GLFWwindow* window, GLuint program){

	const float timeValue = glfwGetTime();
	const float deltaTime = float(timeValue - context->last_time);
	context->last_time = timeValue;

	// Get mouse position
	double cur_cursor_pos_x, cur_cursor_pos_y;
	glfwGetCursorPos(window, &cur_cursor_pos_x, &cur_cursor_pos_y);


	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(context->vertical_angle) * sin(context->horizontal_angle),
		sin(context->vertical_angle),
		cos(context->vertical_angle) * cos(context->horizontal_angle)
	);

	// Right vector
	glm::vec3 right = glm::vec3(
		sin(context->horizontal_angle - 3.14f / 2.0f),
		0,
		cos(context->horizontal_angle - 3.14f / 2.0f)
	);

	// Up vector
	glm::vec3 up = glm::cross(right, direction);

	// Move forward
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		context->position += direction * deltaTime * context->move_speed;
	}
	// Move backward
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		context->position -= direction * deltaTime * context->move_speed;
	}
	// Strafe right
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		context->position += right * deltaTime * context->move_speed;
	}
	// Strafe left
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		context->position -= right * deltaTime * context->move_speed;
	}

	float FoV = context->initial_FoV - context->scale;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

	// Projection matrix : 45 Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	context->projection_matrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);

	// Camera matrix
	context->view_matrix = glm::lookAt(
		context->position,           // Camera is here
		context->position + direction, // and looks here : at the same position, plus "direction"
		up                  // Head is up (set to 0,-1,0 to look upside-down)
	);

	glm::mat4 ModelMatrix = glm::mat4(1.0);

	context->MVP = context->projection_matrix * context->view_matrix * ModelMatrix;

	return 0;
}

int camera_apply(struct camera_context* context){
	// Send our transformation to the currently bound shader, in the "MVP" uniform
	glUniformMatrix4fv(context->mvp_uniform_location, 1, GL_FALSE, &context->MVP[0][0]);
	return 0;
}