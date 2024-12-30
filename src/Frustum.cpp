#include "Frustum.h"

Frustum::Frustum(const Transform& mat4Proj , const Transform& mat4View):m_mat4Proj_(mat4Proj),m_mat4View_(mat4View){
    
    computePlanes();
};

void Frustum::updateClip(const Transform& mat4Proj , const Transform& mat4View){
    m_mat4Proj_ = mat4Proj;
    m_mat4View_ = mat4View;
    computePlanes();
}
bool Frustum::Intersects(const BoundingBox& box) const
{
    const vec3& min = box.min;
    const vec3& max = box.max;

    for (const Plane& plane : m_planes_)
    {
        vec3 p, n;
        //Point plus proche du plan 
        p.x = (plane.normal.x >= 0.0f) ? max.x : min.x;
        p.y = (plane.normal.y >= 0.0f) ? max.y : min.y;
        p.z = (plane.normal.z >= 0.0f) ? max.z : min.z;

        n.x = (plane.normal.x >= 0.0f) ? min.x : max.x;
        n.y = (plane.normal.y >= 0.0f) ? min.y : max.y;
        n.z = (plane.normal.z >= 0.0f) ? min.z : max.z;

        float distP = dot(plane.normal, p) + plane.d;
        if (distP < 0.0f) {
            return false; 
        }
    }
    return true;
}

void Frustum::computePlanes(){
    Transform mat4Clip = m_mat4Proj_ * m_mat4View_;


    // Top plane
    auto vec = mat4Clip.row(3) - mat4Clip.row(1);
    m_planes_[TOP].normal = vec3(vec.x,vec.y,vec.z);
    m_planes_[TOP].d = vec.w;
    
    // Bottom plane
    vec = mat4Clip.row(3) + mat4Clip.row(1);
    m_planes_[BOTTOM].normal = vec3(vec.x,vec.y,vec.z);
    m_planes_[BOTTOM].d = vec.w;

    // Right plane
    vec = mat4Clip.row(3) - mat4Clip.row(0);
    m_planes_[RIGHT].normal = vec3(vec.x,vec.y,vec.z);
    m_planes_[RIGHT].d = vec.w;
    // Left plane
    vec = mat4Clip.row(3) + mat4Clip.row(0);
    m_planes_[LEFT].normal = vec3(vec.x,vec.y,vec.z);
    m_planes_[LEFT].d = vec.w;

    // Near plane
    vec = mat4Clip.row(3) + mat4Clip.row(2);
    m_planes_[NEAR].normal = vec3(vec.x,vec.y,vec.z);
    m_planes_[NEAR].d = vec.w;

    // Far plane
    vec = mat4Clip.row(3) - mat4Clip.row(2);
    m_planes_[FAR].normal = vec3(vec.x,vec.y,vec.z);
    m_planes_[FAR].d = vec.w;
    
    // Normalize the planes
    for (auto& plane : m_planes_) {
       float norm = length(plane.normal);
        plane.normal = normalize(plane.normal);
        plane.d /= norm;

        //std::cout << " |x : " << plane.normal.x << " |y: " <<plane.normal.y<< " |z: " <<plane.normal.z <<" |d: " << plane.d <<  " | " << std::endl;
    }
       // std::cout <<"-----------------------------" <<std::endl;


}


Frustum::~Frustum(){

}

