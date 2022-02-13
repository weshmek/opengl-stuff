#include "input_functions.h"
#include <math.h>

int drag = 0;
void cursor_position_callback(GLFWwindow *window, double xpos, double ypos) {
	static double prev_xpos = 0.0;
	static double prev_ypos = 0.0;
	static double theta = 0.0;
	static double phi = 0.0;
	double dx;
	double dy;
	int width;
	int height;

	if (!drag) goto end;
	glfwGetWindowSize(window, &width, &height);
	dx = prev_xpos - xpos;
	dy = prev_ypos - ypos;
	phi += 2 * M_PI * dy / height;
	theta += 2 * M_PI * dx / width;
	rotation_matrix_x[1][1] = (float) cos(phi);
	rotation_matrix_x[1][2] = (float) -sin(phi);
	rotation_matrix_x[2][1] = (float) sin(phi);
	rotation_matrix_x[2][2] = (float) cos(phi);

	rotation_matrix_y[0][0] = (float) cos(theta);
	rotation_matrix_y[0][2] = (float) sin(theta);
	rotation_matrix_y[2][0] = (float) -sin(theta);
	rotation_matrix_y[2][2] = (float) cos(theta);

end:
	prev_xpos = xpos;
	prev_ypos = ypos;
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS) {
		drag = 1;
	} else if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_RELEASE) {
		drag = 0;
	}
}
