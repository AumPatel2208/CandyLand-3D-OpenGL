#version 400 core

in vec3 vColour;// Interpolated colour using colour calculated in the vertex shader
in vec2 vTexCoord;// Interpolated texture coordinate using texture coordinate from the vertex shader

out vec4 vOutputColour;// The output colour

uniform sampler2D sampler0;// The texture sampler
uniform samplerCube CubeMapTex;
uniform bool bUseTexture;// A flag indicating if texture-mapping should be applied
uniform bool renderSkybox;
in vec3 worldPosition;

// FOG: Open GL Game Tutorial 16: fog https://www.youtube.com/watch?v=qslBNLeSPUc
in float visibility;
uniform vec3 skyColour;

// Structure holding light information:  its position as well as ambient, diffuse, and specular colours
struct LightInfo
{
    vec4 position;
    vec3 La;
    vec3 Ld;
    vec3 Ls;

// https://learnopengl.com/Lighting/Light-casters
    float constant;
    float linear;
    float quadratic;
    float intensity;
};

struct MaterialInfo
{
    vec3 Ma;
    vec3 Md;
    vec3 Ms;
    float shininess;
};

in MaterialInfo materialOut;
in vec4 eyePosition;
in vec3 eyeNormal;
uniform LightInfo pointLight1;
//uniform LightInfo pointLight2;
//uniform LightInfo pointLight3;

#define MAX_NO_LIGHTS 500 
uniform LightInfo pointLights[MAX_NO_LIGHTS];

uniform int numberOfLights;
//uniform LightInfo pointLights[MAX_NO_LIGHTS];



vec3 PointLightPhong(vec4 pEyePos, vec3 pEyeNorm, LightInfo pPointLight){
    vec3 s = normalize(vec3(pPointLight.position - pEyePos));
    vec3 v = normalize(-pEyePos.xyz);
    vec3 r = reflect(-s, pEyeNorm);
    vec3 n = pEyeNorm;
    vec3 ambient = pPointLight.La * materialOut.Ma;
    float sDotN = max(dot(s, n), 0.0f);
    vec3 diffuse = pPointLight.Ld * materialOut.Md * sDotN;
    vec3 specular = vec3(0.0f);
    float eps = 0.000001f;// add eps to shininess below -- pow not defined if second argument is 0 (as described in GLSL documentation)
    if (sDotN > 0.0f)
    specular = pPointLight.Ls * materialOut.Ms * pow(max(dot(r, v), 0.0f), materialOut.shininess + eps);

    // https://learnopengl.com/Lighting/Light-caster
    // Attenuation 
    float distance    = length(pPointLight.position - pEyePos);
    float attenuation = 1.0 / (pPointLight.constant + pPointLight.linear * distance + pPointLight.quadratic * (distance * distance));

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular)*pPointLight.intensity;
}

void main()
{
    vec3 pointLightColours = PointLightPhong(eyePosition, eyeNormal, pointLight1);
//    pointLightColours += PointLightPhong(eyePosition, eyeNormal, pointLight2);
//    pointLightColours += PointLightPhong(eyePosition, eyeNormal, pointLight3);
    // Multiple number of lights
    int cap = numberOfLights;
    if (numberOfLights > MAX_NO_LIGHTS){
        cap = MAX_NO_LIGHTS;
    } 
    for (int i = 0; i<cap; i++){
        pointLightColours +=PointLightPhong(eyePosition, eyeNormal, pointLights[i]); 
    }
    

//    
//    // adding the lights into the colour
//    for(int i = 0; i < cap; i++){
//        pointLightColours = PointLightPhong(eyePosition, eyeNormal, pointLights[i]); 
//    }

    if (renderSkybox) {
        vOutputColour = texture(CubeMapTex, worldPosition);
        // fog
        vOutputColour = mix(vec4(skyColour, 1.0), vOutputColour, visibility);

    } else {

        // Get the texel colour from the texture sampler
        vec4 vTexColour = texture(sampler0, vTexCoord);

        if (bUseTexture)
        vOutputColour = vTexColour*vec4(vColour + pointLightColours, 1.0f);// Combine object colour and texture 
        else
        vOutputColour = vec4(vColour + pointLightColours, 1.0f);// Just use the colour instead

        // fog
        vOutputColour = mix(vec4(skyColour, 1.0), vOutputColour, visibility);
    }
}
