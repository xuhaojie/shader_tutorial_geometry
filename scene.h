#include <vector>
#include <glm/glm.hpp>
struct SceneContext {
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals; // Won't be used at the moment.
	GLuint vbo_vertices;
	GLuint vbo_uvs;
	GLuint vbo_normals;
	GLuint vao;
	GLuint program;
};
int setup_scene(SceneContext* scene_context, const char* vertexShader, const char* fragmentShader);
int destroy_scene(SceneContext* scene_context);
int render_scene(GLFWwindow* window, SceneContext* scene_context) ;