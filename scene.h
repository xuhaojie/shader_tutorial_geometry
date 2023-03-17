
struct SceneContext {
	GLuint vbo;
	GLuint vao;
	GLuint program;
};
int setup_scene(SceneContext* scene_context, const char* vertexShader, const char* fragmentShader);
int destroy_scene(SceneContext* scene_context);
int render_scene(GLFWwindow* window, SceneContext* scene_context) ;