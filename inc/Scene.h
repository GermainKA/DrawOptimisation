#pragma once
#include <iostream>
#include <string>
#include <memory>
#include "mat.h"
#include "Cube.h"
#include "mesh.h"
#include "Texture.h"
#include "Map.h"



class Scene {

public:


     Scene();

    ~Scene(); 

    void OnDraw(const Transform& mat4Projection, const Transform& mat4View,GLuint deepBuffer = NULL);
    void Bounds(Point &pmin,Point &pmax) ;
    int getMapWidth(){return m_Map_->width();}
    int getMapHeight(){return m_Map_->height();}
    void setLightDir(Vector& lightdir ){ m_lightDir = lightdir ;}

protected:
    void Initializer();

private:
    std::unique_ptr<Cube> m_Cube_ = nullptr;
    std::unique_ptr<Map> m_Map_ = nullptr;
    std::unique_ptr<Texture> m_Texture_ = nullptr;
    std::unique_ptr<Frustum> m_Frustum_ = nullptr;
    Transform m_mat4ligth1VP_ ;


    // Shaders ID
    GLuint m_ShaderSceneID_ = 0;
    GLuint m_SceneVAO_ = 0;

    //Ligth
    Vector m_lightDir ;

};
