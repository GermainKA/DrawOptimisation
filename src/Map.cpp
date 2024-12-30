#include <map>
#include "Map.h"
#include "image_io.h"
#include "mat.h"



Map::Map(const char* deepMapfilename)
        : m_deepMapfilename_(deepMapfilename) {
        Initializer();
        computeBoundingBox();
}

void Map::Initializer(){
    Image image =  read_image(m_deepMapfilename_.data());
    m_mapWidth_ = image.width() ;
    m_mapHeight_ = image.height() ;

    
    Color color;
    vec3 pos;
    float h;

    // map with position / chunks
    std::map<std::pair<int, int>, std::vector<vec3>> chunkPositions;

    for (int x = 0; x < image.width(); x++) {
        for (int y = 0; y < image.height(); y++) {
            color = image(x, y);
            h = (color.r + color.b + color.g) / 3;
    
            h = std::round(h * 20);

            pos = { 
                (float)x - image.width() / 2, // Alignement X
                h,                           // Hauteur
                (float)y - image.height() / 2 // Alignement Y
            };
            //pos = { (float)x - image.width() /2,(float)h * 20, (float)y - image.height() /2};
            m_instancesPos_.push_back(pos);

            //Add Pos to corresp Chunk
            int chunkX = x / CHUNK_SIZE;
            int chunkY = y / CHUNK_SIZE;
            chunkPositions[{chunkX, chunkY}].push_back(pos);
        }
    }

    // Create Chunks
    for (const auto& entry : chunkPositions) {
        int chunkX = entry.first.first * CHUNK_SIZE;
        int chunkY = entry.first.second * CHUNK_SIZE;
        m_chunks_.emplace_back(Chunk(chunkX, chunkY, CHUNK_SIZE, CHUNK_SIZE, entry.second));
    }
}




void Map::initInstanceMapVBO() {
    // Configurer l'attribut d'instance
    glGenBuffers(1, &m_instanceVBO_);
    glBindBuffer(GL_ARRAY_BUFFER, m_instanceVBO_);
    glBufferData(GL_ARRAY_BUFFER, m_instancesPos_.size() * sizeof(vec3), nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), 0);
    glVertexAttribDivisor(3, 1);

    glBindBuffer(GL_ARRAY_BUFFER,0);
}

void Map::deleteInstanceMapVBO(){
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glDeleteBuffers(1,&m_instanceVBO_);
}
void Map::computeBoundingBox(bool isRegularGrid) {
    if (m_instancesPos_.empty()) {
        std::cerr<<"Warning Cmpt AABB: Empty Map Positions" << std::endl;
        m_bounds_.min = vec3();
        m_bounds_.max = vec3();
        return;
    }

    vec3 min = m_instancesPos_[0];
    vec3 max = m_instancesPos_[0];

    for (const vec3& pos : m_instancesPos_) {
        min.x = std::min(min.x, pos.x);
        min.y = std::min(min.y, pos.y);
        min.z = std::min(min.z, pos.z);

        max.x = std::max(max.x, pos.x);
        max.y = std::max(max.y, pos.y);
        max.z = std::max(max.z, pos.z);
    }

    m_bounds_.min = min;
    m_bounds_.max = max;

}

Map::~Map(){

}