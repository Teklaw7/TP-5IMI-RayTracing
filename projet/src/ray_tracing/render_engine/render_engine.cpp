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

#include "render_engine.hpp"

#include "image/image.hpp"
#include "ray_tracing/scene/scene_parameter.hpp"
#include "ray_tracing/scene/ray.hpp"
#include "ray_tracing/primitives/intersection_data.hpp"
#include "ray_tracing/scene/anti_aliasing_table.hpp"

#include <cmath>

namespace cpe
{


void render(image& im,scene_parameter const& scene)
{
    // **************************************************************************** //
    //
    // Current Algorithm:
    //
    // For all pixels of the image
    //    Generate ray from the camera toward in the direction of the current pixel
    //    Compute associated color (ray tracing algorithm)
    //    Set the color to the current pixel
    //
    // **************************************************************************** //

    camera const& cam = scene.get_camera();

    int const Nx = im.Nx();
    int const Ny = im.Ny();

    // loop over all the pixels of the image
    int const N_sample = 5;
    anti_aliasing_table aa(N_sample);
    for(int kx=0 ; kx<Nx ; ++kx)
    {
        float const u = static_cast<float>(kx)/(Nx-1);
        for(int ky=0 ; ky<Ny ; ++ky)
        {
            float const v = static_cast<float>(ky)/(Ny-1);
            //with anti aliasing
            color c {0.0f,0.0f,0.0f};
            for(int dx=0 ; dx<N_sample ; ++dx)
            {
                for(int dy=0 ; dy<N_sample ; ++dy)
                {
                    float const du = aa.displacement(dx)/(Nx-1); //Nx is the size in pixel in x direction
                    float const dv = aa.displacement(dy)/(Ny-1); //Ny is the size in pixel in y direction

                    float const w = aa.weight(dx,dy);
                    ray r = ray_generator(cam, u+du,v+dv);
                    c += w*ray_trace(r,scene); //F is a function of (u,v)
                }
            }
            im({kx,ky}) = c;
            /* without anti aliasing
            // generate ray and compute color
            ray const r = ray_generator(cam,u,v);
            color const c = ray_trace(r,scene);
            im({kx,ky}) = c;*/
        }
    }

}


ray ray_generator(camera const& cam,float const u,float const v)
{
    // position of the sample on the screen in 3D
    vec3 const p_screen = screen_position(cam,u,v);

    // vector "camera center" to "screen position"
    vec3 const d = p_screen-cam.center();

    // compute the ray
    ray const r(cam.center(),d);

    return r;
}

color ray_trace(ray const& r,scene_parameter const& scene)
{
    // ********************************************* //
    // ********************************************* //
    //
    // TO DO: Calculer la couleur associee a un rayon dans la scene 3D
    //
    // ********************************************* //
    // ********************************************* //
    //Le code suivant affecte la couleur de base de la premiere intersection trouvee
    //Ce code ne gere pas les reflections.


    intersection_data intersection; //current intersection
    int intersected_primitive = 0;  //current index of intersected primitive
    color c_trace ;
    float att = 1;

    bool is_intersection = compute_intersection(r,scene,intersection,intersected_primitive);
    if(is_intersection) //if the ray intersects a primitive
    {
        material const& mat = scene.get_material(intersected_primitive);

        c_trace += att*compute_illumination(mat,intersection,scene);

        for (int k =0; is_intersection && k<5 ; k++)
        {
           material const& mat = scene.get_material(intersected_primitive);

           att *= 0.7*mat.reflection();
           c_trace += att * compute_illumination(mat,intersection,scene);

           vec3 u_sym = reflected(-r.u(),normalized(intersection.normal));
           ray r_sym(intersection.position,u_sym);
           r_sym.offset();

           is_intersection = compute_intersection(r_sym,scene,intersection,intersected_primitive);
        }

        return c_trace;
    }
    else
        return color(0,0,0); //no intersection
}


bool compute_intersection(ray const& r,
                          scene_parameter const& scene,
                          intersection_data& intersection,
                          int& index_intersected_primitive)
{
    // ********************************************* //
    // ********************************************* //
    //
    // TO DO: Calculer l'intersection valide la plus proche
    //        Doit retourner vrai/faux si une intersection est trouvee ou non
    //        Doit mettre a jour intersection et index_intersected_primitive avec l'intersection la plus proche
    //
    // ********************************************* //
    // ********************************************* //

    //Le code suivant renvoie la premiere intersection valide que l'on trouve dans l'ordre de stockage du vecteur
    //Ce code est a modifier afin de trouver la 1ere intersection dans l'espace 3D le long du rayon.

    int const N_primitive = scene.size_primitive();
    intersection_data intersection_temp;

    bool found_intersection = false;
    int k = 0;

    for(int k = 0; k<N_primitive;k++){
        primitive_basic const & primitive = scene.get_primitive(k);
        bool is_intersection = primitive.intersect(r,intersection_temp);
        if(is_intersection){
//            std::cout<<"coucou"<<std::endl;
            if( (found_intersection==false) || intersection_temp.relative < intersection.relative){
//                std::cout<<intersection_temp.relative<<std::endl;
//                std::cout<<intersection.relative<<std::endl;
                intersection=intersection_temp;
                index_intersected_primitive=k;
                found_intersection=true;
            }
        }

    }
    return found_intersection;


//    while(k<N_primitive && found_intersection==false)
//    {
//        primitive_basic const & primitive = scene.get_primitive(k);
//        bool is_intersection = primitive.intersect(r,intersection);
//        if(is_intersection)
//            found_intersection = true;
//        else
//            ++k;
//    }
//    index_intersected_primitive = k;

//    return found_intersection;
}


bool is_in_shadow(vec3 const& p,vec3 const& p_light, scene_parameter const& scene)
{
    // ********************************************* //
    //
    // TO DO: Calculer si le point p est dans l'ombre de la lumiere situee en p_light
    //
    // ********************************************* //
    vec3 ombre = p_light-p;
    ray r(p,ombre/norm(ombre));
    r.offset();
    intersection_data intersection;
    int intersected_primitive = 0;
    bool is_intersection = compute_intersection(r,scene,intersection,intersected_primitive);
    return is_intersection;

}



color compute_illumination(material const& mat,intersection_data const& intersection,scene_parameter const& scene)
{
    // ********************************************* //
    //
    // TO DO: Mettre en place le calcul d'illumination correct
    //
    //   Pour toutes les lumieres
    //     Si point dans l'ombre
    //       Ajouter a couleur courante une faible part de la couleur du materiau
    //          ex. c += mat.color_object()/10.0f;
    //     Sinon
    //       Calculer illumination au point courant en fonction de la position
    //          de la lumiere et de la camera
    //       Ajouter a couleur courante la contribution suivante:
    //           puissance de la lumiere (L.power)
    //         X couleur de la lumiere (L.c)
    //         X valeur de l'illumination
    //
    // ********************************************* //

    color c;

    vec3 const& p0 = intersection.position;
    camera const& camera = scene.get_camera();
    int const N_light = scene.size_light();
    for(int k=0; k<N_light ; ++k)
    {
        light const& L = scene.get_light(k);
        bool const shadow = is_in_shadow(p0,L.p,scene);
        if(shadow==true){
            color illumination = mat.color_object()/10;
            c += L.power*illumination;
        }
        else{
            color illumination = compute_shading(mat.shading(),mat.color_object(),p0,L.p,intersection.normal, camera.center());
            c += L.power*L.c*illumination;
        }


    }

    return c;


}





}


