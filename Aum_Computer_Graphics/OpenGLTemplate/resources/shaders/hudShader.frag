// tutorial : https://learnopengl.com/In-Practice/2D-Game/Rendering-Sprites
//#version 330 core
//in vec2 TexCoords;
//out vec4 color;
//
//uniform sampler2D image;
//uniform vec3 spriteColor;
//
//void main()
//{
//    color = vec4(spriteColor, 1.0) * texture(image, TexCoords);
//}










































// Fragment shader : https://stackoverflow.com/questions/4579020/how-do-i-use-a-glsl-shader-to-apply-a-radial-blur-to-an-entire-scene


uniform sampler2D tex;
varying vec2 uv;
const float sampleDist = 1.0;
const float sampleStrength = 2.2;

void main(void)
{
//    float samples[10];
//    samples[0] = -0.08;
//    samples[1] = -0.05;
//    samples[2] = -0.03;
//    samples[3] = -0.02;
//    samples[4] = -0.01;
//    samples[5] =  0.01;
//    samples[6] =  0.02;
//    samples[7] =  0.03;
//    samples[8] =  0.05;
//    samples[9] =  0.08;

    // location of the center of the blur
    vec2 dir = 0.5 - uv;

    float dist = sqrt(dir.x*dir.x + dir.y*dir.y);
    dir = dir/dist;

    vec4 color = texture(tex,uv);
    vec4 sum = color;

//    for (int i = 0; i < 10; i++)
//    sum += texture( tex, uv + dir * samples[i] * sampleDist );

    sum *= 1.0/11.0;
    float t = dist * sampleStrength;
    t = clamp( t ,0.0,1.0);

//    gl_FragColor = mix( color, sum, t );   
    gl_FragColor = color;
}


