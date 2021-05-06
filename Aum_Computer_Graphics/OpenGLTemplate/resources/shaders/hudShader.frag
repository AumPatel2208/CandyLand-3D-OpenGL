#version 400 core

uniform sampler2D tex;
in vec2 uv;
const float sampleDist = 1.0;
const float sampleStrength = 2.2;

void main(void)
{
    // location of the center of the blur
    vec2 dir = 0.5 - uv;

    float dist = sqrt(dir.x*dir.x + dir.y*dir.y);
    dir = dir/dist;

    vec4 color = texture(tex,uv);
    vec4 sum = color;

    sum *= 1.0/11.0;
    float t = dist * sampleStrength;
    t = clamp( t ,0.0,1.0);

    gl_FragColor = color;
}


