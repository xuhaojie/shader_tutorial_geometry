#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "camera.h"
struct camera_context {
	GLuint m_matrixID;
	glm::mat4 m_ViewMatrix;
	glm::mat4 m_ProjectionMatrix;
	glm::mat4 m_MVP;

	double pre_xpos = 0.0f;
	double pre_ypos = 0.0f;

	// Initial position : on +Z
	glm::vec3 m_position = glm::vec3(0, 0, 5);

	// Initial horizontal angle : toward -Z
	float m_horizontalAngle = 3.14f;

	// Initial vertical angle : none
	float m_verticalAngle = 0.0f;

	// Initial Field of View
	const float initialFoV = 45.0f;

	const float speed = 3.0f; // 3 units / second

	const float mouseSpeed = 0.0;

	// glfwGetTime is called only once, the first time this function is called
	double m_lastTime = 0;

	double m_scale = 1.0f;
};

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	void* p= glfwGetWindowUserPointer(window);
	struct camera_context* camera = (struct camera_context*)p;
	if (yoffset > 0) {
		camera->m_scale += 1.0;
	}
	else
	{
		camera->m_scale -= 1.0;
	}
}

struct camera_context* camera_create(GLFWwindow* window, GLuint program) {
	struct camera_context* camera = new camera_context;
    // Get mouse position
	int xpos, ypos;
//	glfwGetMousePos(&xpos, &ypos);

	// Reset mouse position for next frame
//	glfwSetMousePos(1024/2, 768/2);
	camera->m_lastTime = glfwGetTime();
	// Get mouse position
	glfwGetCursorPos(window, &camera->pre_xpos, &camera->pre_ypos);
	glfwSetWindowUserPointer(window, camera);
	glfwSetScrollCallback(window, scroll_callback);
   // Get a handle for our "MVP" uniform
    camera->m_matrixID = glGetUniformLocation(program, "MVP");

    // Projection matrix : 45 Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
    // Or, for an ortho camera :
    //glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates

    // Camera matrix
    // Camera matrix
    glm::mat4 View = glm::lookAt(
        glm::vec3(4, 3, 3), // Camera is at (4,3,3), in World Space
        glm::vec3(0, 0, 0), // and looks at the origin
        glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
    );

    // Model matrix : an identity matrix (model will be at the origin)
    glm::mat4 Model = glm::mat4(1.0f);
    // Our ModelViewProjection : multiplication of our 3 matrices
    camera->m_MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around

	
    return camera;
}


int camera_update(struct camera_context* context, GLFWwindow* window, GLuint program){
    const float timeValue = glfwGetTime();
	const float deltaTime = float(timeValue - context->m_lastTime);
	int width = 1024;
	int height = 768;
	glfwGetWindowSize(window, &width, &height);

	// Get mouse position
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	// Compute new orientation
	context->m_horizontalAngle += context->mouseSpeed * float(context->pre_xpos - xpos);
	context->pre_xpos = xpos;

	context->m_verticalAngle += context->mouseSpeed * float(context->pre_ypos - ypos);
	context->pre_ypos = ypos;

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(context->m_verticalAngle) * sin(context->m_horizontalAngle),
		sin(context->m_verticalAngle),
		cos(context->m_verticalAngle) * cos(context->m_horizontalAngle)
	);

	// Right vector
	glm::vec3 right = glm::vec3(
		sin(context->m_horizontalAngle - 3.14f / 2.0f),
		0,
		cos(context->m_horizontalAngle - 3.14f / 2.0f)
	);

	// Up vector
	glm::vec3 up = glm::cross(right, direction);

	// Move forward
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		context->m_position += direction * deltaTime * context->speed;
	}
	// Move backward
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		context->m_position -= direction * deltaTime * context->speed;
	}
	// Strafe right
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		context->m_position += right * deltaTime * context->speed;
	}
	// Strafe left
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		context->m_position -= right * deltaTime * context->speed;
	}
	float FoV = context->initialFoV - context->m_scale;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

	// Projection matrix : 45 Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	context->m_ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	context->m_ViewMatrix = glm::lookAt(
		context->m_position,           // Camera is here
		context->m_position + direction, // and looks here : at the same position, plus "direction"
		up                  // Head is up (set to 0,-1,0 to look upside-down)
	);

	glm::mat4 ModelMatrix = glm::mat4(1.0);
	context->m_MVP = context->m_ProjectionMatrix * context->m_ViewMatrix * ModelMatrix;



	context->m_lastTime = timeValue;    
    return 0;
}

int camera_apply(struct camera_context* context){
    // Send our transformation to the currently bound shader, 
    // in the "MVP" uniform
    glUniformMatrix4fv(context->m_matrixID, 1, GL_FALSE, &context->m_MVP[0][0]);

    return 0;
}