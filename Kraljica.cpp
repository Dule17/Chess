#include "Kraljica.h"
#include <GL/freeglut.h>  
#include <math.h>
#include "vector3d.h"
#include "vector4d.h"
#include "matrix4x4.h"
#include <vector>
#include <SOIL.h>
#define M_PI 3.14
Kraljica::Kraljica(void)
{
}

Kraljica::Kraljica(int n,double r1,double r2,double r4,double r3,double d1,double d2,Vector3D v,int boja)
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

	



	br_tacaka_kraljica=n;
	figura.resize(5);
	for(int i=0;i<4;i++)figura[i].resize(2);
	figura[4].resize(br_tacaka_kraljica);
	ind=boja;
	for(int i=0;i<4;i++)
		for(int j=0;j<2;j++)figura[i][j].resize(br_tacaka_kraljica);
	for(int j=0;j<br_tacaka_kraljica;j++)
	{
		figura[4][j].resize(br_tacaka_kraljica);

	}

	double ugao=2*M_PI/br_tacaka_kraljica;
	Matrix4x4 mt;
	mt.loadRotateY(ugao);
		for(int j=0;j<br_tacaka_kraljica;j++)
		{
			figura[0][0][j].Set(r1*cos(j*ugao),0,r1*sin(j*ugao));
			figura[0][1][j].Set(r1*cos(j*ugao),d1,r1*sin(j*ugao));

			figura[1][0][j].Set(r2*cos(j*ugao),d1,r2*sin(j*ugao));
			figura[1][1][j].Set(r3*cos(j*ugao),d1+d2,r3*sin(j*ugao));


			figura[2][0][j].Set(r4*cos(j*ugao),d1+d2,r4*sin(j*ugao));
			figura[2][1][j].Set(r4*cos(j*ugao),d1+d2+d1,r4*sin(j*ugao));

			figura[3][0][j].Set(r3*cos(j*ugao),d1+d2+d1,r3*sin(j*ugao));
			if(j%2==0)figura[3][1][j].Set(r4*cos(j*ugao),d1+d2+d1+r2/2,r4*sin(j*ugao));
			else figura[3][1][j].Set((r4+r4/4)*cos(j*ugao),d1+d2+d1+r2/2+r2/3,(r4+r4/4)*sin(j*ugao));

			figura[4][0][j].Set(r3*cos(j*ugao),d1+d2+d1+r3+r2/4+r3*sin(j*ugao),0);

			for(int k=1;k<br_tacaka_kraljica;k++)figura[4][k][j]=mt*figura[4][k-1][j];
		}
		visina=d1+d2+d1+r3+r2/4+r3;
		mt.loadTranslate(v.m_x,v.m_y,v.m_z);
		for(int i=0;i<5;i++)
			for(int k=0;k<figura[i].size();k++)
				for(int j=0;j<figura[i][k].size();j++)figura[i][k][j]=mt*figura[i][k][j];
}
void Kraljica::Draw()
{
glEnable(GL_TEXTURE_2D);
glBindTexture(GL_TEXTURE_2D, texture);

	for(int i=0;i<4;i++)
		for(int k=0;k<figura[i].size();k++)
		{
		    if(i==3)break;
		    glBegin(GL_POLYGON);
			for(int j=0;j<figura[i][k].size();j++)
				{
					glTexCoord2f(figura[i][k][j].m_x,figura[i][k][j].m_z);
					glVertex3f(figura[i][k][j].m_x,figura[i][k][j].m_y,figura[i][k][j].m_z);
			}
			glEnd();
		}
	for(int i=0;i<5;i++)
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


void Kraljica::DrawPicker()
{


	for(int i=0;i<4;i++)
		for(int k=0;k<figura[i].size();k++)
		{
		    if(i==3)break;
		    glBegin(GL_POLYGON);
			for(int j=0;j<figura[i][k].size();j++)
				{
				
					glVertex3f(figura[i][k][j].m_x,figura[i][k][j].m_y,figura[i][k][j].m_z);
			}
			glEnd();
		}
	for(int i=0;i<5;i++)
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

Kraljica::~Kraljica(void)
{
}
void Kraljica::Move(Vector3D v)
{
		Matrix4x4 mt;
		mt.loadTranslate(v.m_x,v.m_y,v.m_z);
		for(int i=0;i<5;i++)
			for(int k=0;k<figura[i].size();k++)
				for(int j=0;j<figura[i][k].size();j++)figura[i][k][j]=mt*figura[i][k][j];
}