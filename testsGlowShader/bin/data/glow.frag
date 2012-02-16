#version 120
#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect src_tex_unit0;
uniform float blurAmnt;
uniform int direction;
//uniform int kernelSize;
uniform float brightness;

float coeffs[3] = float[](0.2270270270, 0.3162162162, 0.0702702703);

void main()
{

	vec2 st = gl_TexCoord[0].st;
	if(direction == 0){
		//horizontal blur 
		//from http://www.gamerendering.com/2008/10/11/gaussian-blur-filter-shader/
		
		vec3 color = vec3(0.0,0.0,0.0);
		
		color += coeffs[2] * texture2DRect(src_tex_unit0, st - vec2(2.0, 0.0)).rgb;
		color += coeffs[1] * texture2DRect(src_tex_unit0, st - vec2(1.0, 0.0)).rgb;
		color += coeffs[0] * texture2DRect(src_tex_unit0, st).rgb;
		color += coeffs[2] * texture2DRect(src_tex_unit0, st + vec2(2.0, 0.0)).rgb;
		color += coeffs[1] * texture2DRect(src_tex_unit0, st + vec2(1.0, 0.0)).rgb;
		
		gl_FragColor = vec4((color * brightness),1);
	}else{
		vec3 color = vec3(0.0,0.0,0.0);
		
		color += coeffs[2] * texture2DRect(src_tex_unit0, st - vec2(0.0, 2.0)).rgb;
		color += coeffs[1] * texture2DRect(src_tex_unit0, st - vec2(0.0, 1.0)).rgb;
		color += coeffs[0] * texture2DRect(src_tex_unit0, st).rgb;
		color += coeffs[2] * texture2DRect(src_tex_unit0, st + vec2(0.0, 2.0)).rgb;
		color += coeffs[1] * texture2DRect(src_tex_unit0, st + vec2(0.0, 1.0)).rgb;
		
		gl_FragColor = vec4((color * brightness),texture2DRect(src_tex_unit0, st).a);
	}
}
