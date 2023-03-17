#include <GL/glew.h>
#include <GLFW/glfw3.h>
struct camera_context;

struct camera_context* camera_create();
int camera_init(struct camera_context* camera, GLFWwindow* window, GLuint program);
int camera_update(struct camera_context* context, GLFWwindow* window, GLuint program);
int camera_apply(struct camera_context* context);
int camera_destory(struct camera_context* context);
