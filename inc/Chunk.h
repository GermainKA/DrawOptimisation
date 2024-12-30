#pragma once

#include <vector>
#include "mat.h"
#include "mesh.h"
#include "Frustum.h"


class Chunk {
public:
    Chunk(int startX, int startY, int width, int height, const std::vector<vec3>& allPositions);
    ~Chunk();

    void makeInstanceChunkVBO();  
    void computeBoundingBox(bool isRegularGrid = true)  ;               

    bool IsVisible(const Frustum& frustum); 
    BoundingBox getBoundingBox() const{return m_bounds_;};
    std::vector<vec3> m_positions_; 

private:
    int m_startX_, m_startY_;
    int m_width_, m_height_;
    GLuint m_instanceVBO_;
    BoundingBox m_bounds_;
};
