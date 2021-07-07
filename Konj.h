#pragma once
#include <GL/freeglut.h>  
#include <math.h>
#include "vector3d.h"
#include "vector4d.h"
#include "matrix4x4.h"
#include <vector>
#include "Figura.h"
class Konj:public Figura
{
public:
	vector< vector<Vector3D> > figura;
	int br_tacka_konj;
	Konj(int n,double R_konj,Vector3D v,int boja);
	virtual void Draw();
	virtual void Move(Vector3D v);
	virtual void DrawPicker();
};

