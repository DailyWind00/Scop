#version 400 core

in vec3 fragmentColor;

out vec4 screenColor;

void main() {
	float shade = 0.2989 * fragmentColor.r + 0.5870 * fragmentColor.g + 0.1140 * fragmentColor.b;
	screenColor = vec4(vec3(shade), 1.0);
}