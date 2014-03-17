#version 330

layout(location = 0) in vec3 in_position;		
layout(location = 1) in vec3 in_normal;		

uniform mat4 model_matrix, view_matrix, projection_matrix;
uniform vec3 light_position;
uniform vec3 eye_position;
uniform int material_shininess;
uniform float material_kd;
uniform float material_ks;

out float light;

void main(){

	//TODO

	gl_Position = projection_matrix*view_matrix*model_matrix*vec4(in_position,1); 
}
