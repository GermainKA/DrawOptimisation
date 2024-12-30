#pragma once
#include <iostream>
#include "image_io.h"
#include "Texture.h"

Texture::Texture(int nbLayer) : m_LayerCount(nbLayer){

    loadArrayTextures();

}


void Texture::loadArrayTextures(){
    const std::vector<std::string> textureFiles = { "TP/TP2/data/texture0.png",
                                                    "TP/TP2/data/texture1.png",
                                                    "TP/TP2/data/texture2.png",
                                                    "TP/TP2/data/texture3.png",
                                                    "TP/TP2/data/texture4.png",
                                                    "TP/TP2/data/texture5.png",
                                                    "TP/TP2/data/texture6.png",
                                                    "TP/TP2/data/texture7.png",
                                                    "TP/TP2/data/texture8.png",
                                                    "TP/TP2/data/texture9.png"};
    loadArrayTextures(textureFiles);
}


void Texture::loadArrayTextures(const std::vector<std::string>& textureFiles){

    glGenTextures(1, &m_TexArraysId);
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_TexArraysId);

    if(textureFiles.size() < m_LayerCount) {
        std::cout << "Lower Texture Files";
        return ;
    }

    ImageData image = read_image_data(textureFiles[0].c_str());
    int width = image.width;
    int height = image.height;
    GLenum data_format;
    GLenum data_type= GL_UNSIGNED_BYTE;
    if(image.channels == 3)
        data_format= GL_RGB;
    else // par defaut
        data_format= GL_RGBA;

    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, data_format, width, height, m_LayerCount, 0, GL_RGBA, data_type, nullptr);

    for(int i=0;i<m_LayerCount;++i){
        
        image = read_image_data(textureFiles[i].c_str());
       // std::cout << "LOad : " << image.channels << std::endl;
        if(image.width != width || image.height!= height){
            std::cerr << "Error: Texture size mismatch " << textureFiles[i] << std::endl;
            return;
        }
        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, image.data());
    }

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);


  }

void Texture::setTexArrayUnit(){
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_TexArraysId);
}