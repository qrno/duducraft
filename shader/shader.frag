#version 330 core

in vec2 TexCoord;
in vec3 blockColor;

uniform sampler2D dirt;
uniform sampler2D awesome;

void main() {
	gl_FragColor = vec4(blockColor, 1.0f);
//	gl_FragColor = texture(awesome, TexCoord);
}
