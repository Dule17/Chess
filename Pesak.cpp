#include "Pesak.h"
#include <GL/freeglut.h>  
#include <math.h>
#include "vector3d.h"
#include "vector4d.h"
#include "matrix4x4.h"
#include <vector>
#include <SOIL.h>
#define M_PI 3.14
Pesak::Pesak(void)
{
}

Pesak::Pesak(int N,double r1,double r2,double r3,double r4,double d1,double d2,Vector3D V,int boja)
{

	
			glGenTextures(1,&texture);


	int width, height;
	unsigned char* image;


	glBindTexture(GL_TEXTURE_2D, texture);
	if(boja==1)image = SOIL_load_image("beli.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	else image = SOIL_load_image("crni.jpg", &width, &height, 0, SOIL_LOAD_RGB);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);





	brtacakaPesak=N;
	figura.resize(3);
	for(int i=0;i<2;i++)figura[i].resize(2);
	figura[2].resize(brtacakaPesak);
	figura[0][0].resize(brtacakaPesak);
	figura[0][1].resize(brtacakaPesak);
	figura[1][0].resize(brtacakaPesak);
	figura[1][1].resize(brtacakaPesak);
	double ugao=2*M_PI/brtacakaPesak;
	for(int i=0;i<brtacakaPesak;i++)figura[2][i].resize(brtacakaPesak);
	ind=boja;
	for(int j=0;j<brtacakaPesak;j++)
	{
		figura[0][0][j].Set(r1*cos(j*ugao),0,r1*sin(j*ugao));
		figura[0][1][j].Set(r1*cos(j*ugao),d1,r1*sin(j*ugao));

		figura[1][0][j].Set(r2*cos(j*ugao),d1,r2*sin(j*ugao));
		figura[1][1][j].Set(r3*cos(j*ugao),d1+d2,r3*sin(j*ugao));

		figura[2][0][j].Set(r4*cos(j*ugao),(d1+d2+r4/2)+r4*sin(j*ugao),0);
	}
	visina=d1+d2+r4/2+r4;
	Matrix4x4 mt;
	mt.loadRotateY(ugao);

		for(int k=1;k<brtacakaPesak;k++)
			for(int j=0;j<figura[2][k].size();j++)figura[2][k][j]=mt*figura[2][k-1][j];

	mt.loadTranslate(V.m_x,V.m_y,V.m_z);
	for(int i=0;i<3;i++)
		for(int k=0;k<figura[i].size();k++)
			for(int j=0;j<figura[i][k].size();j++)figura[i][k][j]=mt*figura[i][k][j];
}

void Pesak::Draw()
{

glEnable(GL_TEXTURE_2D);
glBindTexture(GL_TEXTURE_2D, texture);


	for(int i=0;i<2;i++)
	{
		for(int k=0;k<figura[i].size();k++)
		{
		glBegin(GL_POLYGON);
		for(int j=0;j<figura[i][k].size();j++)
			{
				glTexCoord2f(figura[i][k][j].m_x,figura[i][k][j].m_z);
				glVertex3f(figura[i][k][j].m_x,figura[i][k][j].m_y,figura[i][k][j].m_z);
			}
		glEnd();
		}
	}	
	
	
	for(int i=0;i<3;i++)
	{
		for(int k=0;k<figura[i].size()-1;k++)
		{

		glBegin(GL_QUAD_STRIP);
		for(int j=0;j<figura[i][k].size();j++)
		{			

			glTexCoord2f(figura[i][k][j].m_x,figura[i][k][j].m_y);
			glVertex3f(figura[i][k][j].m_x,figura[i][k][j].m_y,figura[i][k][j].m_z);
			glTexCoord2f(figura[i][k+1][j].m_x,figura[i][k+1][j].m_y);
			glVertex3f(figura[i][k+1][j].m_x,figura[i][k+1][j].m_y,figura[i][k+1][j].m_z);
		}
		glTexCoord2f(figura[i][k][0].m_x,figura[i][k][0].m_y);
		glVertex3f(figura[i][k][0].m_x,figura[i][k][0].m_y,figura[i][k][0].m_z);
		glTexCoord2f(figura[i][k+1][0].m_x,figura[i][k+1][0].m_y);
		glVertex3f(figura[i][k+1][0].m_x,figura[i][k+1][0].m_y,figura[i][k+1][0].m_z);
		glEnd();
		}
	}
	glDisable(GL_TEXTURE_2D);
}


void Pesak::DrawPicker()
{




	for(int i=0;i<2;i++)
	{
		for(int k=0;k<figura[i].size();k++)
		{
		glBegin(GL_POLYGON);
		for(int j=0;j<figura[i][k].size();j++)
			{
				
				glVertex3f(figura[i][k][j].m_x,figura[i][k][j].m_y,figura[i][k][j].m_z);
			}
		glEnd();
		}
	}	
	
	
	for(int i=0;i<3;i++)
	{
		for(int k=0;k<figura[i].size()-1;k++)
		{

		glBegin(GL_QUAD_STRIP);
		for(int j=0;j<figura[i][k].size();j++)
		{			

			
			glVertex3f(figura[i][k][j].m_x,figura[i][k][j].m_y,figura[i][k][j].m_z);
			
			glVertex3f(figura[i][k+1][j].m_x,figura[i][k+1][j].m_y,figura[i][k+1][j].m_z);
		}
	
		glVertex3f(figura[i][k][0].m_x,figura[i][k][0].m_y,figura[i][k][0].m_z);

		glVertex3f(figura[i][k+1][0].m_x,figura[i][k+1][0].m_y,figura[i][k+1][0].m_z);
		glEnd();
		}
	}

}

void Pesak::Move(Vector3D v)
{
	Matrix4x4 mt;
	mt.loadTranslate(v.m_x,v.m_y,v.m_z);
	for(int i=0;i<3;i++)
		for(int k=0;k<figura[i].size();k++)
			for(int j=0;j<figura[i][k].size();j++)figura[i][k][j]=mt*figura[i][k][j];
}
Pesak::~Pesak(void)
{
}
