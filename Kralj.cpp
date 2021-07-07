#include "Kralj.h"
#include <GL/freeglut.h>  
#include <math.h>
#include "vector3d.h"
#include "vector4d.h"
#include "matrix4x4.h"
#include <vector>
#include <SOIL.h>
#define M_PI 3.14

Kralj::Kralj(void)
{
}

Kralj::Kralj(int n,double r1,double r2,double r4,double r3,double d1,double d2,Vector3D v,int boja)
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

	
	


	br_tacaka_kralj=n;
	figura.resize(5);
	for(int i=0;i<5;i++)figura[i].resize(2);
	for(int i=0;i<4;i++)
		for(int j=0;j<2;j++)figura[i][j].resize(br_tacaka_kralj);

	figura[4][0].resize(12);
	figura[4][1].resize(12);
	ind=boja;
	double ugao=2*M_PI/br_tacaka_kralj;
	
		for(int j=0;j<br_tacaka_kralj;j++)
		{
			figura[0][0][j].Set(r1*cos(j*ugao),0,r1*sin(j*ugao));
			figura[0][1][j].Set(r1*cos(j*ugao),d1,r1*sin(j*ugao));

			figura[1][0][j].Set(r2*cos(j*ugao),d1,r2*sin(j*ugao));
			figura[1][1][j].Set(r3*cos(j*ugao),d1+d2,r3*sin(j*ugao));


			figura[2][0][j].Set(r4*cos(j*ugao),d1+d2,r4*sin(j*ugao));
			figura[2][1][j].Set(r4*cos(j*ugao),d1+d2+d1,r4*sin(j*ugao));

			figura[3][0][j].Set(r3*cos(j*ugao),d1+d2+d1,r3*sin(j*ugao));
			figura[3][1][j].Set(r2*cos(j*ugao),d1+d2+d1+d1,r2*sin(j*ugao));
		}


		figura[4][0][0].Set(-r2/4,d1+d2+d1+d1+d1/2,r2/2);
		figura[4][0][1].Set(-r2/4,d1+d2+d1+d1,r2/2);
		figura[4][0][2].Set(r2/4,d1+d2+d1+d1,r2/2);
		figura[4][0][3].Set(r2/4,d1+d2+d1+d1+d1/2,r2/2);
		figura[4][0][4].Set(r2/2,d1+d2+d1+d1+d1/2,r2/2);
		figura[4][0][5].Set(r2/2,d1+d2+d1+d1+d1,r2/2);
		figura[4][0][6].Set(r2/4,d1+d2+d1+d1+d1,r2/2);
		figura[4][0][7].Set(r2/4,d1+d2+d1+d1+d1+d1/2,r2/2);
		figura[4][0][8].Set(-r2/4,d1+d2+d1+d1+d1+d1/2,r2/2);
		figura[4][0][9].Set(-r2/4,d1+d2+d1+d1+d1,r2/2);
		figura[4][0][10].Set(-r2/2,d1+d2+d1+d1+d1,r2/2);
		figura[4][0][11].Set(-r2/2,d1+d2+d1+d1+d1/2,r2/2);
		
		figura[4][1][0].Set(-r2/4,d1+d2+d1+d1+d1/2,-r2/2);
		figura[4][1][1].Set(-r2/4,d1+d2+d1+d1,-r2/2);
		figura[4][1][2].Set(r2/4,d1+d2+d1+d1,-r2/2);
		figura[4][1][3].Set(r2/4,d1+d2+d1+d1+d1/2,-r2/2);
		figura[4][1][4].Set(r2/2,d1+d2+d1+d1+d1/2,-r2/2);
		figura[4][1][5].Set(r2/2,d1+d2+d1+d1+d1,-r2/2);
		figura[4][1][6].Set(r2/4,d1+d2+d1+d1+d1,-r2/2);
		figura[4][1][7].Set(r2/4,d1+d2+d1+d1+d1+d1/2,-r2/2);
		figura[4][1][8].Set(-r2/4,d1+d2+d1+d1+d1+d1/2,-r2/2);
		figura[4][1][9].Set(-r2/4,d1+d2+d1+d1+d1,-r2/2);
		figura[4][1][10].Set(-r2/2,d1+d2+d1+d1+d1,-r2/2);
		figura[4][1][11].Set(-r2/2,d1+d2+d1+d1+d1/2,-r2/2);
		visina=d1+d2+d1+d1+d1+d1/2;
		Matrix4x4 mt;
		mt.loadTranslate(v.m_x,v.m_y,v.m_z);
		for(int i=0;i<5;i++)
			for(int k=0;k<2;k++)
				for(int j=0;j<figura[i][k].size();j++)figura[i][k][j]=mt*figura[i][k][j];

}
void Kralj::Draw()
{
glEnable(GL_TEXTURE_2D);
glBindTexture(GL_TEXTURE_2D, texture);

	for(int i=0;i<5;i++)
		for(int k=0;k<2;k++)
		{

			glBegin(GL_POLYGON);
			for(int j=0;j<figura[i][k].size();j++)
				{
					if(i==4)glTexCoord2f(figura[i][k][j].m_x, figura[i][k][j].m_y);
						else glTexCoord2f(figura[i][k][j].m_x,figura[i][k][j].m_z);
					glVertex3f(figura[i][k][j].m_x,figura[i][k][j].m_y,figura[i][k][j].m_z);
				}
			glEnd();
		}
	for(int i=0;i<5;i++)
		{

			glBegin(GL_QUAD_STRIP);
			for(int j=0;j<figura[i][0].size();j++)
			{
				if(i==4) glTexCoord2f(figura[i][0][j].m_x, figura[i][0][j].m_z);
				else glTexCoord2f(figura[i][0][j].m_x,figura[i][0][j].m_y);
					glVertex3f(figura[i][0][j].m_x,figura[i][0][j].m_y,figura[i][0][j].m_z);
				if(i==4)glTexCoord2f(figura[i][1][j].m_x, figura[i][1][j].m_z);
				else	glTexCoord2f(figura[i][1][j].m_x,figura[i][1][j].m_y);
					glVertex3f(figura[i][1][j].m_x,figura[i][1][j].m_y,figura[i][1][j].m_z);
			}
			if(i==4)glTexCoord2f(figura[i][0][0].m_x, figura[i][0][0].m_z);
			else glTexCoord2f(figura[i][0][0].m_x,figura[i][0][0].m_y);
			glVertex3f(figura[i][0][0].m_x,figura[i][0][0].m_y,figura[i][0][0].m_z);
			if(i==4)glTexCoord2f(figura[i][1][0].m_x, figura[i][1][0].m_z);
			else glTexCoord2f(figura[i][1][0].m_x,figura[i][1][0].m_y);
			glVertex3f(figura[i][1][0].m_x,figura[i][1][0].m_y,figura[i][1][0].m_z);
			glEnd();
		}
	glDisable(GL_TEXTURE_2D);
}


