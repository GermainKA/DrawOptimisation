#ifndef __GKITUTILS__

#define __GKITUTIS__
#include "mat.h"
#include "mesh.h"
#include "vec.h"
#include "wavefront.h"

#include "orbiter.h"
#include "program.h"
#include "uniforms.h"
#include "draw.h"

#include "app.h"

#include "color.h"
#include "image.h"
#include "image_io.h"
#include "texture.h"
#include <assert.h>
#include <iostream>
#include "app_time.h"
#endif // !__GKITUTILS__
#include <chrono>
#include <fstream>
#include "text.h"
#include "Scene.h"
#include "FB_Objet.h"

Mesh make_frustum( )
        {
            glLineWidth(2);    
            Mesh camera= Mesh(GL_LINES);
            
            camera.color(Yellow());
            // face avant
            camera.vertex(-1, -1, -1);
            camera.vertex(-1, 1, -1);
            camera.vertex(-1, 1, -1);
            camera.vertex(1, 1, -1);
            camera.vertex(1, 1, -1);
            camera.vertex(1, -1, -1);
            camera.vertex(1, -1, -1);
            camera.vertex(-1, -1, -1);
            
            // face arriere
            camera.vertex(-1, -1, 1);
            camera.vertex(-1, 1, 1);
            camera.vertex(-1, 1, 1);
            camera.vertex(1, 1, 1);
            camera.vertex(1, 1, 1);
            camera.vertex(1, -1, 1);
            camera.vertex(1, -1, 1);
            camera.vertex(-1, -1, 1);
            
            // aretes
            camera.vertex(-1, -1, -1);
            camera.vertex(-1, -1, 1);
            camera.vertex(-1, 1, -1);
            camera.vertex(-1, 1, 1);
            camera.vertex(1, 1, -1);
            camera.vertex(1, 1, 1);
            camera.vertex(1, -1, -1);
            camera.vertex(1, -1, 1);
            
            return camera;
        }

class TP : public AppTime
{
public:
    // constructeur : donner les dimensions de l'image, et eventuellement la version d'openGL.
    TP() : AppTime(1024, 640) {
        SDL_GL_SetSwapInterval(0);
    }

    int init()
    {
        frustum = make_frustum();
        m_Scene = new Scene();
        m_shadowBuffer = new FB_Objet(1024,1024,GL_NONE, GL_TEXTURE_2D, 0, GL_LINEAR);
        FB_Objet::staticInit();
        //Point center(m_Scene->getMapWidth()/2,20,20);
        m_center = {0,20,0};

        m_camera.lookat(m_center, 120);

        Point pmin, pmax;
        //m_Scene->Bounds(pmin,pmax);
        //m_camera.lookat(pmin, pmax);

        glClearColor(0.2f, 0.2f, 0.2f, 1.f);        // couleur par defaut de la fenetre

        glClearDepth(1.f);                          // profondeur par defaut
        glDepthFunc(GL_LESS);                       // ztest, conserver l'intersection la plus proche de la camera
        glEnable(GL_DEPTH_TEST);                    // activer le ztest

        return 0;   // ras, pas d'erreur
    }

    // destruction des objets de l'application
    int quit()
    {
        delete m_Scene;
        delete m_shadowBuffer;
        FB_Objet::staticDestroy();
        return 0;
    }

    // dessiner une nouvelle image
    int render()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // recupere l'etat de la souris
        int mx, my;
        unsigned int mb= SDL_GetRelativeMouseState(&mx, &my);

        // deplace la camera
        if(mb & SDL_BUTTON(1))
            m_camera.rotation(mx, my);      // tourne autour de l'objet
        else if(mb & SDL_BUTTON(3))
            m_camera.translation((float) mx / (float) window_width(), (float) my / (float) window_height()); // deplace le point de rotation
        else if(mb & SDL_BUTTON(2))
            m_camera.move(mx);           // approche / eloigne l'objet

        // recupere l'etat de la molette / touch
        SDL_MouseWheelEvent wheel= wheel_event();
        if(wheel.y != 0)
        {
            clear_wheel_event();
            m_camera.move(8.f * wheel.y);  // approche / eloigne l'objet
        }

        // recharge le shader program
        if(key_state('r'))
        {
        }

        //Shadows Pass

        // Transform lightProjection = Ortho(-120, +120, -120, +120, 1, 320);
        // Point lightPos(50, 100, 20);   // position
        // Point lookAt(0, 0, 0);         // cible que regarde la lumière
        // Vector up(0, 1, 0);            // up vector

        Transform lightProjection = Ortho(-10, +10, -10, +10, 1, 30);
        Point lightPos(5, 30, 20);     // position
        Point lookAt(0, 0, 0);         // cible que regarde la lumière
        Vector up(0, 1, 0);            // up vector

        Transform lightView = Lookat(lightPos, lookAt, up);
        Transform lightTrans= Inverse(lightProjection * lightView);

        Vector lightDir = normalize(Vector(lookAt.x - lightPos.x, 
                                   lookAt.y - lightPos.y, 
                                   lookAt.z - lightPos.z));

        m_shadowBuffer->enable();
        glClear(GL_DEPTH_BUFFER_BIT);
        m_Scene->OnDraw(lightProjection, lightView);
        m_shadowBuffer->disable();
        // glClear(GL_DEPTH_BUFFER_BIT);
        // m_shadowBuffer->onDraw(GL_DEPTH_ATTACHMENT);

        // configurer le shader program
        // . recuperer les transformations
        //Transform view = m_camera.view() * RotationX(-90) * Translation(0.0f, 0.0f, -10.0f) * Translation((float)-m_Scene->getMapWidth()/ 2, 0.0f, 0.0f);
        //Transform projection = Perspective(45, float(window_width()) / float(window_height()), 100, 1000);
        // Transform model= RotationX(global_time() / 20) * Scale(10);
        Transform view= m_camera.view();
        Transform projection= m_camera.projection(window_width(), window_height(), 45);
        m_Scene->setLightDir(lightDir);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        m_Scene->OnDraw(projection,view,m_shadowBuffer->getDepthBufferId());
        draw(frustum,lightTrans, m_camera);


        return 1;
    }

protected:
    Transform m_Model;
    Orbiter m_camera;
    Scene * m_Scene;
    Mesh frustum ;
    FB_Objet * m_shadowBuffer ;
    Image image;
    Point m_center ;
    


};


int main(int argc, char** argv)
{
    TP tp;
    tp.run();

    return 0;
}
