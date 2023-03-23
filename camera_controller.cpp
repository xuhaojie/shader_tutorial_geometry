#include "camera_controller.h"
#include "camera.h"
struct camera_controller{
    const float move_speed = 3.0f; // 3 units / second
    const float mouse_speed = 0.001; 
 
    struct camera_context* camera;
	GLFWwindow *window;
	double last_time = 0;
	bool draging = false;
   
	double drag_start_pos_x = 0.0f;
	double drag_start_pos_y = 0.0f;

	float scale = 1.0f;
};

struct camera_controller *camera_controller_create()
{
	struct camera_controller *controller = new camera_controller;
	return controller;
}

static void mouse_wheel_scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
	void *p = glfwGetWindowUserPointer(window);
	struct camera_controller *controller = (struct camera_controller *)p;
	struct camera_context *camera = controller->camera;
	if (yoffset > 0)
	{
		controller->scale += 1.0f;
		//camera->scale += 1.0f;
	}
	else
	{
		controller->scale -= 1.0f;
		//camera->scale -= 1.0f;
	}
	camera_scale(camera, controller->scale);
	camera_update(camera, 0.0f, 0.0f, 0.0f, 0.0f);
}

static void mouse_cursor_pos_callback(GLFWwindow *window, double xpos, double ypos)
{
	void *p = glfwGetWindowUserPointer(window);
	struct camera_controller *controller = (struct camera_controller *)p;
	struct camera_context *camera = controller->camera;
	if (controller->draging)
	{
		// Compute new orientation
		double dha = controller->mouse_speed * float(xpos - controller->drag_start_pos_x);
		controller->drag_start_pos_x = xpos;
		double dva = controller->mouse_speed * float(ypos - controller->drag_start_pos_y);
		controller->drag_start_pos_y = ypos;	
		camera_update(camera, 0.0f, 0.0f, dha, dva);
	}
	else
	{
		// camera->pre_cursor_pos_x = xpos;
		// camera->pre_cursor_pos_y = ypos;
	}
}


static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
	void *p = glfwGetWindowUserPointer(window);
	struct camera_controller *controller = (struct camera_controller *)p;
	struct camera_context *camera = controller->camera;
	if (controller->draging)
	{
		if (action == GLFW_RELEASE)
		{
			controller->draging = false;
			double xpos;
			double ypos;
			glfwGetCursorPos(window, &xpos, &ypos);
			//camera->horizontal_start_angle += camera->mouse_speed * (xpos - camera->drag_start_pos_x);
			//camera->vertical_start_angle += camera->mouse_speed * (ypos - camera->drag_start_pos_y);
		}
	}
	else
	{
		if (action == GLFW_PRESS)
		{
			controller->draging = true;
			glfwGetCursorPos(window, &controller->drag_start_pos_x, &controller->drag_start_pos_y);
		}
	}
}

int camera_controller_init(struct camera_controller *controller, GLFWwindow *window, struct camera_context *camera)
{

	controller->last_time = glfwGetTime();
	controller->window = window;	
	controller->camera = camera;
	controller->draging = false;
	controller->drag_start_pos_x = 0;
	controller->drag_start_pos_y = 0;

	// Get mouse position
	// glfwGetCursorPos(window, &camera->pre_cursor_pos_x, &camera->pre_cursor_pos_y);
	glfwSetCursorPos(window, controller->drag_start_pos_x, controller->drag_start_pos_y);
	glfwSetWindowUserPointer(window, controller);
	glfwSetScrollCallback(window, mouse_wheel_scroll_callback);
	glfwSetCursorPosCallback(window, mouse_cursor_pos_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	
	return 0;
}

void camera_controller_move_forward(struct camera_controller* controller, float delta){
	const float timeValue = glfwGetTime();
	const float deltaTime = float(timeValue - controller->last_time);
	controller->last_time = timeValue;
}

void camera_controller_strafe(struct camera_controller* controller, float delta){

}

void camera_controller_rotate(struct camera_controller* controller, float hor_delta, float ver_delta){

}

int camera_controller_handle_inputs(struct camera_controller *controller)
{

	const float timeValue = glfwGetTime();
	const float deltaTime = float(timeValue - controller->last_time);
	controller->last_time = timeValue;

	// Move forward
	if (glfwGetKey(controller->window, GLFW_KEY_W) == GLFW_PRESS)
	{
		// context->position += direction * deltaTime * context->move_speed;
		camera_update(controller->camera, deltaTime * controller->move_speed, 0.0f, 0.0f, 0.0f);
	}
	// Move backward
	if (glfwGetKey(controller->window, GLFW_KEY_S) == GLFW_PRESS)
	{
		// context->position -= direction * deltaTime * context->move_speed;
		camera_update(controller->camera, -deltaTime * controller->move_speed, 0.0f, 0.0f, 0.0f);
	}
	// Strafe right
	if (glfwGetKey(controller->window, GLFW_KEY_A) == GLFW_PRESS)
	{
		// context->position += right * deltaTime * context->move_speed;
		camera_update(controller->camera, 0.0f, -deltaTime * controller->move_speed, 0.0f, 0.0f);
	}
	// Strafe left
	if (glfwGetKey(controller->window, GLFW_KEY_D) == GLFW_PRESS)
	{
		// context->position -= right * deltaTime * context->move_speed;
		camera_update(controller->camera, 0.0f, deltaTime * controller->move_speed, 0.0f, 0.0f);
	}
	return 0;
}

int camera_controller_destory(struct camera_controller *controller)
{
	if (nullptr != controller)
	{
		delete controller;
	}
	return 0;
}


