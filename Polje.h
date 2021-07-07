#pragma once
#include <GL/freeglut.h>  
#include <math.h>
#include "vector3d.h"
#include "vector4d.h"
#include "matrix4x4.h"
#include <vector>
#include "Figura.h"
class Polje
{
public:
	Figura* figura;
	Vector3D t1,t2,t3,t4;
	int r,g,b;
	Polje(void);
	~Polje(void);
};

