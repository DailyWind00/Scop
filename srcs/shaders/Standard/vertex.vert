#version 400 core

layout (location=0) in vec3 vertexPos;
layout (location=1) in vec3 vertexColor;
layout (location=2) in vec2 vertexTexCoord;

out vec3 fragmentColor;

uniform mat4 Transform;
uniform mat4 View;
uniform mat4 Projection;

uniform float RenderTexture;
uniform sampler2D Texture;

void main() {
	gl_Position = Projection * View * Transform * vec4(vertexPos, 1.0);

	float RenderTex = clamp(RenderTexture, 0.0, 1.0);

	fragmentColor = mix(vertexColor, texture(Texture, vertexTexCoord).rgb, RenderTex);
}
