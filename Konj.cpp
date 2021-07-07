#include "Konj.h"
#include <GL/freeglut.h>  
#include <math.h>
#include "vector3d.h"
#include "vector4d.h"
#include "matrix4x4.h"
#include <vector>
#include <SOIL.h>
#define M_PI 3.14

Konj::Konj(int n,double R_konj,Vector3D v,int boja)
{

			glGenTextures(1,&texture);


	int width, height;
	unsigned char* image;


	glBindTexture(GL_TEXTURE_2D, texture);
	if(boja==1)image = SOIL_load_image("beli.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	else image = SOIL_load_image("crni.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
				  GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	br_tacka_konj=n;
	ind=boja;
	figura.resize(4);
	figura[0].resize(br_tacka_konj);
	figura[1].resize(br_tacka_konj);
	figura[2].resize(8);
	figura[3].resize(8);

	double ugao=2*M_PI/br_tacka_konj;

	for(int j=0;j<br_tacka_konj;j++)
	{
		figura[0][j].Set(R_konj*cos(j*ugao),0,R_konj*sin(j*ugao));
		figura[1][j].Set(R_konj*cos(j*ugao),0.3,R_konj*sin(j*ugao));
	}

	figura[2][0].Set(0.1,1,0.2);
	figura[2][1].Set(-0.3,0.3,0.2);
	figura[2][2].Set(0.3,0.3,0.2);
	figura[2][3].Set(0.7,1.1,0.2);
	figura[2][4].Set(0.5,1.5,0.2);
	figura[2][5].Set(0,1.5,0.2);
	figura[2][6].Set(-0.5,1,0.2);
	figura[2][7].Set(-0.4,0.8,0.2);
	
	


	figura[3][0].Set(0.1,1,-0.2);
	figura[3][1].Set(-0.3,0.3,-0.2);
	figura[3][2].Set(0.3,0.3,-0.2);
	figura[3][3].Set(0.7,1.1,-0.2);
	figura[3][4].Set(0.5,1.5,-0.2);
	figura[3][5].Set(0,1.5,-0.2);
	figura[3][6].Set(-0.5,1,-0.2);
	figura[3][7].Set(-0.4,0.8,-0.2);
	Matrix4x4 mr,mv,mt;
	visina=1.5;
	if(ind==1)mr.loadRotateY(-M_PI/2);
	else mr.loadRotateY(M_PI/2);
	mv.loadTranslate(v.m_x,v.m_y,v.m_z);
	mt=mv*mr;
	for(int i=0;i<figura.size();i++)
		for(int j=0;j<figura[i].size();j++)figura[i][j]=mt*figura[i][j];
}


void Konj::Draw()
{

glEnable(GL_TEXTURE_2D);
glBindTexture(GL_TEXTURE_2D, texture);


	for(int i=0;i<4;i++)
	{
		glBegin(GL_POLYGON);
		for(int j=0;j<figura[i].size();j++)
			{
				if(i<2)glTexCoord2f(figura[i][j].m_x,figura[i][j].m_z);
				else glTexCoord2f(figura[i][j].m_y, figura[i][j].m_z);
				glVertex3f(figura[i][j].m_x,figura[i][j].m_y,figura[i][j].m_z);
			}
		glEnd();
	}




	for(int i=0;i<4;i+=2)
	{
		glBegin(GL_QUAD_STRIP);
		for(int j=0;j<figura[i].size();j++)
		{
			if(j==4||j==5)glTexCoord2f(figura[i][j].m_x, figura[i][j].m_z);
			else glTexCoord2f(figura[i][j].m_x,figura[i][j].m_y);
			glVertex3f(figura[i][j].m_x,figura[i][j].m_y,figura[i][j].m_z);
			if(j==4||j==5)glTexCoord2f(figura[i + 1][j].m_x, figura[i + 1][j].m_z);
				else glTexCoord2f(figura[i+1][j].m_x,figura[i+1][j].m_y);
			glVertex3f(figura[i+1][j].m_x,figura[i+1][j].m_y,figura[i+1][j].m_z);
		}
		glTexCoord2f(figura[i][0].m_x,figura[i][0].m_y);
		glVertex3f(figura[i][0].m_x,figura[i][0].m_y,figura[i][0].m_z);
		glTexCoord2f(figura[i+1][0].m_x,figura[i+1][0].m_y);
		glVertex3f(figura[i+1][0].m_x,figura[i+1][0].m_y,figura[i+1][0].m_z);
		glEnd();
	}
	glDisable(GL_TEXTURE_2D);
	
}


void Konj::DrawPicker()
{




	for(int i=0;i<4;i++)
	{
		glBegin(GL_POLYGON);
		for(int j=0;j<figura[i].size();j++)
			{
			
				glVertex3f(figura[i][j].m_x,figura[i][j].m_y,figura[i][j].m_z);
			}
		glEnd();
	}




	for(int i=0;i<4;i+=2)
	{
		glBegin(GL_QUAD_STRIP);
		for(int j=0;j<figura[i].size();j++)
		{
			
			glVertex3f(figura[i][j].m_x,figura[i][j].m_y,figura[i][j].m_z);
			
			glVertex3f(figura[i+1][j].m_x,figura[i+1][j].m_y,figura[i+1][j].m_z);
		}
	
		glVertex3f(figura[i][0].m_x,figura[i][0].m_y,figura[i][0].m_z);

		glVertex3f(figura[i+1][0].m_x,figura[i+1][0].m_y,figura[i+1][0].m_z);
		glEnd();
	}

	
}


void Konj::Move(Vector3D v)
{
	Matrix4x4 mt;
	mt.loadTranslate(v.m_x,v.m_y,v.m_z);
	for(int i=0;i<figura.size();i++)
		for(int j=0;j<figura[i].size();j++)figura[i][j]=mt*figura[i][j];
}





