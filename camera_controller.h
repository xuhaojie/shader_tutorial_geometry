struct camera_context;
struct camera_controller;
struct GLFWwindow;

struct camera_controller *camera_controller_create();


int camera_controller_init(struct camera_controller *controller, GLFWwindow *window, struct camera_context *camera);

void camera_controller_move_forward(struct camera_controller* controller, float delta);

void camera_controller_strafe(struct camera_controller* controller, float delta);

void camera_controller_rotate(struct camera_controller* controller, float hor_delta, float ver_delta);

int camera_controller_handle_inputs(struct camera_controller *controller);

int camera_controller_destory(struct camera_controller *controller);


