#version 330

#ifdef VERTEX_SHADER
layout(location= 0) in vec3 aPosition;
layout(location= 1) in vec3 aNormal;
layout(location= 2) in vec2 aTexCoord;


uniform mat4 mat4View ;
uniform mat4 mat4Proj;

out vec3 frgNormal;
out vec2 frgTexcoord;
flat out int texLayer;


void main( )
{
    gl_Position= mat4Proj * mat4View * model * vec4(position, 1);

    frgNormal = mat3(transpose(inverse(mat4View * model))) * normal; 
    frgTexcoord = aTexCoord;
    //texLayer = aTexLayer;

}

#endif


#ifdef FRAGMENT_SHADER

//uniform vec4 color;
in vec3 frgNormal;
in vec2 frgTexcoord;

uniform sampler2D textureArray;

out vec4 fragment_color;


void main( )
{

    vec3 norm = normalize(frgNormal);
    fragment_color= vec4(color * abs(norm.z));

}

#endif
