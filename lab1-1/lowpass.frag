#version 150

/*Lowpass filter to make blooming effect*/


in vec2 outTexCoord;
uniform sampler2D texUnit;
out vec4 out_Color;

float filter_Kernal[25] = float[]
                          (
                            1, 4,  6,  4, 1,
                            4, 16, 24, 16, 4,
                            6, 24, 36, 24, 6,
                            4, 16, 24, 16, 4,
                            1, 4,  6,  4,  1
                          );

void main(void)
{
    vec3 pixel_Color = vec3(0,0,0);

    int sum = 256;
    int max_offset = 2;
    for(int x = -max_offset; x <= max_offset; x++){
      for(int y = -max_offset; y <= max_offset; y++){
        int index = ((y+2)*5)+(x+2);

        float offset_x = x/512.0; // screenWidth
        float offset_y = y/512.0; // screenHeight
        vec2 offset = vec2(offset_x, offset_y);
       
        //vec2 coord = vec2(outTexCoord.x + offset_x, outTexCoord.y + offset_y);

        vec2 uv = outTexCoord + offset;

        //vec3 old_Pixel_Color = vec3(texture(texUnit, coord));

        //pixel_Color = pixel_Color + filter_Kernal[index]*old_Pixel_Color;

        float weight = filter_Kernal[index]/255;

        pixel_Color += texture(texUnit, uv).rgb * weight;
      }
    }
    

    out_Color = vec4(pixel_Color, 1.0);
}