#include <vector>
#include <string>
#include <GL/gl.h>


class Texture{

    public :

    Texture(int nbLayer = 6) ;
    void setTexArrayUnit();
    void setSampler();
    void loadArrayTextures();
    void loadArrayTextures(const std::vector<std::string>& textureFiles);
    GLuint getTexArraysId(){return m_TexArraysId;};


    protected :


    private :
    GLuint m_TexArraysId;
    int m_LayerCount;



};