//
//  Framework for a raytracer
//  File: scene.h
//
//  Created for the Computer Science course "Introduction Computer Graphics"
//  taught at the University of Groningen by Tobias Isenberg.
//
//  Authors:
//    Maarten Everts
//    Jasper van de Gronde
//
//  This framework is inspired by and uses code of the raytracer framework of 
//  Bert Freudenberg that can be found at
//  http://isgwww.cs.uni-magdeburg.de/graphik/lehre/cg2/projekt/rtprojekt.html 
//
#pragma once

#include <vector>
#include "triple.h"
#include "light.h"
#include "object.h"
#include "image.h"

enum RenderMode
{
	Phong,      // lighting mode using Phone shading.
	ZBuffer,     // mode to generate depth buffer
	Normal     // mode to generate normal buffer
};
class Scene
{
private:
    std::vector<Object*> objects;
    std::vector<Light*> lights;
    Triple eye;
	RenderMode mode;
	double maxDepth;

public:
	Scene() : mode(Normal), maxDepth(1000.) {}
    Color trace(const Ray &ray);
    void render(Image &img);
    void addObject(Object *o);
    void addLight(Light *l);
    void setEye(Triple e);
    unsigned int getNumObjects() { return objects.size(); }
    unsigned int getNumLights() { return lights.size(); }
	void setMode(RenderMode c) { mode = c; }
	RenderMode getMode() { return mode; }
};
