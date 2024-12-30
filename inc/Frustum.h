#pragma once
#include "mat.h"
#include "vector"

struct Plane {
    Vector normal; 
    float d; 

   inline float HalfSpace(Vector v) const {
    return dot(normal, v) + d;
    }
};
struct BoundingBox {

    vec3 min, max;
    //bool Intersects(const Frustum& frustum) const;
};

enum {
    TOP = 0,BOTTOM,RIGHT,LEFT,FAR,NEAR
};

class Frustum {
    public:
        Frustum(const Transform& mat4Proj , const Transform& mat4View);
        ~Frustum();
        bool Intersects(const BoundingBox& box) const;
    
    protected :
        void computePlanes();
        void updateClip(const Transform& mat4Proj , const Transform& mat4View);

    private:
    std::vector<Plane> m_planes_{6};
    Transform m_mat4Proj_,m_mat4View_;


};

inline vec4 operator-(const vec4 v1,const vec4 v2){
    return vec4(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z,v1.w - v2.w);
}
inline vec4 operator+(const vec4 v1,const vec4 v2){
    return vec4(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z,v1.w + v2.w);
}