#pragma once

#define MAX(a, b) ((a)>(b)? (a) : (b))
#define MIN(a, b) ((a)<(b)? (a) : (b))

struct MBoundingBox {
    RVector3 min;
    RVector3 max;

    MBoundingBox() : MBoundingBox(RVector3::Zero(), RVector3::Zero()) {}
    MBoundingBox(RVector3 min, RVector3 max) {
        this->min = min;    
        this->max = max;    
    }
    MBoundingBox(BoundingBox box){
        this->min = box.min;
        this->max = box.max;
    }

    operator BoundingBox () const {
        return { min,max };
    }
};
