#shader vertex
#version 330 core 

layout(location = 0) in vec3 Pos;
layout(location = 1) in vec3 Normals;

uniform mat4 MVP;
uniform mat3 NormalMatrix;
uniform mat4 Model;

out vec3 FragPos;
out vec3 Norm;

void main()
{

	gl_Position = MVP * vec4(Pos,1.0);
	FragPos = vec3(Model * vec4(Pos, 1.0f));
	Norm =  normalize(NormalMatrix * Normals);

}


#shader fragment
#version 330 core

// in case you don't understand how the fragment shader works rn, basically, the vertex shader just runs for each vertex, and then
// passes it all to the fragment shader. The fragment shader must fill in all the pixels, and cannot fill any other out of this 
// triangle. However, by passing the postion of a vertex to the fragment shaer, you can calculate the effect of light on that particular
// triangle. But you must not transform the postion of the verex by anything, otherwise, you'd be using a different position instead of the 
// real verticie's position.

in vec3 FragPos, Norm;

uniform vec3 cameraPos;

out vec4 fragColour;

struct Material
{
	vec3 diffuse;
	vec3 ambient;
	vec3 specular;
	
	float shininess;

	// so these values which the struct holds basically represent the colours which each type of light will cause the object 
	// to reflect
	// so we can define different colours we wanna reflect for each type of light
	// That's also why we remove the object colour at the bottom - the one that we used to times the light colour and the
	// other types of light by to get the final output.
};

struct Light {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Light light;
uniform Material material;

void main() 
{
	// ambient
	vec3 ambienceLight = light.ambient * material.ambient;


	// diffuse
	vec3 LightDirection = normalize(light.position - FragPos); //means a vector pointing from fragpos to light
	float diff = max(dot(LightDirection, Norm), 0.0f);
	// diffuse light
	vec3 diffuseLight = diff * material.diffuse * light.diffuse;


	// specular
	float specularStrength = 0.5f;
	vec3 viewerPos = normalize(cameraPos - FragPos);
	vec3 reflectedRay = normalize(reflect(-light.position, Norm));
	float spec = pow(max(dot(reflectedRay, viewerPos), 0.0f), material.shininess);


	// specular light
	vec3 specularLight = spec * material.specular * light.ambient;


	fragColour = vec4((diffuseLight + specularLight + ambienceLight), 1.0f);


}


