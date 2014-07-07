// Ubisoft.cpp : Defines the entry point for the console application.



#include "stdafx.h"
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW32/glfw3.h> // GLFW helper library
#include <math.h>
#include <iostream>
#include <stdlib.h>
#include <crtdbg.h>
#include <stb_image\stb_image.cpp>

#define _CRTDBG_MAP_ALLOC
#define pi 3.1415926
using namespace std;


#ifdef _DEBUG
   #ifndef DBG_NEW
      #define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
      #define new DBG_NEW
   #endif
#endif  // _DEBUG


// functie banala de incarcat continutul unui fisier intr-un buffer
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
		delete[] buffer;
	}
	fclose(f);
	buffer[size] = 0;
	return buffer;
}


// flip image
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



int main () {

	// Initializare (se creeaza contextul)
	if (!glfwInit ()) {
		fprintf (stderr, "ERROR: could not start GLFW3\n");
		return 1;
	}

	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	// Se creeaza fereastra
	GLFWwindow* window = glfwCreateWindow (640, 480, "Workshop1", NULL, NULL);
	if (!window) {
    // nu am reusit sa facem fereastra, oprim totul si dam mesaj de eroare
		printf ( "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return 1;
  }

	// Atasam contextul de fereastra
	glfwMakeContextCurrent (window);
                                  
	// Pornit extension handler-ul
	glewInit ();

	// Vedem versiunile
	const GLubyte* renderer = glGetString (GL_RENDERER); //renderer string
	const GLubyte* version = glGetString (GL_VERSION); // version string
	printf ("Renderer: %s\n", renderer);
	printf ("OpenGL version supported %s\n", version);

	//load shaders
	const char * vertex_shader = LoadFileInMemory("vertexShader.glsl");
	const char * fragment_shader = LoadFileInMemory("pixelShader.glsl");
	const char * fragment_shader2 = LoadFileInMemory("pixelShader2.glsl");

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, NULL);
	glCompileShader(vs);

	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, NULL);
	glCompileShader(fs);

	GLuint fs2 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs2, 1, &fragment_shader2, NULL);
	glCompileShader(fs2);

	GLuint shader_programme = glCreateProgram();
	glAttachShader(shader_programme, fs);
	glAttachShader(shader_programme, vs);
	glLinkProgram(shader_programme);

	GLuint shader_programme2 = glCreateProgram();
	glAttachShader(shader_programme2, fs2);
	glAttachShader(shader_programme2, vs);
	glLinkProgram(shader_programme2);


	delete[] vertex_shader;
	delete[] fragment_shader;
	delete[] fragment_shader2;
	_crtBreakAlloc = 18;

 
  /*
	// buffer cu vertecsi in RAM 
	float vertex_buffer_triangle1[] = {
	  -0.5f, 0.7f, 0.0f,
	  -0.3f, 0.3f, 0.0f,
	  -0.7f, 0.3f, 0.0f
  };

	//triangle 2
	float vertex_buffer_triangle2[] = {
	  0.6f, 0.7f, 0.0f,
	  0.4f, 0.3f, 0.0f,
	  0.8f, 0.3f, 0.0f
  };

	//array for square -> simple vbo
	float vertex_buffer_square[] = {
	 -0.2f, -0.2f, 0.0f,
	  0.2f, -0.2f, 0.0f,
	  0.2f, 0.2f, 0.0f,
	 -0.2f, -0.2f, 0.0f,
	  0.2f, 0.2f, 0.0f,
	 -0.2f, 0.2f, 0.0f
  };
  */

   /*
	int nr;
	nr=10;// numar vertecsi
	float vertex_buffer_circle[1000];
	float angle = 2*(float)pi/nr;

	//coordonatele initiale ale centrului cercului
	float centerx=0.0f;
	float centery=-0.6f;
	float t=0;
	float r=0.2f; // raza cerc
	int i=0;

	//Se vor forma "nr" triunghiuri 
	for(int j = 0;j < nr; j++) {
		//centrul cercului
		vertex_buffer_circle[i++] = centerx; 
		vertex_buffer_circle[i++] = centery; 
		vertex_buffer_circle[i++] = 0;

		//al doilea punct al triunghiului
		vertex_buffer_circle[i++] = centerx + sin(t) * r; 
		vertex_buffer_circle[i++] = centery + cos(t) * r;
		vertex_buffer_circle[i++] = 0;

		//al treilea punct al triunghiului
		vertex_buffer_circle[i++] = centerx + sin(t + angle) * r; 
		vertex_buffer_circle[i++] = centery + cos(t + angle) * r; 
		vertex_buffer_circle[i++] = 0;

		t += angle; 
  }
  */

	//array for square -> index buffer
	float vertex_square[] = {
		0.0f, 0.0f, 0.0f,
        0.3f, 0.0f, 0.0f, 
		0.3f, 0.3f, 0.0f, 
		0.0f, 0.3f, 0.0f
  };

	float uv[] ={
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
  };

	//array of indices for square
	unsigned int indices [] = { 0, 1, 2, 0, 2, 3};

	//load texture
	int x, y, n, force_channels = 4;
	unsigned char* image_data;
	GLuint tex = 0;
	glGenTextures(1, &tex);

	image_data = stbi_load("index.jpg", &x, &y, &n, force_channels);
	FlipTexture(image_data, x, y, n);

	glBindTexture(GL_TEXTURE_2D, tex);
	glActiveTexture(GL_TEXTURE0);
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

	// setam parametri de sampling
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); //ce se intampla cand coordonata nu se inscrie in limite
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); //ce se intampla cand coordonata nu se inscrie in limite
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // setam samplare cu interpolare liniara
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // setam samplare cu interpolare liniara

	//activare transparenta
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	//vertex array object
	GLuint vbo[4], vao[4];  
	glGenVertexArrays(4, &vao[0]);

  /*
	//triangle 1
	vbo[0] = 0;
	glBindVertexArray(vao[0]);
	glGenBuffers(1, &vbo[0]); // generam un buffer 
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); // setam bufferul generat ca bufferul curent 
	glBufferData(GL_ARRAY_BUFFER,  sizeof (vertex_buffer_triangle1), vertex_buffer_triangle1, GL_STATIC_DRAW); //  scriem in bufferul din memoria video informatia din bufferul din memoria RAM
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	//triangle 2
	vbo[1] = 0;
	glBindVertexArray(vao[1]);
	glGenBuffers(1, &vbo[1]); // generam un buffer 
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]); // setam bufferul generat ca bufferul curent 
	glBufferData(GL_ARRAY_BUFFER,  sizeof (vertex_buffer_triangle2), vertex_buffer_triangle2, GL_STATIC_DRAW); 
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    
	//circle
	vbo[3] = 0;
	glBindVertexArray(vao[3]);
	glGenBuffers(1, &vbo[3]); // generam un buffer 
	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]); // setam bufferul generat ca bufferul curent 
	glBufferData(GL_ARRAY_BUFFER,  9 * nr * sizeof(GLfloat), vertex_buffer_circle, GL_STATIC_DRAW); 
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL); 
  */

	//square with index buffer
	vbo[2] = 0;
	glBindVertexArray(vao[2]);
	glGenBuffers(1, &vbo[2]); // generam un buffer 
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]); // setam bufferul generat ca bufferul curent 
	glBufferData(GL_ARRAY_BUFFER,  sizeof(vertex_square), vertex_square, GL_STATIC_DRAW);

	unsigned int tex_buff=0;
    glGenBuffers(1, &tex_buff);
	glBindBuffer(GL_ARRAY_BUFFER, tex_buff);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), uv, GL_STATIC_DRAW);

	GLuint elementbuffer;
	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) , &indices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, tex_buff);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);


	while (!glfwWindowShouldClose(window)) {
		//..... Randare................. 
		// stergem ce s-a desenat anterior
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// spunem ce shader vom folosi pentru desenare
	/*  glUseProgram(shader_programme);
		//facem bind la vertex buffer
		glBindVertexArray(vao[0]);
	   //draw points 0-3 from the currently bound VAO with current in-use shader
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// triangle 2
		glUseProgram(shader_programme2);
		glBindVertexArray(vao[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	  	 //circle  
		glUseProgram(shader_programme);
		glBindVertexArray(vao[3]);
		glDrawArrays(GL_TRIANGLES, 0, 3*nr );	  
	  */

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex);
		glUseProgram(shader_programme);
		glBindVertexArray(vao[2]);
		// Index buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
		glEnableVertexAttribArray(0);
		glDrawElements(
			GL_TRIANGLES,      // mode
			6,    // count
			GL_UNSIGNED_INT,   // type
			NULL         // element array buffer offset
			);

		// facem swap la buffere (Double buffer)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteProgram(shader_programme);
	glDeleteProgram(shader_programme2);
	glDeleteBuffers(4, &vbo[0]);
	glDeleteVertexArrays(4, &vao[4]);
	glfwTerminate();
	return 0;
}