
	
#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;
layout (location = 3) in vec2 uv;

out struct fragment_data
{
    vec3 position;
    vec3 normal;
    vec3 color;
    vec2 uv;
	vec3 eye;
} fragment;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;
uniform int parcelle_length;

void main()
{
	vec3 p0 = position.xyz;
	float d = sqrt((p0.x-parcelle_length/2)*(p0.x-parcelle_length/2)+(p0.y-parcelle_length/2)*(p0.y-parcelle_length/2));
	float omega = 2 * d - 3* time;
	//float omega = 4*3.14*p0.x-3*time;

	float height = 0.5 * exp(-7*d/parcelle_length);
	// Procedural deformation: z = 0.05*cos( (x^2+y^2)^0.5 - 3*t );
	vec3 p = vec3(p0.x, p0.y, height*cos(omega) );

	// Compute exact normals after deformation
	vec3 dpdx = vec3(1, 0, -20*(p0.x-parcelle_length/2)/d*height*cos(omega) );
	vec3 dpdy = vec3(0, 1, -20*(p0.y-parcelle_length/2)/d*height*cos(omega) );
	vec3 n = normalize(cross(dpdx,dpdy));

	fragment.position = vec3(model * vec4(p,1.0));
	fragment.normal   = vec3(model * vec4(n  ,0.0));
	fragment.color = color;
	fragment.uv = uv;
	fragment.eye = vec3(inverse(view)*vec4(0,0,0,1.0));

	gl_Position = projection * view * model * vec4(p, 1.0);
}