#version 400 core

in vec3 fragmentColor;

out vec4 screenColor;

uniform float Time;

void main() {
	// screenColor = vec4(fragmentColor, 1.0);
	float Red = (sin(Time) / 2.0f) + 0.5f;
	float Green = (sin(Time * 2.0f) / 2.0f) + 0.5f;
	float Blue = (sin(Time * 3.0f) / 2.0f) + 0.5f;
	screenColor = vec4(Red, Green, Blue, 1.0f);
}