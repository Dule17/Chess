#pragma once
#include <GL/freeglut.h>  
#include <math.h>
#include "vector3d.h"
#include "vector4d.h"
#include "matrix4x4.h"
#include <vector>
#include "Figura.h"
class Top:public Figura
{
public:
	vector< vector<Vector3D> > figura;
	int brtacakaTop;

	Top(int n,double r1,double r2,double r3,double r4,double r5,double d1,double d2,double d3,double d4,Vector3D V,int boja);
	Top(void);
	virtual void Draw();
	virtual void Move(Vector3D v);
	virtual void DrawPicker();
	~Top(void);
};

