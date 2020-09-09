#version 150

in vec2 outTexCoord;
uniform sampler2D texUnit;
out vec4 out_Color;

void main(void){
    vec4 pixel_value = texture(texUnit, outTexCoord);

    out_Color.r = max(pixel_value.r - 1.0, 0);
    out_Color.b = max(pixel_value.b - 1.0, 0);
    out_Color.g = max(pixel_value.g - 1.0, 0);

}