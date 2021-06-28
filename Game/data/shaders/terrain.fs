
varying vec3 v_position;
varying vec3 v_world_position;
varying vec3 v_normal;
varying vec2 v_uv;
varying vec4 v_color;

uniform vec4 u_color;
uniform sampler2D u_texture_1;
uniform sampler2D u_texture_2;
uniform sampler2D map_info;
uniform float u_time;

void main()
{
	vec2 uv = v_uv;
	vec2 uv_mask = v_uv * 1/1024;
	vec4 rocks = texture2D(u_texture_1,uv);
	vec4 grass = texture2D(u_texture_2,uv);
	vec4 mask = texture2D(map_info, uv_mask);
	float mask_float = step(0.7, mask.x);
	vec4 mask_color = grass * mask_float + rocks * (1 - mask_float);
	gl_FragColor = u_color * mask_color;
	//gl_FragColor = u_color * mix(grass,rocks,clamp(mask,0,1));
}
