#version 330

#ifdef VERTEX_SHADER
layout(location= 0) in vec3 aPosition;
layout(location= 1) in vec3 aNormal;
layout(location= 2) in vec2 aTexCoord;
layout(location= 3) in vec3 aInstPos;
//layout(location= 4) in int aTexLayer;


uniform mat4 mat4View ;
uniform mat4 mat4Proj;

out vec3 frgNormal;
out vec2 frgTexcoord;
flat out int texLayer;


void main( )
{
    int maxHeight = 20; 
    int nbLayer = 5; 
    mat4 model = mat4(1.0);
    model[3] = vec4(aInstPos, 1.0);
    gl_Position= mat4Proj * mat4View * model * vec4(aPosition, 1);

    texLayer = int((aInstPos[1] / maxHeight) * nbLayer) + int(aInstPos[1] > 0);

    frgNormal = mat3(transpose(inverse(mat4View * model))) * aNormal; 
    frgTexcoord = aTexCoord;

}

#endif


#ifdef FRAGMENT_SHADER

//uniform vec4 color;
in vec3 frgNormal;
in vec2 frgTexcoord;
flat in int texLayer;

uniform sampler2DArray textureArray;
layout(location = 0) out vec4 fragment_color; 


void main( )
{

    vec3 norm = normalize(frgNormal);
    vec4 color= texture(textureArray,vec3(frgTexcoord,texLayer));
    fragment_color = vec4(color.rgb , color.a);
}

#endif
