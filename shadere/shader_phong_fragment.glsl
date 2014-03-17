#version 330
layout(location = 0) out vec4 out_color;

uniform vec3 light_position;
uniform vec3 eye_position;
uniform int material_shininess;
uniform float material_kd;
uniform float material_ks;

in vec3 world_pos;
in vec3 world_normal;
in vec2 texcoord;

uniform sampler2D textura1;
uniform sampler2D textura2;
uniform vec3 part_color;
uniform int part;

void main(){

	vec3 text1 = texture(textura1, texcoord).xyz;
	vec3 text2 = texture(textura2, texcoord).xyz;
	
	float light=0;

	vec3 l = normalize(light_position-world_pos);
	vec3 v = normalize(eye_position-world_pos);
	vec3 h = normalize(l+v);

	float ambientala = 0.4f;
	float difuza = material_kd * max(dot(world_normal,l),0);
	float spectrala = material_ks * pow(max(dot(world_normal , h), 0),material_shininess) * (difuza < 0 ? 0 : 1);

	light = ambientala + difuza + spectrala;
	//ignora pixelii albi pentru textura particulei
	if(part == 2) {
		if(text2.rgb.x > 0.5 && text2.rgb.y > 0.5 && text2.rgb.z > 0.5)
			discard; 

		out_color = vec4(text2, 1) + vec4(part_color,1);
	}
	//textura si iluminare pentru fata
	else if(part == 1 || part == 3) {
		out_color = vec4(text1, 1) * vec4(light,light, light,1);
	}
}