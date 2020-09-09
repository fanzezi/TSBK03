#version 150

in float shade;
in vec2 frag_texcoord;

uniform sampler2D exampletexture;
uniform float time;

out vec4 out_Color;

void main(void)
{
	
	
	// --Procedurell textur--
	//float a = sin(frag_texcoord.s * 30.0 + time)/2.0 + 0.5;
	//float b = sin(frag_texcoord.t * 30.0 * (1.0+sin(time/4.0)))/2.0 + 0.5;
	//out_Color = vec4(a, b, 0.5, 1.0); // inColor;
	// ----

	


	//out_Color = (exampletexture, frag_texcoord);
	out_Color=vec4(shade,shade,shade,1.0);
	// Red
	//out_Color=vec4(1.0, 0.0, 0.0,1.0);
}

