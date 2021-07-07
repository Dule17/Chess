#pragma once
#include <GL/freeglut.h>  
#include <math.h>
#include "vector3d.h"
#include "vector4d.h"
#include "matrix4x4.h"
#include <vector>
#include "Figura.h"
#include <SOIL.h>
class Pesak:public Figura
{
public:
	int brtacakaPesak;
	
	vector< vector< vector<Vector3D> > > figura;
	Pesak(void);
	Pesak(int N,double r1,double r2,double r3,double r4,double d1,double d2,Vector3D V,int boja);
	virtual void Draw();
	virtual void Move(Vector3D v);
	virtual void DrawPicker();
	~Pesak(void);
};

