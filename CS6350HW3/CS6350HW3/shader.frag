#version 330 core 

in vec3 FragPos; 
in vec3 Normal; 
in vec2 textureCoord; 

out vec4 color; 
uniform vec3 lightPosition; 
uniform vec3 lightColor; 
uniform vec3 camPos; 
uniform vec3 cubeColor; 
uniform int revertNormals; 

uniform sampler2D theTexture; 

void main () {

	// 0.004f is attenuation constant 
	//USE THIS ONE OR FIND ANOTHER ATTENUATION FORMULA 
	vec3 lightAttenuated = lightColor * (1.0f / (1.0f + 0.004f * pow(distance(FragPos, lightPosition), 2.0f))); 
	
	//ambient 
	float ambientStrength = 0.1f; 
	vec3 ambient = ambientStrength * lightColor; 

	//diffuse 
	float diffuseStrength = 0.9f; 
	vec3 norm = normalize(Normal); 
	vec3 lightDir = normalize(lightPosition - FragPos); 
	float diff = max(dot(normal, lightDir), 0.0); 
	vec3 diffuse = diff * lightColor * diffuseStrength; 

	//specular 
	float specularStrength = 0.5f; 
	vec3 viewDir = normalize(camPos - FragPos); 
	vec3 reflectDir = reflect(-lightDir, norm); 
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32); //32 == phong component 
	vec3 specular = specularStrength * spec * lightColor; 

	vec4 texCala = texture(theTexture, textureCoord); 
	vec3 combinationOfColors; 

//[KHB] trying to get constant color
//	if (texCala.a < 0.1 || revertNormals == 1) {
		combinationOfColors = cubeColor; 
//	}
//	else {
//		combinationOfColors = texCala.rgb; 
//	}

	vec3 result = (ambient + lightAttenuated * (diffuse + specular)) * combinationOfColors; 
	color = vec4(result, 1.0f); 

}