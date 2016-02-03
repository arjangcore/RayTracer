//
//  Framework for a raytracer
//  File: scene.cpp
//
//  Created for the Computer Science course "Introduction Computer Graphics"
//  taught at the University of Groningen by Tobias Isenberg.
//
//  Authors:
//    Reza Nezami
//    Maarten Everts
//    Jasper van de Gronde
//
//  This framework is inspired by and uses code of the raytracer framework of 
//  Bert Freudenberg that can be found at
//  http://isgwww.cs.uni-magdeburg.de/graphik/lehre/cg2/projekt/rtprojekt.html 
//

#include "scene.h"
#include "material.h"
#include <math.h>

double MaxFound = 1.0;
Color Scene::trace(const Ray &ray)
{
    // Find hit object and distance
    Hit min_hit(std::numeric_limits<double>::infinity(),Vector());
    Object *obj = NULL;
    for (unsigned int i = 0; i < objects.size(); ++i) 
	{
        Hit hit(objects[i]->intersect(ray));
        if (hit.t<min_hit.t) 
		{
            min_hit = hit;
            obj = objects[i];
        }
    }

	Color color(0.0, 0.0, 0.0);
	// No hit? Return background color.
	if (!obj)
		return color;

    Material *material = obj->material;            //the hit objects material
    Point hit = ray.at(min_hit.t);                 //the hit point
    Vector N = min_hit.N;                          //the normal at hit point
    Vector V = -ray.D;                             //the view vector

	if(mode == Phong)
	{
		/****************************************************
		* This is where you should insert the color
		* calculation (Phong model).
		*
		* Given: material, hit, N, V, lights[]
		* Sought: color
		****************************************************/

		color = material->ka * material->color;     // ambient term
		for (auto &l : lights)  // add diffuse and specular terms for all lights
		{
			// first add diffuse for this light
			Vector li = l->position - hit;
			li.normalize();
			double lDotN = li.dot(N);
			color += (lDotN * material->kd) * material->color;

			// add specular terms for all lights
			Vector ri = 2.0 * (lDotN * N) - li;
			ri.normalize();
			color += pow(ri.dot(ray.D), material->n)* material->ks * material->color;
		}
	}
	else if (mode == ZBuffer)
	{
		double proj = ray.D.dot(Triple(0.0, 0.0, -1.0));
		MaxFound = fmaxl(MaxFound, (min_hit.t * proj));
		double depth = (min_hit.t * proj)/ maxDepth;
		color.set(depth, depth, depth);
	}
	else if (mode == Normal)
	{
		color = N / 2.0 + 0.5;
	}
    return color;
}

void Scene::render(Image &img)
{
    int w = img.width();
    int h = img.height();
    for (int y = 0; y < h; y++) 
	{
        for (int x = 0; x < w; x++) 
		{
            Point pixel(x+0.5, h-1-y+0.5, 0);
            Ray ray(eye, (pixel-eye).normalized());
            Color col = trace(ray);
            col.clamp();
            img(x,y) = col;
        }
    }
}

void Scene::addObject(Object *o)
{
    objects.push_back(o);
}

void Scene::addLight(Light *l)
{
    lights.push_back(l);
}

void Scene::setEye(Triple e)
{
    eye = e;
}
