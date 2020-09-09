#version 150

in  vec3  in_Position;
in  vec3  in_Normal;
in  vec2  in_TexCoord;

uniform mat4 projectionMatrix;
uniform mat4 modelToWorldToView;
uniform float time;

out float shade;
//out vec2 texCoord;


out vec2 frag_texcoord;

void main(void)
{
	//Phong shading
	//exNormal = in_Normal;


	// Texture
	frag_texcoord=in_TexCoord;

	shade = (mat3(modelToWorldToView)*in_Normal).z; // Fake shading
	
	gl_Position=projectionMatrix * modelToWorldToView * vec4(in_Position, 1.0);
	
	// Crazy rotation and shape, animation
	//gl_Position=projectionMatrix * modelToWorldToView * vec4(in_Position*cos(time), 1.0);
}

