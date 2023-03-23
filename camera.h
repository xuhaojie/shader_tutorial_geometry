#include <GL/glew.h>
#include <GLFW/glfw3.h>
struct camera_context;

struct camera_context* camera_create();
int camera_init(struct camera_context* camera, GLFWwindow* window, GLuint program);
int camera_update_project_matrix(struct camera_context* camera, float ratio);
int camera_update(struct camera_context* context, float dx, float dy, float dha, float dva);
int camera_scale(struct camera_context* camera, float scale);
int camera_handle_inputs(struct camera_context* context);
int camera_apply(struct camera_context* context);
int camera_destory(struct camera_context* context);
