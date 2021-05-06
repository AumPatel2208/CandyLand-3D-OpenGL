// Vertex shader : https://stackoverflow.com/questions/4579020/how-do-i-use-a-glsl-shader-to-apply-a-radial-blur-to-an-entire-scene
#version 400 core
out vec2 uv;
// Layout of vertex attributes in VBO
layout (location = 0) in vec2 inPosition;
layout (location = 1) in vec2 inCoord;
void main(void)
{
    //vec4 position = vec4( inPosition.xy, 0.0, 1.0 );
    gl_Position = vec4( inPosition.xy, 0.0, 1.0 );

    //gl_Position = sign( gl_Position );
    gl_Position = sign( gl_Position );

    // y is being scaled by -1 to flip it
    uv = (vec2( gl_Position.x, - gl_Position.y*-1.0 ) + vec2(1.0) ) / vec2(2.0);
}