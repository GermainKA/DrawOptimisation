#include "Scene.h"

Scene::Scene()
    : m_Cube_(std::unique_ptr<Cube>(new Cube("TP/TP2/shaders/cube_shader_scene.glsl"))),
      m_Map_(std::unique_ptr<Map>(new Map())),m_Texture_(std::unique_ptr<Texture>(new Texture())) {
    Initializer();
}
void Scene::Bounds(Point &pmin,Point &pmax) {
    pmin = m_Map_->getBoundingBox().min;
    pmax = m_Map_->getBoundingBox().max;
}

void Scene::Initializer(){
    if (!m_Cube_->getVertexCount()) return ;
    glBindVertexArray(m_Cube_->getVertexArrayId());
    m_Map_->initInstanceMapVBO();
    m_Texture_->setTexArrayUnit();
    //Desactive le Vao
    glBindVertexArray(0);
}

Scene::~Scene(){

}

void Scene::OnDraw(const Transform& mat4Projection,const Transform& mat4View,GLuint deepBuffer ) {
    m_Frustum_ = std::unique_ptr<Frustum>(new Frustum(mat4Projection,mat4View));

    Transform model = Identity();
    GLuint  programShader;
    if (!deepBuffer){
        programShader = m_Cube_->getShaderId();
        glUseProgram(programShader);
        this->m_mat4ligth1VP_ = mat4Projection * mat4View ;
    }else{
        if (!glIsTexture(deepBuffer)) {
             std::cerr << "Erreur : deepBuffer n'est pas une texture valide !" << std::endl;
             exit(EXIT_FAILURE);
        }
         programShader = m_Cube_->getShadowShaderId();
         glUseProgram(programShader);
         glUniformMatrix4fv(glGetUniformLocation(programShader, "mat4lightVP"),1,GL_TRUE,m_mat4ligth1VP_.data());
    }

    //Attach Matrix
    glUniformMatrix4fv(m_Cube_->getViewMatrixLocation(), 1, GL_TRUE, mat4View.data());
    glUniformMatrix4fv(m_Cube_->getProjectionMatrixLocation(), 1, GL_TRUE, mat4Projection.data());
            
    glBindVertexArray( m_Cube_->getVertexArrayId());  
    glBindBuffer(GL_ARRAY_BUFFER, m_Map_->getInstanceVboId());
    bool instanced = false ;
    size_t offset = 0;
    for(int i =0;i<m_Map_->m_chunks_.size();++i){
        if(m_Frustum_->Intersects(m_Map_->m_chunks_[i].getBoundingBox())){
            const auto& positions = m_Map_->m_chunks_[i].m_positions_;
            glBufferSubData(GL_ARRAY_BUFFER, offset, positions.size() * sizeof(vec3), positions.data());
            offset += positions.size() * sizeof(vec3);
            //std::cout << "Chunk : " << i << " Added | Size: " << positions.size() << std::endl;
            instanced = true;
        }
    }


    GLint texArrayLoc= glGetUniformLocation(programShader, "textureArray");
    glUniform1i(texArrayLoc, 0);  
    //Select Texture :
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_Texture_->getTexArraysId());  

    //Add Shadows Texture & Ligth
    if(deepBuffer){
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, deepBuffer);
        GLint shadowMapLoc = glGetUniformLocation(programShader, "shadowMap");
        glUniform1i(shadowMapLoc, 1);
        GLint lightDirLoc = glGetUniformLocation(programShader, "lightDir");
        glUniform3f(lightDirLoc, m_lightDir.x, m_lightDir.y, m_lightDir.z);


    }

    if(instanced)
        glDrawArraysInstanced(GL_TRIANGLES, 0, m_Cube_->getVertexCount(),offset/sizeof(vec3));
    glBindVertexArray(0);

    glUseProgram(0);


}