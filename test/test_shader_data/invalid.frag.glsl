#version 330 core
uniform int numPointLights;
// INVALID FRAG SHADER
// THIS WILL FAIL UPON ATTEMPTING TO COMPILE


void main()
{
    // properties
    vec3 norm = normalize(fragNormal);
    vec3 viewDir = normalize(viewPos - fragPos);

    // phase 1: Directional lighting
    vec3 dirResult = calcDirLight(dirLight, norm, viewDir);
    // point lighting
    vec3 pointResult = {0, 0, 0};
    for (int i = 0; i < numPointLights; i++) {
        pointResult += calcPointLight(pointLights[i], norm, viewDir);
    }
    FragColor = vec4(dirResult + pointResult, 1.0);
