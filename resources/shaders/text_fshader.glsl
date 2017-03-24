#version 330 core

in v_data
{
  vec3 f_Position;
  vec2 f_Textcoords;
} fs_in;

uniform sampler2D GLTextNodeTexture;

out vec4 color;

void main()
{
  /* ambient */
  // vec3 ambient = light.ambient * material.ambient;
  
  // /* diffuse */
  // vec3 normal = normalize(fs_in.f_Normal);
  // vec3 lightDirection = normalize(light.position - fs_in.f_Position);
  // float diffuseStrength = max(dot(lightDirection, normal), 0.0);
  // vec3 diffuse = light.diffuse * (diffuseStrength * material.diffuse);
  
  // /* specular */
  // vec3 toEye = normalize(eyePosition - fs_in.f_Position);
  // vec3 reflection = reflect(-lightDirection, normal);
  // float specularStrength = pow(max(dot(reflection, toEye), 0.0), material.shininess);
  // vec3 specular = light.specular * (specularStrength * material.specular);
  
  // /* attenuation */
  // float r = length(light.position - fs_in.f_Position);
  // float attenuation = 1/(light.Kc + light.Kl * r + light.Kq * r * r );
  vec4 _color = texture(GLTextNodeTexture, fs_in.f_Textcoords);
  if ((_color.z +_color.y + _color.x) < 0.001)
    discard;
  else
    color = vec4(_color.rgb, 1.0);
  // color = vec4( (ambient + (diffuse + specular) * attenuation), 1.0f);
}
