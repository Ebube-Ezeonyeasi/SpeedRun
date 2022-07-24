#shader vertex
#version 330

layout(location = 0) in vec3 pos;

uniform mat4 transform;

void main() 
{

	gl_Position = transform * vec4(pos, 1.0f);

}

#shader fragment
#version 330

uniform vec3 lightColour;

out vec4 fragColour;

void main()
{

	fragColour = vec4(lightColour, 1.0f);

}