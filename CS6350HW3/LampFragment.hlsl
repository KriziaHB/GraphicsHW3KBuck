#version 330 core

out vec4 color;
uniform vec3 colorChanged;

void main() {
	color = vec4(colorChanged, 1.0f);
}
//Previously lamp.frag 

