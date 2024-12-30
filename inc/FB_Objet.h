#pragma once
#include <GL/glew.h>
#include <GL/gl.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <iostream>



class FB_Objet {

    public:
    /*COnstructor*/
    FB_Objet(int width, int height, GLenum color, GLenum depth, int colorsnb = 3,GLenum filtering = GL_NEAREST);

    /*Destructor*/
    virtual ~FB_Objet();

    void enable ();
    void disable ();

    //Draw 
    void onDraw(GLenum attachement, bool alpha=false);

    void onDrawDepth();
    void onDrawAlpha(GLenum attachement);
    void onDrawRGB(GLenum attachement);
    

    //Attach Texture To Program
    virtual void setTextureUnit(GLint unit, GLint locSampler=-1, GLuint bufferId=0);

    // Init VBO and Shaders for Draw
    static void staticInit();
    static void staticDestroy();

    GLuint getColorBuffer(int num=0);
    GLuint getDepthBufferId(){return m_DepthBufferId ;} ;


    protected:
    FB_Objet();


    //FBO
    GLuint m_FBO;
    GLint m_PrevFBO;
    GLint m_ViewPort[4];

    //FBO Ids
    //Colors Textures ID and AttachementPos
    std::vector<GLuint> m_ColorBufferIds;
    std::vector<GLuint> m_DrawBufferNames;
    //Depth Buffer ID
    GLuint m_DepthBufferId;

    //Dimension
    GLuint m_Width;
    GLuint m_Height;
    //Init
    void init(int width,int heigth);
    void checkStat();


    //RGB Draw Shader Info
    static GLint m_ShaderRGBId;
    static GLint m_ShaderDepthId;
    static GLuint m_RGB_VAO;
    static GLint m_BufferRGBLoc; // 
};

