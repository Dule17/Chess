#include "Top.h"
#include <GL/freeglut.h>  
#include <math.h>
#include "vector3d.h"
#include "vector4d.h"
#include "matrix4x4.h"
#include <vector>
#include <SOIL.h>
#define M_PI 3.14
Top::Top(void)
{
}

 Top::Top(int n,double r1,double r2,double r3,double r4,double r5,double d1,double d2,double d3,double d4,Vector3D V,int boja)
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




	figura.resize(9);
	brtacakaTop=n;

	for(int i=0;i<9;i++)figura[i].resize(brtacakaTop);
	ind=boja;
	double ugao=2*M_PI/brtacakaTop;

	for(int j=0;j<brtacakaTop;j++)
	{
		figura[0][j].Set(r1*cos(j*ugao),0,r1*sin(j*ugao));
		figura[1][j].Set(r1*cos(j*ugao),d1,r1*sin(j*ugao));

		figura[2][j].Set(r2*cos(j*ugao),d1,r2*sin(j*ugao));
		figura[3][j].Set(r3*cos(j*ugao),d1+d2,r3*sin(j*ugao));

		figura[4][j].Set(r4*cos(j*ugao),d1+d2,r4*sin(j*ugao));

		figura[5][j].Set(r4*cos(j*ugao),d1+d2+d3,r4*sin(j*ugao));
		figura[6][j].Set(r4*cos(j*ugao),d1+d2+d3+d4,r4*sin(j*ugao));

		figura[7][j].Set(r5*cos(j*ugao),d1+d2+d3,r5*sin(j*ugao));
		figura[8][j].Set(r5*cos(j*ugao),d1+d2+d3+d4,r5*sin(j*ugao));
	}
	visina=d1+d2+d3+d4;
	Matrix4x4 mt;
	mt.loadTranslate(V.m_x,V.m_y,V.m_z);
	for(int i=0;i<9;i++)
	{

		for(int j=0;j<figura[i].size();j++)figura[i][j]=mt*figura[i][j];
	}
}

