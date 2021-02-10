#version 330 core

in vec2 TexCoord;
in vec3 blockColor;

uniform sampler2D dirt;

void main() {
	gl_FragColor = vec4(blockColor, 1.0f);
//	gl_FragColor = texture(dirt, TexCoord);
}
