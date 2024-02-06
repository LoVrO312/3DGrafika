#version 330 core
// Blinn-Phong Fragment Shader 
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

// Blinn-Phong koeficijenti
struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light
{ 
	vec3 position;
	// intenziteti za pojedinu komponentu
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform vec3 viewPos;
uniform Light light;
uniform Material material;

// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	// ambient color 
	vec3 ambient= light.ambient* material.ambient;
	// diffuse
	vec3 norm= normalize(Normal);
	vec3 lightDir= normalize(light.position-FragPos);
	vec3 diffuse= light.diffuse* material.diffuse* max(dot(norm,lightDir),0.0);
	// specular
	vec3 viewDir= normalize(viewPos-FragPos);
	vec3 reflectDir= reflect(-lightDir,norm);
	vec3 specular= light.specular* material.specular* pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
	// result
	vec3 result= ambient+ diffuse+ specular;
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2) * vec4(result,1.0);
}