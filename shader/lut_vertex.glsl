#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 text_coord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vertex_color;
out vec2 texture_coord;

void main()
{
	vertex_color = color;
	texture_coord = text_coord;
	gl_Position = projection * view * model * vec4(position, 1.0f);
}