/*
**    TP CPE Lyon
**    Copyright (C) 2015 Damien Rohmer
**
**    This program is free software: you can redistribute it and/or modify
**    it under the terms of the GNU General Public License as published by
**    the Free Software Foundation, either version 3 of the License, or
**    (at your option) any later version.
**
**   This program is distributed in the hope that it will be useful,
**    but WITHOUT ANY WARRANTY; without even the implied warranty of
**    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**    GNU General Public License for more details.
**
**    You should have received a copy of the GNU General Public License
**    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "sphere.hpp"

#include "intersection_data.hpp"
#include "../scene/ray.hpp"
#include "lib/common/error_handling.hpp"

#include <cmath>

namespace cpe
{

sphere::sphere(vec3 const& center_param,float radius_param)
    :center_data(center_param),radius_data(radius_param)
{}

vec3 const& sphere::center() const
{
    return center_data;
}
float sphere::radius() const
{
    return radius_data;
}

bool sphere::intersect(ray const& ray_param,intersection_data& intersection) const
{

    vec3 const& u = ray_param.u();

    // ********************************************************** //
    // ********************************************************** //
    //  TO DO:
    //    Calcul d'intersection entre un rayon et une plan
    //
    // Variales:
    //  - Position initiale du rayon: ray_param.p0()
    //  - Vecteur directeur unitaire du rayon: u
    //  - Position du centre de la sphere: center_data
    //  - Rayon de la sphere: radius_data
    //
    // Aide de syntaxe:
    //  - Norme au carre d'un vecteur v: float norme=v.norm2();
    //             ou: float norme=v.dot(v);
    //
    // ********************************************************** //
    // ********************************************************** //



    //Le code suivant est arbitraire est doit etre modifie
    vec3 const& p0 = ray_param.p0();
    vec3 const& c = center_data;
    float const& r = radius_data;
    float delta = (2*dot(u,p0-c)*2*dot(u,p0-c))-4*(dot(p0-c,p0-c)-r*r);;
//    float f=sqrt(4.0f*dot(p0-center_data,u)*dot(p0-1.1f*center_data,u)-3.5f*dot(p0-center_data,p0-center_data));

    double t_inter;

    if(delta<0){
        return false;
    }
    if (delta==0){
        t_inter= -dot(u,p0-c);
        if(t_inter<0)
            return false;

    }
    else{
        double t_inter_1= (-2*dot(u,p0-c)-sqrt(delta))/2;
        double t_inter_2=(-2*dot(u,p0-c)+sqrt(delta))/2;
        if (t_inter_1<0 && t_inter_2<0)
            return false;
        if (t_inter_1<t_inter_2 && t_inter_1>0)
           t_inter=t_inter_1;
        if (t_inter_2<t_inter_1 && t_inter_2>0)
            t_inter=t_inter_1;
        }
    vec3 p_inter = p0 +t_inter*u;
    vec3 n = (p_inter-c)/r;
    intersection.set(p_inter,n,t_inter);
    return true;

}



}
