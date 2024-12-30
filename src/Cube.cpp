#include "Cube.h"
#include "glcore.h"


/*Constructeur*/
Cube::Cube(const char * shaderfile , const char * shadowShaderfile):m_ShaderFilname(shaderfile) , m_shawdowShaderFilname(shadowShaderfile){
    mesh = new Mesh(read_mesh("TP/TP2/data/cube.obj"));
    if (!mesh->vertex_count()) return ;
    
    m_ShadowsShaderId = read_program(m_shawdowShaderFilname.data());
    program_print_errors(m_ShadowsShaderId);
    m_ShaderId = read_program(m_ShaderFilname.data());
    program_print_errors(m_ShaderId);

    m_VertexLocation = glGetAttribLocation(m_ShaderId, "aPosition");
    m_NormalLocation = glGetAttribLocation(m_ShaderId, "aNormal");
    m_TexCoordLocation = glGetAttribLocation(m_ShaderId, "aTexCoord");
    m_ViewMatLocation = glGetUniformLocation(m_ShaderId, "mat4View");
    m_ProjectionMatLocation = glGetUniformLocation(m_ShaderId, "mat4Proj");

    glGenVertexArrays(1, &m_VertexArrayId);
    glBindVertexArray(m_VertexArrayId);

    glGenBuffers(1, &m_VertexBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferId );
    glBufferData(GL_ARRAY_BUFFER, mesh->vertex_buffer_size(), mesh->vertex_buffer(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &m_NormalBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, m_NormalBufferId);
    glBufferData(GL_ARRAY_BUFFER, mesh->normal_buffer_size(), mesh->normal_buffer(), GL_STATIC_DRAW);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &m_TextCoordBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, m_TextCoordBufferId);
    glBufferData(GL_ARRAY_BUFFER, mesh->texcoord_buffer_size(), mesh->texcoord_buffer(), GL_STATIC_DRAW);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);



    // conserve le nombre de sommets
    m_VertexCount = mesh->vertex_count();

    //Desactive le Vao
    glBindVertexArray(0);
}

Cube::~Cube()
{
    glDeleteBuffers(1, &m_VertexBufferId);
    glDeleteBuffers(1, &m_NormalBufferId);
    glDeleteBuffers(1, &m_TextCoordBufferId);
    glDeleteVertexArrays(1, &m_VertexArrayId);
    delete mesh;
    release_program(m_ShaderId);
}

void Cube::onDrawBase(Transform& projection,Transform& view) const {
    //Active
    glUseProgram(m_ShaderId);


    //Attach Matrix
    glUniformMatrix4fv(m_ViewMatLocation, 1, GL_TRUE, view.data());
    glUniformMatrix4fv(m_ProjectionMatLocation, 1, GL_TRUE, projection.data());
    //Attach Attrib
    //glUniform4f(m_ColorLocation, 0.5, 0.5, 0.5, 1);

    glBindVertexArray(m_VertexArrayId);
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferId );
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_NormalBufferId);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glDrawArrays(GL_TRIANGLES, 0, m_VertexCount);
    glBindVertexArray(0);


}