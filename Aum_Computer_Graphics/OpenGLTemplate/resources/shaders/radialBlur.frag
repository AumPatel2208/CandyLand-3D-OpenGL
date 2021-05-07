// Fragment shader : https://stackoverflow.com/questions/4579020/how-do-i-use-a-glsl-shader-to-apply-a-radial-blur-to-an-entire-scene
#version 400 core
uniform sampler2D tex;
in vec2 uv;
const float sampleDist = 1.0;
//const float sampleStrength = 2.2;
const float sampleStrength = 3.2;

void main(void)
{
    float samples[10];
    samples[0] = -0.08;
    samples[1] = -0.05;
    samples[2] = -0.03;
    samples[3] = -0.02;
    samples[4] = -0.01;
    samples[5] =  0.01;
    samples[6] =  0.02;
    samples[7] =  0.03;
    samples[8] =  0.05;
    samples[9] =  0.08;

    // location of the center of the blur
    vec2 dir = 0.5 - uv;
    
    float dist = sqrt(dir.x*dir.x + dir.y*dir.y);
    dir = dir/dist;

    vec4 color = texture(tex,uv);
    vec4 sum = color;

    for (int i = 0; i < 10; i++)
        sum += texture( tex, uv + dir * samples[i] * sampleDist );

    sum *= 1.0/11.0;
    float t = dist * sampleStrength;
    t = clamp( t ,0.0,1.0);

    gl_FragColor = mix( color, sum, t );
}

