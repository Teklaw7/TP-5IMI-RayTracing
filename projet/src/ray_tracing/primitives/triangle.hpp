#pragma once

#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "primitive_basic.hpp"
#include "lib/3d/vec3.hpp"


namespace cpe
{

class triangle : public primitive_basic
{
public:

    triangle(vec3 const& A,vec3 const& B,vec3 const& C,vec3 const& nA,vec3 const& nB,vec3 const& nC);

    /** One point of the plane */
//    vec3 const& position() const;
    vec3 const& P_A() const;
    vec3 const& P_B() const;
    vec3 const& P_C() const;
    vec3 const& N_A() const;
    vec3 const& N_B() const;
    vec3 const& N_C() const;
    /** Normal of the plance */
//    vec3 const& normal() const;

    /** Intersection computation with a ray */
    bool intersect(ray const& ray_param,intersection_data& intersection) const override;


private:

    /** One point belonging to the plane */
    vec3 Point_A;
    /** Normal of the plane */
    vec3 Point_B;
    vec3 Point_C;
    vec3 Normal_A;
    vec3 Normal_B;
    vec3 Normal_C;
};

}

#endif
