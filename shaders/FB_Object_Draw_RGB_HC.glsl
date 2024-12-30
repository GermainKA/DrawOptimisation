#version 330 

#ifdef VERTEX_SHADER


out vec2 frgTexCoord;
void main()
{
    vec2 positions[4] = vec2[4](
        vec2(-1.0, -1.0),
        vec2(1.0, -1.0), 
        vec2(1.0, 1.0),  
        vec2(-1.0, 1.0)  
    );

    gl_Position = vec4(positions[gl_VertexID], 0.0, 1.0);
    frgTexCoord = positions[gl_VertexID] * 0.5 + 0.5; 
}

#endif

#ifdef FRAGMENT_SHADER

precision mediump float;
uniform sampler2D Buffer;
in vec2 frgTexCoord;
out vec4 glFragColor;
void main() {
    glFragColor = texture(Buffer,frgTexCoord) ;
};
#endif