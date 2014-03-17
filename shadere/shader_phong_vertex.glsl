#version 330

layout(location = 0) in vec3 in_position;		
layout(location = 1) in vec3 in_normal;		
layout(location = 2) in vec2 in_texcoord;
layout(location = 3) in vec3 newpos;
layout(location = 4) in vec3 newnormal;

uniform mat4 model_matrix, view_matrix, projection_matrix;
uniform int counter;
uniform int fcounter;
uniform vec3 speed;
uniform int part;

out vec3 world_pos;
out vec3 world_normal;

out vec2 texcoord;

void main() {
	texcoord = in_texcoord;
	// calculare pozitie noua pentru vertecsii fetei
	if(part == 1) {
		vec3 pos = in_position + (newpos - in_position) / fcounter * counter;
		vec3 norm = in_normal + (newnormal - in_normal) / fcounter * counter;

		world_pos = (model_matrix * vec4(pos,1)).xyz;
		world_normal = normalize(mat3(model_matrix) * norm);
		gl_Position = projection_matrix*view_matrix*model_matrix*vec4(pos,1);
	}
	// calculare pozitie noua pentru particule
	else if(part == 2) {
		vec3 bilb_pos;
		bilb_pos = in_position + counter * speed;
		world_pos = (model_matrix * vec4(bilb_pos,1)).xyz;
		gl_Position = projection_matrix * view_matrix * model_matrix * vec4(0.0, 0.0, 0.0, 1.0)
              + vec4(bilb_pos.x, bilb_pos.y, 0.0, 0.0);  
	}
	// pentru perioada sleep-sleep
	else if(part == 3) {
		world_pos = (model_matrix * vec4(in_position,1)).xyz;
		world_normal = normalize(mat3(model_matrix) * in_normal);
		gl_Position = projection_matrix*view_matrix*model_matrix*vec4(in_position,1); 
	}
}
