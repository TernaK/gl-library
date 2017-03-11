#version 330 core

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
  
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  
  float Kc;	//constant
  float Kl;	//linear
  float Kq;	//quadratic
};

in v_data
{
  vec3 f_Position;
  vec3 f_Normal;
} fs_in;

uniform vec3 eyePosition;
uniform Material material;/* set by Material */
uniform Light light;/* set by Light */

out vec4 color;

void main()
{
  /* ambient */
  vec3 ambient = light.ambient * material.ambient;
  
  /* diffuse */
  vec3 normal = normalize(fs_in.f_Normal);
  vec3 lightDirection = normalize(light.position - fs_in.f_Position);
  float diffuseStrength = max(dot(lightDirection, normal), 0.0);
  vec3 diffuse = light.diffuse * (diffuseStrength * material.diffuse);
  
  /* specular */
  vec3 toEye = normalize(eyePosition - fs_in.f_Position);
  vec3 reflection = reflect(-lightDirection, normal);
  float specularStrength = pow(max(dot(reflection, toEye), 0.0), material.shininess);
  vec3 specular = light.specular * (specularStrength * material.specular);
  
  /* attenuation */
  float r = length(light.position - fs_in.f_Position);
  float attenuation = 1/(light.Kc + light.Kl * r + light.Kq * r * r );

  color = vec4( (ambient + (diffuse + specular) * attenuation), 1.0f);
}
