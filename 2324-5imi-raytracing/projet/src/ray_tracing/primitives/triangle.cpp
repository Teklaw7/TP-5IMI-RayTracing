#include "triangle.hpp"
#include "intersection_data.hpp"
#include "../scene/ray.hpp"
#include "lib/common/error_handling.hpp"
#include <cmath>


namespace cpe
{

triangle::triangle(vec3 const& A,vec3 const& B,vec3 const& C,vec3 const& nA,vec3 const& nB, vec3 const& nC)
    :Point_A(A),Point_B(B),Point_C(C),Normal_A(nA),Normal_B(nB),Normal_C(nC)
{

}

vec3 const& triangle::P_A() const
{
    return Point_A;
}
vec3 const& triangle::P_B() const
{
    return Point_B;
}
vec3 const& triangle::P_C() const
{
    return Point_C;
}
vec3 const& triangle::N_A() const
{
    return Normal_A;
}
vec3 const& triangle::N_B() const
{
    return Normal_B;
}
vec3 const& triangle::N_C() const
{
    return Normal_C;
}

bool triangle::intersect(ray const& ray_param,intersection_data& intersection) const
{

    vec3 const& u = ray_param.u();
//    vec3 const& n = normal_data;
    auto const& p0 = ray_param.p0();

    bool inter = false;
    vec3 const& A = Point_A;
    vec3 const& B = Point_B;
    vec3 const& C = Point_C;

    vec3 x10 = B-A;
    vec3 x20 = C-A;
    vec3 u10 = normalized(x10);
    vec3 u20 = normalized(x20);
    vec3 n = normalized(cross(u10,u20));

    float proj = dot(u,n);
    double eps = 1e-5;
//    if(std::fabs(proj)<eps)
//        return false;
    double t_inter = dot((A-ray_param.p0()),n)/proj;
    if(t_inter<0.0f)
        return false;

    vec3 p_inter = ray_param(t_inter);
    double area_0 = norm(cross((C-B),p_inter-B))/2.0;
    double area_1 = norm(cross((A-C),p_inter-C))/2.0;
    double area_2 = norm(cross((B-A),p_inter-A))/2.0;
    double area = norm(cross(x10,x20))/2.0;
    double a = area_0/area;
    double b = area_1/area;
    double c = area_2/area;

    if ((a >= 0 && b >= 0 && c >= 0 && a <= 1 && b <= 1 && c <= 1) && (std::fabs(a+b+c-1.0f) <= eps))
    {
        n = dot(n, u) > 0 ? -n : n;
        intersection.set(p_inter,n,t_inter);

        return true;
    }
    return false;
}

}
