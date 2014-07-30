#ifndef __H_UTILS_
#define __H_UTILS_

#include <time.h>
#include <stdarg.h>


char * LoadFileInMemory(const char *filename);
void FlipTexture(unsigned char* image_data, int x, int y, int n);
int LoadTexture(const char* filename, int &width, int &height);
void _update_fps_counter(GLFWwindow* window);



#endif