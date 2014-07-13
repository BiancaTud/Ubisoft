// Ubisoft.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include "SpriteManager.h"


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

/*
	//circle
	vbo[3] = 0;
	glBindVertexArray(vao[3]);
	glGenBuffers(1, &vbo[3]); // generam un buffer 
	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]); // setam bufferul generat ca bufferul curent 
	glBufferData(GL_ARRAY_BUFFER,  9 * nr * sizeof(GLfloat), vertex_buffer_circle, GL_STATIC_DRAW); 
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL); 
  */

	SpriteManager *manag=new SpriteManager(shader_programme, 4, window);
	manag->AddSprite("Ship.png",0);
	manag->AddSprite("Space/Enemies/saucer_blades0000.png",1);
	manag->AddSprite("Space/Enemies/spikey0000.png",2);
	manag->AddSprite("Space/Enemies/slicer0000.png",3);
	
	manag->Init();

	while (!glfwWindowShouldClose(window)) {
		//..... Randare................. 
		// stergem ce s-a desenat anterior
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	/*	
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
		manag->Draw();
		// facem swap la buffere (Double buffer)
		glfwSwapBuffers(window);
		glfwPollEvents();
		//manag->DelSprite(0);
	}

	glDeleteProgram(shader_programme);
	glDeleteProgram(shader_programme2);
	delete manag;
	_CrtSetBreakAlloc(167);
	glfwTerminate();
	return 0;
}