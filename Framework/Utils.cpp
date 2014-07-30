#include "common.h"
#include "Utils.h"
#include "stb_image.h"

char * LoadFileInMemory(const char *filename)
{
	int size = 0;
	char *buffer = NULL;
	FILE *f = fopen(filename, "rb");
	if (f == NULL)
	{
		return NULL; 
	}
	fseek(f, 0, SEEK_END);
	size = ftell(f);
	fseek(f, 0, SEEK_SET);
	buffer = new char[size + 1];
	if (size != fread(buffer, sizeof(char), size, f))
	{
		delete [] buffer;
	}
	fclose(f);
	buffer[size] = 0;
	return buffer;
}

void FlipTexture(unsigned char* image_data,int x,int y , int n)
{
	//flip texture
	int width_in_bytes = x * 4;
	unsigned char *top = NULL;
	unsigned char *bottom = NULL;
	unsigned char temp = 0;
	int half_height = y / 2;

	for (int row = 0; row < half_height; row++) {
		top = image_data + row * width_in_bytes;
		bottom = image_data + (y - row - 1) * width_in_bytes;
		for (int col = 0; col < width_in_bytes; col++) {
			temp = *top;
			*top = *bottom;
			*bottom = temp;
			top++;
			bottom++;
		}
	}
}

int LoadTexture(const char* filename,int &width,int &height)
{
	int x, y, n;
	int force_channels = 4;
	unsigned char* image_data = stbi_load(filename, &x, &y, &n, force_channels);
	if (!image_data) {
		fprintf(stderr, "ERROR: could not load %s\n", "../data/frunza.png");
	}
	width = x;
	height = y;
	// NPOT check
	if ((x & (x - 1)) != 0 || (y & (y - 1)) != 0) {
		fprintf(
			stderr, "WARNING: texture %s is not power-of-2 dimensions\n", "../data/frunza.png"
			);
	}
	FlipTexture(image_data, x, y, n);

	unsigned int tex = 0;
	glGenTextures(1, &tex);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		x,
		y,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		image_data
		);

	free(image_data);
	return tex;
}

//functie pentru afisare fps
void _update_fps_counter(GLFWwindow* window) {

	static double previous_seconds = glfwGetTime();
	static int frame_count;
	double current_seconds = glfwGetTime();
	double elapsed_seconds = current_seconds - previous_seconds;

	if (elapsed_seconds > 0.25) {
		previous_seconds = current_seconds;
		double fps = (double)frame_count / elapsed_seconds;
		char tmp[128];

		sprintf(tmp, "opengl - > FPS: %.2f", fps);

		glfwSetWindowTitle(window, tmp);
		frame_count = 0;
	}

	frame_count++;
}