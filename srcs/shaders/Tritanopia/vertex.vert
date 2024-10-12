#version 400 core

layout (location=0) in vec3 vertexPos;
layout (location=1) in vec3 vertexColor;

out vec3 fragmentColor;

uniform mat4 Transform;

void main() {
	gl_Position = Transform * vec4(vertexPos, 1.0);
	fragmentColor = vertexColor;
}
