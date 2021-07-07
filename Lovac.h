#pragma once
#include <GL/freeglut.h>  
#include <math.h>
#include "vector3d.h"
#include "vector4d.h"
#include "matrix4x4.h"
#include <vector>
#include "Pesak.h"
#include "Top.h"
#include "Figura.h"

class Lovac:public Figura
{
public:
	vector< vector< vector<Vector3D>> > figura;
	int br_tacka_lovac;
	Lovac(int n,double r1,double r2,double r3,double r4,double d1,double d2,Vector3D v,int boja);
	Lovac(void);
	virtual void Move(Vector3D v);
	virtual void DrawPicker();
	virtual void Draw();
	~Lovac(void);
};

