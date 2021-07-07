#pragma once
#include <GL/freeglut.h>  
#include <math.h>
#include "vector3d.h"
#include "vector4d.h"
#include "matrix4x4.h"
#include <vector>
#include <SOIL.h>

class Pozadina
{
public:
	GLuint texture;
	vector<vector<Vector3D>> world;
	Pozadina(void);
	void Draw();
	void createWorld();

	~Pozadina(void);
};

