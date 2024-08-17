#version 330 core

struct Material {
	vec3 ambient;
	vec3 diffuse; //漫射光
	vec3 specular;
	float shininess;
};

struct Light {
	vec3 position;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

//uniform sampler2D texture1;
//uniform sampler2D texture2;

//uniform float mixVal;

uniform Material material;
uniform Light light;

uniform vec3 viewPos;

void main() {
	//FragColor = vec4(1.0f, 0.2f, 0.6f, 1.0f);
	//FragColor = vec4(ourColor, 1.0);
	//FragColor = vec4(ourColor, 1.0f) * texture(texture1, TexCoord);
	//FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), mixVal);

	// Ambient
	vec3 ambient = light.ambient * material.ambient;
	
	// Diffuse - Frag Pos
	// Diffuse where the material is
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * material.diffuse);
	
	// Specular - View Pos
	// Specular where the viewer is 以及当你越靠近光源时 光就会越聚集的效果
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess * 128); //128是个magic number
	vec3 specular = light.specular * (spec * material.specular);

	FragColor = vec4(vec3(ambient + diffuse + specular), 1.0);

}