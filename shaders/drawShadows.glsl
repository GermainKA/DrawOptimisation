#version 330

#ifdef VERTEX_SHADER
layout(location= 0) in vec3 aPosition;
layout(location= 1) in vec3 aNormal;
layout(location= 2) in vec2 aTexCoord;
layout(location= 3) in vec3 aInstPos;
//layout(location= 4) in int aTexLayer;


uniform mat4 mat4View ;
uniform mat4 mat4Proj;
uniform mat4 mat4lightVP;

out vec3 frgNormal;
out vec2 frgTexcoord;
flat out int texLayer;
out vec4 fragPosLightSpace;



void main( )
{
    int maxHeight = 20; 
    int nbLayer = 5 ; 
    mat4 model = mat4(1.0);
    model[3] = vec4(aInstPos, 1.0);
    gl_Position= mat4Proj * mat4View * model * vec4(aPosition, 1.0);

     texLayer = int((aInstPos[1] / maxHeight) * nbLayer) + int(aInstPos[1] > 0);

    //frgNormal = mat3(mat4View * model) * aNormal;
    frgNormal = mat3(transpose(inverse(mat4View * model))) * aNormal; 
    frgTexcoord = aTexCoord;
    fragPosLightSpace = mat4lightVP * model * vec4(aPosition, 1.0);

}

#endif


#ifdef FRAGMENT_SHADER

//uniform vec4 color;
in vec3 frgNormal;
in vec2 frgTexcoord;
flat in int texLayer;
in vec4 fragPosLightSpace;

uniform sampler2DArray textureArray;
uniform sampler2D shadowMap;
uniform vec3 lightDir;

layout(location = 0) out vec4 fragment_color; 


float ShadowCalculation(vec4 fragPosLightSpace)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    if(projCoords.x < 0.0 || projCoords.x > 1.0 || projCoords.y < 0.0 || projCoords.y > 1.0 || projCoords.z > 1.0)
        return 1.0; // On met les zones hors frustum source a l'ombre.

    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    float currentDepth = projCoords.z;
    float shadow = 0.0;

    for(int x = -1; x <= 1; x++) {
        for(int y = -1; y <= 1; y++) {
            float depth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += float(currentDepth > depth + 0.005);
        }
    }
    shadow /= 9.0;
    return shadow;
}




void main( )
{

float shadow = ShadowCalculation(fragPosLightSpace);


vec4 color= texture(textureArray, vec3(frgTexcoord, texLayer));

//vec3 lighting = vec3(1.0 - shadow);
vec3 lighting = mix(vec3(0.3), vec3(1.0), 1.0 - shadow);


//fragment_color = vec4(1.0-shadow);
fragment_color = vec4(color.rgb * lighting, color.a);

}

#endif
