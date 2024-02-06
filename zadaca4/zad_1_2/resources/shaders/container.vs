#version 330 core
layout(location=0) in vec3 in_Position;
layout(location=1) in vec3 in_Normal;
layout(location=2) in vec2 in_texCoord;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;
// transformacija pogleda
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main() 
{ 
	Normal= mat3(transpose(inverse(model))) * in_Normal;
	FragPos= vec3(model * vec4(in_Position,1.0));
	gl_Position= projection* view* vec4(FragPos,1);
	TexCoord= in_texCoord;
	};