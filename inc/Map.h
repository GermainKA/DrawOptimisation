#include <string>
#include "mesh.h"
#include "Shape.h"
#include "Chunk.h"

#define CHUNK_SIZE 128
class Map {
    public:
    Map(const char* deepMapfilename = "data/terrain/Clipboard01.png");

    int getInstancePosSize() {return m_instancesPos_.size();}

    ~Map();


    void initInstanceMapVBO();
    void deleteInstanceMapVBO();
    void computeBoundingBox(bool isRegularGrid = true);
    int width(){return m_mapWidth_ ;}
    int height(){return m_mapHeight_ ;}
    BoundingBox getBoundingBox() {return m_bounds_;}
    GLuint getInstanceVboId() {return  m_instanceVBO_;}
    std::vector<Chunk> m_chunks_;
    std::vector<vec3>  m_instancesPos_ ;


    protected:
    void Initializer();

    private :
    std::string m_deepMapfilename_;

    //std::shared_ptr<Shape> m_Shape_ ;
    BoundingBox m_bounds_;
    GLuint m_instanceVBO_;
    int m_mapWidth_ ;
    int m_mapHeight_ ;


};