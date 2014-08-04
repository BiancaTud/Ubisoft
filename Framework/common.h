#ifndef ___H_COMMON_H
#define ___H_COMMON_H


#include <GL/glew.h>
#include <assert.h>
#define GLFW_DLL

/*
          Aces ta e           
         ste fisierul          
        comun in care s        
       e afla toate info       
r     matiile comune tutu     r
 o  r celorlalte fisiere.Nu    
 e marebranza i n el dar scriu 
  mai mult aic   i ca sa iasa  
    bine mus       tata ASCI   
     II...           .....     

*/

#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<iostream>
#include<vector>


using namespace std;

extern	int g_gl_width;
extern	int g_gl_height;
extern	GLFWwindow* window;

#define SCREEN_LEFT -4.0f
#define SCREEN_RIGHT 4.0f
#define SCREEN_TOP	 4.0f
#define SCREEN_BOTTOM -4.0f

#define background -2
#define player_proj -1
#define player 0
#define enemy 1
#define enemy_proj 2
#define mini 3
#define textLife 4


enum VERTEX_ATRIBUTES
{
	POSITION = 0,
	TEXTURE_COORDINATE_0 = 1
};


#endif //___H_COMMON_H


/*
            __
           / _)
    .-^^^-/ /
 __/       /
<__.|_|-|_|           Cine vede dinozaurul asta are un punct in plus

*/
