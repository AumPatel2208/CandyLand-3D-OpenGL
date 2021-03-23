//uniform sampler2D uTexLinearDepth;
//
//uniform mat4 uInverseModelViewMat; // inverse model->view
//uniform mat4 uPrevModelViewProj; // previous model->view->projection
//
//noperspective in vec2 vTexcoord;
//noperspective in vec3 vViewRay; // for extracting current world space position
//
//void main() {
//    vec3 current = vViewRay * texture(uTexLinearDepth, vTexcoord).r;
//    vec4 previous = uCurrentToPreviousMat * vec4(current, 1.0);
//    previous.xyz /= previous.w;
//    previous.xy = previous.xy * 0.5 + 0.5;
//
//    vec2 blurVec = previous.xy - vTexcoord;
//    // perform blur:
//    vec4 result = texture(uTexInput, vTexcoord);
//    for (int i = 1; i < nSamples; ++i) {
//        // get offset in range [-0.5, 0.5]:
//        vec2 offset = blurVec * (float(i) / float(nSamples - 1) - 0.5);
//
//        // sample & add to result:
//        result += texture(uTexInput, vTexcoord + offset);
//    }
//
//    result /= float(nSamples);
//}
//
//
//
//

#version 400
in vec2 vTexCoord;
out vec4 vOutputColour;
uniform sampler2D sampler0; // used to access the rendered image from pass 1
uniform sampler2D sampler1; // used to access the depth buffer
uniform mat4 invMVP;
uniform mat4 prevMVP;
void main()
{
    // Unproject the point to 3D using the current modelview and projection matrix
    float p_depth = texture(sampler1, vTexCoord).r;
    vec2 p_ndc = vTexCoord.xy * 2.0f - 1.0f;
    vec4 p3d = invMVP * vec4(p_ndc, p_depth, 1.0);
    p3d = p3d / p3d.w;
    // Project the point using the previous frame's modelview and projection matrix
    vec4 q = prevMVP * p3d;
    q.xyz /= q.w; // normalised device coordinates
    q.xy = q.xy * 0.5 + vec2(0.5); // texture coordinates
    // Construct the blur vector (scaled for extra blur)
    vec2 blurVec = (q.xy - vTexCoord)*5.0f;
    vec4 result = texture(sampler0, vTexCoord);
    int nSamples = 11;
    for (int i = 1; i < nSamples; i++) {
        vec2 offset = blurVec * (float(i) / float(nSamples - 1) - 0.5);
        result += texture(sampler0, vTexCoord + offset);
    }
    result /= float(nSamples);
    vOutputColour = result;
}












//uniform sampler2D uTexInput; // texture we're blurring
//uniform sampler2D uTexVelocity; // velocity buffer
//
//uniform float uVelocityScale;
//
//out vec4 oResult;
//
//void main(void) {
//    vec2 texelSize = 1.0 / vec2(textureSize(uTexInput, 0));
//    vec2 screenTexCoords = gl_FragCoord.xy * texelSize;
//
//    vec2 velocity = texture(uTexMotion, screenTexCoords).rg;
//    // NEED TO DO THIS IN CLIENT ->>> uVelocityScale = currentFps / targetFps;
//    velocity *= uVelocityScale;
//
//    float speed = length(velocity / texelSize);
////    nSamples = clamp(int(speed), 1, MAX_SAMPLES);
//    // added type, might give error
//    int nSamples = clamp(int(speed), 1, MAX_SAMPLES);
//    // blur code will go here...
//    oResult = texture(uTexInput, screenTexCoords);
//    for (int i = 1; i < nSamples; ++i) {
//        vec2 offset = velocity * (float(i) / float(nSamples - 1) - 0.5);
//        oResult += texture(uTexInput, screenTexCoords + offset);
//    }
//    oResult /= float(nSamples);
//}