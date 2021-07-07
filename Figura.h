#pragma once
#include <GL/freeglut.h>  
#include <math.h>
#include "vector3d.h"
#include "vector4d.h"
#include "matrix4x4.h"
#include <vector>
class Figura
{
public:
	 int ind;
	 GLuint texture;
	 double visina;
	virtual void Draw()=0;
	virtual void Move(Vector3D v)=0;
	virtual void DrawPicker()=0;
};

