#include "FB_Objet.h"
#include "program.h"


FB_Objet::FB_Objet(int width, int height, GLenum color, GLenum depth, int colorsnb , GLenum filtering){
    //FBO Initiatilisation
    init(width,height);
    glGenFramebuffers(1, &m_FBO);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FBO);

    unsigned int texture = 0;
    const GLfloat borderColor[] = {0.0,0.0,0.0,0.0};
    //Create & Attach Texture
    switch(color){

        case GL_TEXTURE_2D :

            //Create Color Texture
            glGenTextures(1,&texture);
            glBindTexture(GL_TEXTURE_2D,texture);
            glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA32F,m_Width,m_Height,0,GL_RGBA,GL_UNSIGNED_BYTE,NULL);
            //Texture Configuration
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);
            //Attach to FBO
            glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,texture,0);

            m_ColorBufferIds.push_back(texture);
            m_DrawBufferNames.push_back(GL_COLOR_ATTACHMENT0);
            std::cout << "DEBUG" << std::endl;

        break;

        case GL_RENDERBUFFER :
        std::cerr <<" Not Define Yet" << std::endl;
        break;
        
        case GL_NONE :
        break;

        default :
        std::cerr <<" Invalid base color" << std::endl;
        break;
    }

    switch (depth) {

    case GL_TEXTURE_2D:
        glGenTextures(1, &m_DepthBufferId);
        glBindTexture(GL_TEXTURE_2D, m_DepthBufferId);
        //glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height,
             0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

            //Texture Configuration
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);

        // Border
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

        // Attach Depth component to FBO
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthBufferId, 0);
        break;

    case GL_RENDERBUFFER:
        glGenRenderbuffers(1, &m_DepthBufferId);
        glBindRenderbuffer(GL_RENDERBUFFER, m_DepthBufferId);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthBufferId);
        break;

    case GL_NONE:
        break;

    default:
        std::cerr << "FrameBufferObject: depth param is not Correct" << std::endl;
        break;
    }


    for(int i=0;i<colorsnb;i++){
        //Create Color Texture
        glGenTextures(1,&texture);
        glBindTexture(GL_TEXTURE_2D,texture);
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA32F,m_Width,m_Height,0,GL_RGBA,GL_UNSIGNED_BYTE,NULL);

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);
        glBindTexture(GL_TEXTURE_2D,0);
        //Attach to FBO
        glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT1+i,GL_TEXTURE_2D,texture,0);

        m_ColorBufferIds.push_back(texture);
        m_DrawBufferNames.push_back(GL_COLOR_ATTACHMENT1+i);

    }

    glDrawBuffers(m_DrawBufferNames.size(),&m_DrawBufferNames[0]);

    checkStat();

    //Disable The FBO
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void FB_Objet::checkStat()
{
    GLuint code = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    const char* status;
    switch (code) {
        case GL_FRAMEBUFFER_COMPLETE:
            status="FRAMEBUFFER_COMPLETE";
            break;
        case GL_FRAMEBUFFER_UNDEFINED:
            status="FRAMEBUFFER_UNDEFINED";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            status="FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            status="FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
            status="FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
            status="FRAMEBUFFER_INCOMPLETE_READ_BUFFER";
            break;
        case GL_FRAMEBUFFER_UNSUPPORTED:
            status="FRAMEBUFFER_UNSUPPORTED";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
            status="FRAMEBUFFER_INCOMPLETE_MULTISAMPLE";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
            status="FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS";
            break;
        default:
            status="FRAMEBUFFER_??";
            break;
    }
    if (code != GL_FRAMEBUFFER_COMPLETE) {
        // ERROR
        std::cout << "FrameBufferObject : creating FBO " << m_Width<<"x"<<m_Height << " : " << status << std::endl;
        throw std::invalid_argument("creating FBO");
    }
}

void FB_Objet::enable (){

    //Save View Port
   glGetIntegerv(GL_VIEWPORT, m_ViewPort);

    // Current VIEW PORT
    glViewport(0,0,m_Width,m_Height);

    //Save Old FBO
    glGetIntegerv(GL_FRAMEBUFFER_BINDING,&m_PrevFBO);


    //Enable FBO
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

}

void FB_Objet::disable(){

    glBindFramebuffer(GL_FRAMEBUFFER,m_PrevFBO);

    glViewport(m_ViewPort[0],m_ViewPort[1],m_ViewPort[2],m_ViewPort[3]);

}
/*
*Init FBO params
*/
void FB_Objet::init(int width, int height){
    m_Width = width ;
    m_Height = height ;
    m_FBO = 0 ;
    m_PrevFBO = 0;
    m_DepthBufferId = 0 ;

}

