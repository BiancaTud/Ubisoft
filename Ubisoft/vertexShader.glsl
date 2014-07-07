#version 400 // 150 daca nu e suportat opengl 4.x dar e suportat 3.3
layout(location = 0) in vec3 vertex_position;	
layout(location = 1) in vec2 texture_coordinates;
out vec2 texture_coord;

uniform mat4 u_transfMatrix; 
uniform mat4 u_proj_matrix;

void main(){
    texture_coord = texture_coordinates;
    vec4 vertex_post =   /*u_transfMatrix **/ vec4 (vertex_position, 1.0);
    gl_Position = /* u_proj_matrix **/ vec4(vertex_post.xyz,1.0) ;
}
