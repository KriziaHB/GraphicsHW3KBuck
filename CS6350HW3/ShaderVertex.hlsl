//Previously shader.vert 

#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightPosition;
uniform vec3 lightColor;

uniform int revertNormals;

out vec3 Normal;
out vec3 FragPos;
out vec2 textureCoord;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f);
	FragPos = vec3(model * vec4(position, 1.0f));
	Normal = mat3(transpose(inverse(model))) * normal;
	if (revertNormals == 1) {
		Normal = Normal * (-1);
	}

	textureCoord = vec2(texCoord.x, texCoord.y);
}