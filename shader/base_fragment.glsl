#version 330 core

in vec3 vertex_color;
in vec2 texture_coord;

uniform float weight;
uniform sampler2D texture_1;
uniform sampler2D texture_2;

out vec4 color;

void main()
{
	vec4 color_1 = texture(texture_1, texture_coord);
	vec4 color_2 = texture(texture_2, vec2(texture_coord.s, texture_coord.t));
	color = mix(color_1, color_2, weight);
}