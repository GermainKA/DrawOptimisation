#ifndef __GKITUTILS__

#define __GKITUTILS__
#include "mat.h"
#include "mesh.h"
#include "wavefront.h"

#include "orbiter.h"
#include "program.h"
#include "uniforms.h"
#include "draw.h"

#include "app.h"

#include "color.h"
#include "image.h"
#include "image_io.h"
#include <assert.h>
#include <iostream>
#endif // !__GKITUTILS__

#ifndef __CUBE__
#define  __CUBE__

class Cube
{

protected:
	/*Tableau des coordonnees */
	Mesh * mesh ;
	GLuint m_VertexArrayId;
	GLuint m_VertexBufferId;
	GLuint m_NormalBufferId;
	GLuint m_TextCoordBufferId;
	GLuint m_ElementBufferId;
	int m_VertexCount;

	/*Identifiants liees au shader */
	GLuint m_ShaderId;
	GLuint m_ShadowsShaderId;
	GLuint m_ViewMatLocation;
	GLuint m_ProjectionMatLocation;
	GLuint m_VertexLocation;
	GLuint m_NormalLocation;
	GLuint m_TexCoordLocation;
	std::string m_ShaderFilname ;
	std::string m_shawdowShaderFilname ;


public:
	/*Constructeur,cree le VAO,VBOs et le programShader*/
	Cube(const char * shaderfile = "TP/TP2/shaders/cube_shader_scene.glsl", const char * shadowShaderfile = "TP/TP2/shaders/drawShadows.glsl");
	virtual ~Cube();

	
	GLuint getVertexArrayId() const { return m_VertexArrayId; };
	GLuint getShaderId() const { return m_ShaderId; };
	GLuint getShadowShaderId() const { return m_ShadowsShaderId; };
    GLint getVertexLocation() const { return m_VertexLocation; }
    GLint getNormalLocation() const { return m_NormalLocation; }
	GLuint getViewMatrixLocation() const { return m_ViewMatLocation; };
	GLuint getProjectionMatrixLocation() const { return m_ProjectionMatLocation; };
	int getVertexCount() { return m_VertexCount; };

	GLuint getVertexBufferId() const { return m_VertexBufferId; }
    GLuint getNormalBufferId() const { return m_NormalBufferId; }
    GLuint getTexCoordBuffeId() const { return m_TextCoordBufferId;}


	virtual void onDrawBase(Transform& projection ,Transform& view) const;

};

#endif //  !__CUBE__