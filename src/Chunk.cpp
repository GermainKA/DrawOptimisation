#include "Chunk.h"

Chunk::Chunk(int startX, int startY, int width, int height, const std::vector<vec3>& positions)
    : m_startX_(startX), m_startY_(startY), m_width_(width), m_height_(height),m_positions_(positions) {

    computeBoundingBox();
}

void Chunk::computeBoundingBox(bool isRegularGrid) {
    if (m_positions_.empty()) {
        std::cerr<<"Warning Cmpt AABB: Empty Chunk Positions" << std::endl;
        m_bounds_.min = vec3();
        m_bounds_.max = vec3();
        return;
    }

        vec3 min = m_positions_[0];
        vec3 max = m_positions_[0];

        for (const vec3& pos : m_positions_) {
            min.x = std::min(min.x, pos.x);
            min.y = std::min(min.y, pos.y);
            min.z = std::min(min.z, pos.z);

            max.x = std::max(max.x, pos.x);
            max.y = std::max(max.y, pos.y);
            max.z = std::max(max.z, pos.z);
        }

        m_bounds_.min = min;
        m_bounds_.max = max;

    //std::cout << "min : " << m_bounds_.min.x << "| "<< m_bounds_.min.y << "| "<< m_bounds_.min.z << "| " << std::endl;
    //std::cout << "max : " << m_bounds_.max.x << "| "<< m_bounds_.max.y << "| "<< m_bounds_.max.z<< "| "<< std::endl;

}
void Chunk::makeInstanceChunkVBO(){
    glGenBuffers(1, &m_instanceVBO_);
    glBindBuffer(GL_ARRAY_BUFFER, m_instanceVBO_);
    glBufferData(GL_ARRAY_BUFFER, m_positions_.size() * sizeof(vec3), m_positions_.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), 0);
    glVertexAttribDivisor(3, 1);

    glBindBuffer(GL_ARRAY_BUFFER,0);

 }




Chunk::~Chunk(){

 }