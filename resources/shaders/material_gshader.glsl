#version 330 core

/* wireframe shader */
layout(triangles) in;
layout(line_strip, max_vertices = 4) out;

in v_data
{
	vec3 f_Position;
	vec3 f_Normal;
} vs_out[3];

out v_data
{
  vec3 f_Position;
  vec3 f_Normal;
} gs_out;

void main()
{
  /* close the line strip loop by emitting the first vertex an the end */
  for(int i = 0; i < 4; i++)
  {
    gl_Position = gl_in[i % 3].gl_Position;
    gs_out.f_Position = vs_out[i % 3].f_Position;
    gs_out.f_Normal = vs_out[i % 3].f_Normal;
  	EmitVertex();
	}
  EndPrimitive();
}
