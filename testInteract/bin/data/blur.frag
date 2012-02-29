#version 120
#extension GL_ARB_texture_rectangle : enable
uniform sampler2DRect src_tex_unit0;
uniform sampler2D depth;
uniform float brightness;

uniform float rt_w; // render target width
uniform float rt_h; // render target height

float offset[3] = float[]( 0.0, 1.3846153846, 3.2307692308 );
float weight[3] = float[]( 0.2270270270, 0.3162162162, 0.0702702703 );

uniform int direction;
uniform int genTexCoords;

//varying float ldepth;


void main( void ){	
	vec2 uv;
	
	if(direction==0){		
		if(genTexCoords==0){	
			uv = gl_TexCoord[0].st;
		}else{
			uv = gl_FragCoord.xy;
		}
		gl_FragDepth = gl_FragCoord.z;
	}else{
		if(genTexCoords==0){	
			uv = vec2(gl_TexCoord[0].s,gl_TexCoord[0].t);
			gl_FragDepth = texture2D(depth, vec2(gl_TexCoord[0].s/rt_w,gl_TexCoord[0].t/rt_h)).r;
		}else{
			uv = vec2(gl_FragCoord.x,rt_h-gl_FragCoord.y);
			gl_FragDepth = texture2D(depth, vec2(gl_FragCoord.x/rt_w,rt_h-gl_FragCoord.y/rt_h)).r;
		}
	}
	
	
	vec4 color = texture2DRect(src_tex_unit0, uv) * weight[0];
	
	if( direction == 0 ){
	    color = texture2DRect(src_tex_unit0, uv) * weight[0];
	    for (int i=1; i<3; i++){
	        color += texture2DRect(src_tex_unit0, uv + vec2(0.0, i)) * weight[i];
	        color += texture2DRect(src_tex_unit0, uv - vec2(0.0, i)) * weight[i];
	    }
	}else{
	    color = texture2DRect(src_tex_unit0, uv) * weight[0];
	    for (int i=1; i<3; i++){
	        color += texture2DRect(src_tex_unit0, uv + vec2(i, 0.0)) * weight[i];
	        color += texture2DRect(src_tex_unit0, uv - vec2(i, 0.0)) * weight[i];
	    }
	}

	float tempA = color.a;
	
	color *= brightness;
	color.a = tempA;
		
	gl_FragColor = color;
}