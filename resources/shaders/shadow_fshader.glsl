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
  vec3 f_Normal;
  vec3 f_Position;
  vec3 f_PositionInLightSpace;
} fs_in;

uniform sampler2D depthTexture;

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
  
  // frag position normalized
  vec3 fragInLightSpace = fs_in.f_PositionInLightSpace * 0.5 + 0.5;
  
  // be extra careful
  float fragDepthInLightSpace = clamp(fragInLightSpace.z, 0, 1.0);
  float closestDepthInLightView = texture(depthTexture, fragInLightSpace.xy).r;//the first channel
  
  float epsilon= 0.001;
  if((fragDepthInLightSpace - closestDepthInLightView) > epsilon) {
    diffuse = vec3(0.0f);
    specular = vec3(0.0f);
  }

  color = vec4( (ambient + (diffuse + specular) * attenuation), 1.0f);
}
