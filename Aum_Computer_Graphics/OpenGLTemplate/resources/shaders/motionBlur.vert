// Vertex shader : https://stackoverflow.com/questions/4579020/how-do-i-use-a-glsl-shader-to-apply-a-radial-blur-to-an-entire-scene

varying vec2 uv;

void main(void)
{
    gl_Position = vec4( gl_Vertex.xy, 0.0, 1.0 );
    gl_Position = sign( gl_Position );
    uv = (vec2( gl_Position.x, - gl_Position.y ) + vec2(1.0) ) / vec2(2.0);
}