void Top::Draw(void)
{
glEnable(GL_TEXTURE_2D);
glBindTexture(GL_TEXTURE_2D, texture);


	for(int i=0;i<6;i++)
	{

		glBegin(GL_POLYGON);
		for(int j=0;j<figura[i].size();j++)
			{
				glTexCoord2f(figura[i][j].m_x,figura[i][j].m_z);
				glVertex3f(figura[i][j].m_x,figura[i][j].m_y,figura[i][j].m_z);
			}
		glEnd();

	}	
	

	

   for(int i=0;i<8;i++)
	{
		if(i>=5)
		{
		int br=0;
		
		for(int j=0;j<figura[i].size();j++)
			{
				if(br==0)glBegin(GL_QUAD_STRIP);
				glTexCoord2f(figura[i][j].m_x,figura[i][j].m_y);
				glVertex3f(figura[i][j].m_x,figura[i][j].m_y,figura[i][j].m_z);
				glTexCoord2f(figura[i+1][j].m_x,figura[i+1][j].m_y);
				glVertex3f(figura[i+1][j].m_x,figura[i+1][j].m_y,figura[i+1][j].m_z);
				br++;
				if(br==5)	
					{
						glEnd();
						br=0;
					}
			}

	
		}
		else
		{
		glBegin(GL_QUAD_STRIP);
		for(int j=0;j<figura[i].size();j++)
			{
				glTexCoord2f(figura[i][j].m_x,figura[i][j].m_y);
				glVertex3f(figura[i][j].m_x,figura[i][j].m_y,figura[i][j].m_z);
				glTexCoord2f(figura[i+1][j].m_x,figura[i+1][j].m_y);
				glVertex3f(figura[i+1][j].m_x,figura[i+1][j].m_y,figura[i+1][j].m_z);
			}
		glTexCoord2f(figura[i][0].m_x,figura[i][0].m_y);
		glVertex3f(figura[i][0].m_x,figura[i][0].m_y,figura[i][0].m_z);
		glTexCoord2f(figura[i+1][0].m_x,figura[i+1][0].m_y);
		glVertex3f(figura[i+1][0].m_x,figura[i+1][0].m_y,figura[i+1][0].m_z);
		glEnd();
		}
	}


   	glBegin(GL_QUAD_STRIP);
	int br=0;
		for(int j=0;j<figura[7].size();j++)
			{
				if(br==0)
					{
						glBegin(GL_POLYGON);
						glTexCoord2f(figura[5][j].m_x,figura[5][j].m_y);
						glVertex3f(figura[5][j].m_x,figura[5][j].m_y,figura[5][j].m_z);
						glTexCoord2f(figura[7][j].m_x,figura[7][j].m_y);
						glVertex3f(figura[7][j].m_x,figura[7][j].m_y,figura[7][j].m_z);
						glTexCoord2f(figura[8][j].m_x,figura[8][j].m_y);
						glVertex3f(figura[8][j].m_x,figura[8][j].m_y,figura[8][j].m_z);
						glTexCoord2f(figura[6][j].m_x,figura[6][j].m_y);
						glVertex3f(figura[6][j].m_x,figura[6][j].m_y,figura[6][j].m_z);
						glEnd();

						glBegin(GL_QUAD_STRIP);
					}
				glTexCoord2f(figura[6][j].m_x,figura[6][j].m_z);
				glVertex3f(figura[6][j].m_x,figura[6][j].m_y,figura[6][j].m_z);
				glTexCoord2f(figura[8][j].m_x,figura[8][j].m_z);
				glVertex3f(figura[8][j].m_x,figura[8][j].m_y,figura[8][j].m_z);
				br++;
				if(br==5)	
					{
						glEnd();
						br=0;

						glBegin(GL_POLYGON);
						glTexCoord2f(figura[5][j].m_x,figura[5][j].m_y);
						glVertex3f(figura[5][j].m_x,figura[5][j].m_y,figura[5][j].m_z);
						glTexCoord2f(figura[7][j].m_x,figura[7][j].m_y);
						glVertex3f(figura[7][j].m_x,figura[7][j].m_y,figura[7][j].m_z);
						glTexCoord2f(figura[8][j].m_x,figura[8][j].m_y);
						glVertex3f(figura[8][j].m_x,figura[8][j].m_y,figura[8][j].m_z);
						glTexCoord2f(figura[6][j].m_x,figura[6][j].m_y);
						glVertex3f(figura[6][j].m_x,figura[6][j].m_y,figura[6][j].m_z);
						glEnd();

					}
			}
		glDisable(GL_TEXTURE_2D);

}

void Top::DrawPicker(void)
{



	for(int i=0;i<6;i++)
	{

		glBegin(GL_POLYGON);
		for(int j=0;j<figura[i].size();j++)
			{
				
				glVertex3f(figura[i][j].m_x,figura[i][j].m_y,figura[i][j].m_z);
			}
		glEnd();

	}	
	

	

   for(int i=0;i<8;i++)
	{
		if(i>=5)
		{
		int br=0;
		
		for(int j=0;j<figura[i].size();j++)
			{
				if(br==0)glBegin(GL_QUAD_STRIP);
				
				glVertex3f(figura[i][j].m_x,figura[i][j].m_y,figura[i][j].m_z);
			
				glVertex3f(figura[i+1][j].m_x,figura[i+1][j].m_y,figura[i+1][j].m_z);
				br++;
				if(br==5)	
					{
						glEnd();
						br=0;
					}
			}

	
		}
		else
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


   	glBegin(GL_QUAD_STRIP);
	int br=0;
		for(int j=0;j<figura[7].size();j++)
			{
				if(br==0)glBegin(GL_QUAD_STRIP);
		
				glVertex3f(figura[6][j].m_x,figura[6][j].m_y,figura[6][j].m_z);
			
				glVertex3f(figura[8][j].m_x,figura[8][j].m_y,figura[8][j].m_z);
				br++;
				if(br==5)	
					{
						glEnd();
						br=0;
					}
			}
		

}


Top::~Top(void)
{
}

void Top::Move(Vector3D V)
{
	Matrix4x4 mt;
	mt.loadTranslate(V.m_x,V.m_y,V.m_z);
	for(int i=0;i<9;i++)
	{

		for(int j=0;j<figura[i].size();j++)figura[i][j]=mt*figura[i][j];
	}
}