#version 330 core
// INVALID VERTEX SHADER
// THIS WILL FAIL UPON ATTEMPTING TO COMPILE

void main()
{
    fragPos = vec3(model * vec4(aPos, 1.0));
    fragNormal = mat3(transpose(inverse(model))) * aNormal;
    texCoords = aTexCoords;