//Attach Texture To Program
void FB_Objet::setTextureUnit(GLint unit, GLint locSampler, GLuint bufferId){

    /******DEBUG******/
    if (unit < GL_TEXTURE0) {
    throw std::invalid_argument("L'unité de texture doit être GL_TEXTURE0 ou supérieure.");
    }

    /*****DEBUG*****/
    glActiveTexture(unit);
    
    if(locSampler < 0 || bufferId <= 0){
        glBindTexture(GL_TEXTURE_2D, 0);
    }else {
        glBindTexture(GL_TEXTURE_2D,bufferId);
        glUniform1i(locSampler, unit-GL_TEXTURE0);
    }

}

//Draw FBO 
//RGB Draw Shader Info
GLint FB_Objet::m_ShaderRGBId = -1; //RGB TEXT Program Shader Location
GLint FB_Objet::m_ShaderDepthId = -1; //DEPTH Program Shader Location
GLint FB_Objet::m_BufferRGBLoc = -1; // TextureBufferLocation
GLuint FB_Objet::m_RGB_VAO = 0; // TextureBufferLocation


//Init Program For QUAD Draw

void FB_Objet::staticInit()
{

    //Compile

    m_ShaderRGBId = read_program("TP/TP2/shaders/FB_Object_Draw_RGB_HC.glsl");
    program_print_errors(m_ShaderRGBId);
    m_ShaderDepthId = read_program("TP/TP2/shaders/drawDepth.glsl");
    program_print_errors(m_ShaderDepthId);

    
}

void FB_Objet::staticDestroy(){

    release_program(m_ShaderRGBId);

}
//Draw Depth
void FB_Objet::onDrawDepth(){
    //Disable Depth test
    glDisable(GL_DEPTH_TEST);

    //Active Shader
    glUseProgram(m_ShaderDepthId);

    //Bind VBOs & Active Location
    //Vertex VBO
    
    glGenVertexArrays(1,&m_RGB_VAO);
    glBindVertexArray(m_RGB_VAO);
    
    setTextureUnit(GL_TEXTURE0, 0,m_DepthBufferId);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    //Disable Texture
    setTextureUnit(GL_TEXTURE0);

    //Disable Buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //Disable Program
    glUseProgram(0);
    glBindVertexArray(0);

    //Reactive Depth Test
    glEnable(GL_DEPTH_TEST);
    
}

//Draw Depth
void FB_Objet::onDrawAlpha(GLenum attachement){

}

//Draw Depth
void FB_Objet::onDrawRGB(GLenum attachement){
    
    //Disable Depth test
    glDisable(GL_DEPTH_TEST);

    //Active Shader
    glUseProgram(m_ShaderRGBId);

    //Bind VBOs & Active Location
    //Vertex VBO
    
    glGenVertexArrays(1,&m_RGB_VAO);
    glBindVertexArray(m_RGB_VAO);
    

    // attach texture to ShaderProgram
    
    size_t index = attachement - GL_COLOR_ATTACHMENT0;
    if (index >= m_ColorBufferIds.size()) {
        std::cerr << "Index de texture invalide dans m_ColorBufferIds" << std::endl;
        return;
    }
    setTextureUnit(GL_TEXTURE0, 0,m_ColorBufferIds[index]);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    //Disable Texture
    setTextureUnit(GL_TEXTURE0);

    //Disable Buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //Disable Program
    glUseProgram(0);
    glBindVertexArray(0);

    //Reactive Depth Test
    glEnable(GL_DEPTH_TEST);

}

//Draw switch attachement
void FB_Objet::onDraw(GLenum attachement, bool alpha){

    switch (attachement)
    {
    case GL_DEPTH_ATTACHMENT:
        onDrawDepth();
        break;

    default:
        if(alpha) {
            onDrawAlpha(attachement);
        }
        else{
            onDrawRGB(attachement);
        }
        break;
    }

}

GLuint FB_Objet::getColorBuffer(int num)
{
    return m_ColorBufferIds[num];
}
/**
 * Delete  FBO
 */
FB_Objet::~FB_Objet()
{

    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    GLint color = GL_NONE;
    glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE, &color);
    GLint depth = GL_NONE;
    glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE, &depth);


    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteFramebuffers(1, &m_FBO);

    //Delete Color Texture
    switch (color) {
    case GL_TEXTURE:
        glDeleteTextures(1, &m_ColorBufferIds[0]);
        break;
    case GL_RENDERBUFFER:
        glDeleteRenderbuffers(1, &m_ColorBufferIds[0]);
        break;
    }

    //Delete Depth Texture
    switch (depth) {
    case GL_TEXTURE:
        glDeleteTextures(1, &m_DepthBufferId);
        break;
    case GL_RENDERBUFFER:
        glDeleteRenderbuffers(1, &m_DepthBufferId);
        break;
    }
    
    //Delete Others Attachements
    for (size_t i=1; i<m_ColorBufferIds.size(); i++) {
        glDeleteTextures(1, &m_ColorBufferIds[i]);
    }
}