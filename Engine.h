#pragma once
#include <GL/freeglut.h>  
#include <math.h>
#include <sstream>
#include "vector3d.h"
#include "vector4d.h"
#include "matrix4x4.h"
#include <vector>
#include "Pesak.h"
#include "Top.h"
#include "Lovac.h"
#include "Kraljica.h"
#include "Kralj.h"
#include "Figura.h"
#include "Polje.h"
#include "Konj.h"
#include "Pozadina.h"
#define M_PI 3.14

class Engine
{
public:
	Engine();
	~Engine(void);
	void createFrameBufferObject(int WindowWidth,int WindowHeight);
	int igra_u_toku;
	int pocetna_pozicija_i,pocetna_pozicija_j;
	double tacka_x,tacka_z;
	int igrac_igra;
	int beli_sah;
	int crni_sah;
	int beli_broj_poteza;
	int crni_broj_poteza;
	int partija_broj;
	GLuint tablaTexture;
	Pozadina pozadina;
		 GLuint texture;
	 GLuint fbo;
	 GLuint depthTexture;
	vector< vector<Polje> > tabla;
	int tabla_n;
	void createTabla();
	void postaviFigure();
	void drawTabla(int ind);
	void drawFigure(int ind);
	void drawMogucniPokreti();
	void drawIspis();
	void proveriSah();
	void PomeriPesak(int i,int j,int jedi_j,int k);
	void PomeriTop(int i,int j,int k_i,int k_j);
	void PomeriLovac(int i,int j,int k_i,int k_j);
	void PomeriKralja_i_Kraljicu(int i,int j,int k_i,int k_j);
	void PomeriKonja(int i,int j,int k_i,int k_j);
	void Zamena(int i,int j,int K_i,int k_j);
	void ispisPobednika();
	void Proveri(int x,int y,int mode);


};

