#pragma once
#include <GL/freeglut.h>  
#include <math.h>
#include "vector3d.h"
#include "vector4d.h"
#include "matrix4x4.h"
#include <vector>
#include "Figura.h"
class Kralj:public Figura
{
public:
	vector< vector< vector<Vector3D> > > figura;
	int br_tacaka_kralj;
	Kralj(int n,double r1,double r2,double r4,double r3,double d1,double d2,Vector3D v,int boja);
	virtual void Draw();
	virtual void DrawPicker();
	virtual void Move(Vector3D v);
	Kralj(void);
	~Kralj(void);
};

