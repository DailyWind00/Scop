#version 400 core

layout (location=0) in vec3 vertexPos;
layout (location=1) in vec3 vertexColor;

out vec3 fragmentColor;

uniform mat4 Transform;
uniform mat4 View;
uniform mat4 Projection;

void main() {
	gl_Position = Projection * View * Transform * vec4(vertexPos, 1.0);
	fragmentColor = vertexColor;
}
