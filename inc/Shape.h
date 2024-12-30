#pragma once
#include "mat.h"
#include "memory"

class Shape {
    public:
    
    Shape(const vec2& s,const vec2& e) : start(s),end(e){};
    vec2 start,end;
};