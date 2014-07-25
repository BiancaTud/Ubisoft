#version 400
in vec2 texture_coord;
uniform sampler2D basic_texture;
out vec4 frag_colour;
void main () {
	vec4 texel = texture2D (basic_texture, texture_coord);
	frag_colour = texel;
}