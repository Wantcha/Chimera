#type vertex
#version 330 core
			
uniform mat4 projectionMatrix;
layout(location = 0) in vec2 v_position;
layout(location = 1) in vec4 v_color;
layout(location = 2) in float v_size;
out vec4 f_color;
void main(void)
{
	f_color = v_color;
	gl_Position = projectionMatrix * vec4(v_position, 0.0f, 1.0f);
    gl_PointSize = v_size;
}

#type fragment
#version 330 core
			
in vec4 f_color;
out vec4 color;

void main(void)
{
	color = f_color;
}