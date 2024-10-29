#version 400 core

layout (location=0) in vec3 vertexPos;
layout (location=1) in vec3 vertexColor;
layout (location=2) in vec2 vertexTexCoord;
layout (location=3) in vec3 vertexNormal;

out vec3 fragmentColor;
out vec2 fragmentTexCoord;

uniform mat4 Transform;
uniform mat4 View;
uniform mat4 Projection;

void main() {
	gl_Position = Projection * View * Transform * vec4(vertexPos, 1.0);

	fragmentColor = vertexColor;
	fragmentTexCoord = vertexTexCoord;
}