void Kralj::DrawPicker()
{


	for(int i=0;i<5;i++)
		for(int k=0;k<2;k++)
		{

			glBegin(GL_POLYGON);
			for(int j=0;j<figura[i][k].size();j++)
				{
					
					glVertex3f(figura[i][k][j].m_x,figura[i][k][j].m_y,figura[i][k][j].m_z);
				}
			glEnd();
		}
	for(int i=0;i<5;i++)
		{
			glBegin(GL_QUAD_STRIP);
			for(int j=0;j<figura[i][0].size();j++)
			{
					
					glVertex3f(figura[i][0][j].m_x,figura[i][0][j].m_y,figura[i][0][j].m_z);
					
					glVertex3f(figura[i][1][j].m_x,figura[i][1][j].m_y,figura[i][1][j].m_z);
			}
		
			glVertex3f(figura[i][0][0].m_x,figura[i][0][0].m_y,figura[i][0][0].m_z);

			glVertex3f(figura[i][1][0].m_x,figura[i][1][0].m_y,figura[i][1][0].m_z);
			glEnd();
		}

}

Kralj::~Kralj(void)
{
}

void Kralj::Move(Vector3D v)
{
		Matrix4x4 mt;
		mt.loadTranslate(v.m_x,v.m_y,v.m_z);
		for(int i=0;i<5;i++)
			for(int k=0;k<2;k++)
				for(int j=0;j<figura[i][k].size();j++)figura[i][k][j]=mt*figura[i][k][j];
}
