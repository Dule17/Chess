#include "Lovac.h"
#include <GL/freeglut.h>  
#include <math.h>
#include "vector3d.h"
#include "vector4d.h"
#include "matrix4x4.h"
#include <vector>
#include "Pesak.h"
#include "Top.h"
#include <SOIL.h>
#define M_PI 3.14

Lovac::Lovac(void)
{
}
Lovac::Lovac(int n,double r1,double r2,double r3,double r4,double d1,double d2,Vector3D v,int boja)
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

	



	br_tacka_lovac=n;
	figura.resize(5);
	for(int i=0;i<3;i++)figura[i].resize(2);
	for(int i=0;i<3;i++)
		for(int j=0;j<2;j++)figura[i][j].resize(br_tacka_lovac);
	ind=boja;
	for(int i=3;i<5;i++)figura[i].resize(br_tacka_lovac);
	for(int i=3;i<5;i++)
		for(int j=0;j<br_tacka_lovac;j++)figura[i][j].resize(br_tacka_lovac);


	double ugao=2*M_PI/br_tacka_lovac;
	Matrix4x4 mt;
	mt.loadRotateY(ugao);

	for(int i=0;i<5;i++)
	{
		for(int j=0;j<br_tacka_lovac;j++)
				{

					

					if(i<3)
						{
							if(i==0)
							{
								figura[i][0][j].Set(r1*cos(j*ugao),0,r1*sin(j*ugao));
								figura[i][1][j].Set(r1*cos(j*ugao),d1,r1*sin(j*ugao));
							}
							if(i==1)
							{
								figura[i][0][j].Set(r2*cos(j*ugao),d1,r2*sin(j*ugao));
								figura[i][1][j].Set(r3*cos(j*ugao),d1+d2,r3*sin(j*ugao));
							}
							if(i==2)
							{
								figura[i][0][j].Set(r2*cos(j*ugao),d1+d2,r2*sin(j*ugao));
								figura[i][1][j].Set(r2*cos(j*ugao),d1+d1+d2,r2*sin(j*ugao));

							}
					    }
					else
					{
						
						if(i==3)figura[i][0][j].Set(r1*cos(j*ugao),d1+d1+d2+r1-r1/4+r1*sin(j*ugao),0);
						else figura[i][0][j].Set(r4*cos(j*ugao),d1+d1+d2+r1+r1+r4*sin(j*ugao),0);
					
						for(int k=1;k<br_tacka_lovac;k++)figura[i][k][j]=mt*figura[i][k-1][j];

					}
			    }
	}

	visina=d1+d1+d2+r1+r1+r4;

	mt.loadScale(0.5,1,0.5);
	for(int k=0;k<br_tacka_lovac;k++)
		for(int j=0;j<br_tacka_lovac;j++)figura[3][k][j]=mt*figura[3][k][j];


	mt.loadTranslate(v.m_x,v.m_y,v.m_z);
	for(int i=0;i<5;i++)
		for(int j=0;j<figura[i].size();j++)
			for(int k=0;k<figura[i][j].size();k++) figura[i][j][k]=mt*figura[i][j][k];
}

void Lovac::Draw()
{
glEnable(GL_TEXTURE_2D);
glBindTexture(GL_TEXTURE_2D, texture);


	for(int i=0;i<3;i++)
	{
		for(int j=0;j<figura[i].size();j++)
		{
			glBegin(GL_POLYGON);
			for(int k=0;k<figura[i][j].size();k++)
				{
					glTexCoord2f(figura[i][j][k].m_x,figura[i][j][k].m_z);
					glVertex3f(figura[i][j][k].m_x,figura[i][j][k].m_y,figura[i][j][k].m_z);
				}
			glEnd();
		}
	}


	for(int i=0;i<5;i++)
	{
		for(int j=0;j<figura[i].size()-1;j++)
		{
			glBegin(GL_QUAD_STRIP);
			for(int k=0;k<figura[i][j].size();k++)
				{
					glTexCoord2f(figura[i][j+1][k].m_x,figura[i][j+1][k].m_y);
					glVertex3f(figura[i][j+1][k].m_x,figura[i][j+1][k].m_y,figura[i][j+1][k].m_z);
					glTexCoord2f(figura[i][j][k].m_x,figura[i][j][k].m_y);
					glVertex3f(figura[i][j][k].m_x,figura[i][j][k].m_y,figura[i][j][k].m_z);
				}
			glTexCoord2f(figura[i][j+1][0].m_x,figura[i][j+1][0].m_y);
			glVertex3f(figura[i][j+1][0].m_x,figura[i][j+1][0].m_y,figura[i][j+1][0].m_z);
			glTexCoord2f(figura[i][j][0].m_x,figura[i][j][0].m_y);
			glVertex3f(figura[i][j][0].m_x,figura[i][j][0].m_y,figura[i][j][0].m_z);
			glEnd();
		}
	}
	glDisable(GL_TEXTURE_2D);
}

void Lovac::DrawPicker()
{



	for(int i=0;i<3;i++)
	{
		for(int j=0;j<figura[i].size();j++)
		{
			glBegin(GL_POLYGON);
			for(int k=0;k<figura[i][j].size();k++)
				{
		
					glVertex3f(figura[i][j][k].m_x,figura[i][j][k].m_y,figura[i][j][k].m_z);
				}
			glEnd();
		}
	}


	for(int i=0;i<5;i++)
	{
		for(int j=0;j<figura[i].size()-1;j++)
		{
			glBegin(GL_QUAD_STRIP);
			for(int k=0;k<figura[i][j].size();k++)
				{
					
					glVertex3f(figura[i][j+1][k].m_x,figura[i][j+1][k].m_y,figura[i][j+1][k].m_z);
					
					glVertex3f(figura[i][j][k].m_x,figura[i][j][k].m_y,figura[i][j][k].m_z);
				}
			
			glVertex3f(figura[i][j+1][0].m_x,figura[i][j+1][0].m_y,figura[i][j+1][0].m_z);
		
			glVertex3f(figura[i][j][0].m_x,figura[i][j][0].m_y,figura[i][j][0].m_z);
			glEnd();
		}
	}

}
Lovac::~Lovac(void)
{
}
void Lovac::Move(Vector3D v)
{
	Matrix4x4 mt;
	mt.loadTranslate(v.m_x,v.m_y,v.m_z);
	for(int i=0;i<5;i++)
		for(int j=0;j<figura[i].size();j++)
			for(int k=0;k<figura[i][j].size();k++) figura[i][j][k]=mt*figura[i][j][k];
}