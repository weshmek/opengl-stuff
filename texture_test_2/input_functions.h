#ifndef __INPUT_FUNCTIONS_H__
#define __INPUT_FUNCTIONS_H__
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
extern float rotation_matrix_x[][4];
extern float rotation_matrix_y[][4];
extern float rotation_matrix_z[][4];
extern GLuint rot_matrix_x_uniform;
extern GLuint rot_matrix_y_uniform;
extern GLuint rot_matrix_z_uniform;

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
void cursor_position_callback(GLFWwindow *window, double xpos, double ypos);
#endif
