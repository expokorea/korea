#version 120
#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect src_tex_unit0;
uniform float blurAmnt;
uniform int direction;
uniform int kernelSize;
uniform float brightness;

void main()
{

	vec2 st = gl_FragCoord.xy;;
	if(direction == 0){
		//horizontal blur 
		//from http://www.gamerendering.com/2008/10/11/gaussian-blur-filter-shader/
		
		vec4 color;
		
		for(int i=1;i<kernelSize;i++){
			color += float(i) * texture2DRect(src_tex_unit0, st + vec2(-float(kernelSize-i), 0.0));
		}
		
		color += (kernelSize+1) * texture2DRect(src_tex_unit0, st);
		
		for(int i=kernelSize-1;i>0;i--){
			color += float(i) * texture2DRect(src_tex_unit0, st + vec2(float(kernelSize-i), 0.0));
		}
		
		color /= kernelSize*kernelSize;
		gl_FragColor = color * brightness;
	}else{
		vec4 color;
	
		for(int i=1;i<kernelSize;i++){
			color += float(i) * texture2DRect(src_tex_unit0, st + vec2(0.0, float(kernelSize-i)));
		}
		
		color += (kernelSize+1) * texture2DRect(src_tex_unit0, st + vec2(0.0, blurAmnt) );
		
		for(int i=kernelSize-1;i>0;i--){
			color += float(i) * texture2DRect(src_tex_unit0, st + vec2(0.0, -float(kernelSize-i)));
		}
		
		color /= kernelSize*kernelSize;
		gl_FragColor = color * brightness;
	}
}
