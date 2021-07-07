#include<GL\glew.h>
#include "Engine.h"


Engine::Engine()
{
	 tabla_n=8;
	 igra_u_toku=0;
	 pocetna_pozicija_i=-1,pocetna_pozicija_j=-1;
	 tacka_x,tacka_z;
	 igrac_igra=1;
	 beli_sah=0;
	 crni_sah=0;
	 beli_broj_poteza=0;
	 crni_broj_poteza=0;
	 partija_broj=1;

}
void Engine::createFrameBufferObject(int WindowWidth,int WindowHeight )
{
		   glGenFramebuffers(1, &fbo); 
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	
    // Create the texture object for the primitive information buffer
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, WindowWidth, WindowHeight,
                0, GL_RGB, GL_FLOAT, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 
                texture, 0); 

    // Create the texture object for the depth buffer
	glGenTextures(1, &depthTexture);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, WindowWidth, WindowHeight, 
                0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 
                depthTexture, 0); 

    // Disable reading to avoid problems with older GPUs
    glReadBuffer(GL_NONE);

    glDrawBuffer(GL_COLOR_ATTACHMENT0);

   
    // Restore the default framebuffer
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}



Engine::~Engine(void)
{
}

void Engine::postaviFigure()
{




		for(int i=0;i<tabla_n;i++)
			for(int j=0;j<tabla_n;j++)
				{
					tabla[i][j].figura=NULL;
					Vector3D d1((tabla[i][j].t1.m_x+tabla[i][j].t2.m_x)/2,0,(tabla[i][j].t1.m_z+tabla[i][j].t4.m_z)/2);
					
					if(i==1)tabla[i][j].figura=new Pesak(20,0.5,0.3,0.1,0.2,0.3,0.6,d1,1);
					if(i==6)tabla[i][j].figura=new Pesak(20,0.5,0.3,0.1,0.2,0.3,0.6,d1,0);
					if((i==0&&j==0)||(i==0&&j==7))tabla[i][j].figura=new Top(20,0.5,0.3,0.2,0.4,0.3,0.3,0.6,0.1,0.2,d1,1);
					if((i==7&&j==0)||(i==7&&j==7))tabla[i][j].figura=new Top(20,0.5,0.3,0.2,0.4,0.3,0.3,0.6,0.1,0.2,d1,0);
					if((i==0&&j==2)||(i==0&&j==5))tabla[i][j].figura=new Lovac(20,0.5,0.3,0.2,0.15,0.3,0.6,d1,1);
					if((i==7&&j==2)||(i==7&&j==5))tabla[i][j].figura=new Lovac(20,0.5,0.3,0.2,0.15,0.3,0.6,d1,0);
					if(i==0&&j==3)tabla[i][j].figura=new Kraljica(20,0.5,0.4,0.3,0.2,0.3,0.6,d1,1);
					if(i==7&&j==3)tabla[i][j].figura=new Kraljica(20,0.5,0.4,0.3,0.2,0.3,0.6,d1,0);
					if(i==0&&j==4)tabla[i][j].figura=new Kralj(20,0.5,0.4,0.3,0.2,0.3,0.6,d1,1);
					if(i==7&&j==4)tabla[i][j].figura=new Kralj(20,0.5,0.4,0.3,0.2,0.3,0.6,d1,0);

					if((i==0&&j==1)||(i==0&&j==6))tabla[i][j].figura=new Konj(20,0.5,d1,1);
					if((i==7&&j==1)||(i==7&&j==6))tabla[i][j].figura=new Konj(20,0.5,d1,0);

				}
}

void Engine::createTabla()
{


		
	glGenTextures(1,&tablaTexture);


	int width, height;
	unsigned char* image;


	glBindTexture(GL_TEXTURE_2D, tablaTexture);
    image = SOIL_load_image("tabla.jpg", &width, &height, 0, SOIL_LOAD_RGB);	


	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);





	tabla.resize(tabla_n);
	for(int i=0;i<tabla_n;i++)tabla[i].resize(tabla_n);

	int br;
	for(int i=0;i<tabla_n;i++)
	{
		for(int j=0;j<tabla_n;j++)
		{
			tabla[i][j].t1.Set(j,0,-i);
			tabla[i][j].t2.Set(j+1,0,-i);
			tabla[i][j].t3.Set(j+1,0,-i-1);
			tabla[i][j].t4.Set(j,0,-i-1);
			br=10+i*10+j;
			tabla[i][j].r=(br & 0x000000FF) >>  0;
			tabla[i][j].g = (br & 0x0000FF00) >>  8;
			tabla[i][j].b = (br & 0x00FF0000) >> 16;
		}
	}

	Matrix4x4 mt;
	mt.loadTranslate(-tabla_n/2,0,tabla_n/2);
	for(int i=0;i<tabla_n;i++)
		for(int j=0;j<tabla_n;j++)
				{
					tabla[i][j].t1=mt*tabla[i][j].t1;
					tabla[i][j].t2=mt*tabla[i][j].t2;
					tabla[i][j].t3=mt*tabla[i][j].t3;
					tabla[i][j].t4=mt*tabla[i][j].t4;
					


				}

		postaviFigure();
}

void Engine::drawTabla(int ind)
{
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

if(ind==1)
{
	
	for(int i=0;i<tabla_n;i++)
		for(int j=0;j<tabla_n;j++)
		{
			glColor3f(tabla[i][j].r/255.0f,tabla[i][j].g/255.0f, tabla[i][j].b/255.0f);
			

			glBegin(GL_POLYGON);
			glVertex3f(tabla[i][j].t1.m_x,tabla[i][j].t1.m_y,tabla[i][j].t1.m_z);
			glVertex3f(tabla[i][j].t2.m_x,tabla[i][j].t2.m_y,tabla[i][j].t2.m_z);
			glVertex3f(tabla[i][j].t3.m_x,tabla[i][j].t3.m_y,tabla[i][j].t3.m_z);
			glVertex3f(tabla[i][j].t4.m_x,tabla[i][j].t4.m_y,tabla[i][j].t4.m_z);
			glEnd();

			
		}
}
else

{
	glColor3f(1,1,1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tablaTexture);
	float x=5.0;
	float y=4.97;
	glBegin(GL_QUADS);

		glTexCoord2f(0,1);
		glVertex3f(-x,-0.01,-y);


		glTexCoord2f(1,1);
		glVertex3f(-x,-0.01,y);

		glTexCoord2f(1,0);
		glVertex3f(x,-0.01,y);

		glTexCoord2f(0,0);
		glVertex3f(x,-0.01,-y);





	glEnd();
	glDisable(GL_TEXTURE_2D);
}

}

void Engine::drawFigure(int ind)
{
	glColor3f(1,1,1);
		for(int i=0;i<tabla_n;i++)
			for(int j=0;j<tabla_n;j++)if(tabla[i][j].figura!=NULL)
				{
					if(ind==1)
						{
							
							glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
							glColor3f(tabla[i][j].r/255.0f,tabla[i][j].g/255.0f, tabla[i][j].b/255.0f);
							tabla[i][j].figura->DrawPicker();
					}
					else tabla[i][j].figura->Draw();
				}
}
void Engine::drawMogucniPokreti()
{




		if(strcmp(typeid(*tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura).name(),"class Pesak")==0)
			{
				if(tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura->ind==0)
					{
						if(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j].figura==NULL)
						{
							glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
							glColor3f(0,0,0);
							glBegin(GL_POLYGON);
								glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j].t1.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j].t1.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j].t1.m_z);
								glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j].t2.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j].t2.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j].t2.m_z);
								glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j].t3.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j].t3.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j].t3.m_z);
								glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j].t4.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j].t4.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j].t4.m_z);
							glEnd();
							glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
							glColor3f(0.6,0.9,0.6);
							glBegin(GL_POLYGON);
								glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j].t1.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j].t1.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j].t1.m_z);
								glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j].t2.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j].t2.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j].t2.m_z);
								glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j].t3.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j].t3.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j].t3.m_z);
								glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j].t4.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j].t4.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j].t4.m_z);
							glEnd();
							if(pocetna_pozicija_i == 6&&tabla[pocetna_pozicija_i-2][pocetna_pozicija_j].figura==NULL)
							{
															glColor3f(0.6,0.9,0.6);
							glBegin(GL_POLYGON);
								glVertex3f(tabla[pocetna_pozicija_i-2][pocetna_pozicija_j].t1.m_x,tabla[pocetna_pozicija_i-2][pocetna_pozicija_j].t1.m_y,tabla[pocetna_pozicija_i-2][pocetna_pozicija_j].t1.m_z);
								glVertex3f(tabla[pocetna_pozicija_i-2][pocetna_pozicija_j].t2.m_x,tabla[pocetna_pozicija_i-2][pocetna_pozicija_j].t2.m_y,tabla[pocetna_pozicija_i-2][pocetna_pozicija_j].t2.m_z);
								glVertex3f(tabla[pocetna_pozicija_i-2][pocetna_pozicija_j].t3.m_x,tabla[pocetna_pozicija_i-2][pocetna_pozicija_j].t3.m_y,tabla[pocetna_pozicija_i-2][pocetna_pozicija_j].t3.m_z);
								glVertex3f(tabla[pocetna_pozicija_i-2][pocetna_pozicija_j].t4.m_x,tabla[pocetna_pozicija_i-2][pocetna_pozicija_j].t4.m_y,tabla[pocetna_pozicija_i-2][pocetna_pozicija_j].t4.m_z);
							glEnd();
							}
						}
						
						if(pocetna_pozicija_j>0&&tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-1].figura!=NULL&&tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-1].figura->ind!=tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura->ind)
						{
							glColor3f(0.9,0.5,0.5);
							glBegin(GL_POLYGON);
								glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-1].t1.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-1].t1.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-1].t1.m_z);
								glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-1].t2.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-1].t2.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-1].t2.m_z);
								glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-1].t3.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-1].t3.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-1].t3.m_z);
								glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-1].t4.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-1].t4.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-1].t4.m_z);
							glEnd();
						}
						if(pocetna_pozicija_j<tabla_n-1&&tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+1].figura!=NULL&&tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+1].figura->ind!=tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura->ind)
						{
							glColor3f(0.9,0.5,0.5);
							glBegin(GL_POLYGON);
								glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+1].t1.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+1].t1.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+1].t1.m_z);
								glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+1].t2.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+1].t2.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+1].t2.m_z);
								glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+1].t3.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+1].t3.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+1].t3.m_z);
								glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+1].t4.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+1].t4.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+1].t4.m_z);
							glEnd();
						}
				}
				else
						{
							if(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j].figura==NULL)
							{
								glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
								glColor3f(0,0,0);
								glBegin(GL_POLYGON);
									glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j].t1.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j].t1.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j].t1.m_z);
									glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j].t2.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j].t2.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j].t2.m_z);
									glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j].t3.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j].t3.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j].t3.m_z);
									glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j].t4.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j].t4.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j].t4.m_z);
								glEnd();
								glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
								glColor3f(0.6,0.9,0.6);
								glBegin(GL_POLYGON);
									glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j].t1.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j].t1.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j].t1.m_z);
									glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j].t2.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j].t2.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j].t2.m_z);
									glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j].t3.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j].t3.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j].t3.m_z);
									glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j].t4.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j].t4.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j].t4.m_z);
								glEnd();
							if(pocetna_pozicija_i == 1&&tabla[pocetna_pozicija_i+2][pocetna_pozicija_j].figura==NULL)
							{
															glColor3f(0.6,0.9,0.6);
							glBegin(GL_POLYGON);
								glVertex3f(tabla[pocetna_pozicija_i+2][pocetna_pozicija_j].t1.m_x,tabla[pocetna_pozicija_i+2][pocetna_pozicija_j].t1.m_y,tabla[pocetna_pozicija_i+2][pocetna_pozicija_j].t1.m_z);
								glVertex3f(tabla[pocetna_pozicija_i+2][pocetna_pozicija_j].t2.m_x,tabla[pocetna_pozicija_i+2][pocetna_pozicija_j].t2.m_y,tabla[pocetna_pozicija_i+2][pocetna_pozicija_j].t2.m_z);
								glVertex3f(tabla[pocetna_pozicija_i+2][pocetna_pozicija_j].t3.m_x,tabla[pocetna_pozicija_i+2][pocetna_pozicija_j].t3.m_y,tabla[pocetna_pozicija_i+2][pocetna_pozicija_j].t3.m_z);
								glVertex3f(tabla[pocetna_pozicija_i+2][pocetna_pozicija_j].t4.m_x,tabla[pocetna_pozicija_i+2][pocetna_pozicija_j].t4.m_y,tabla[pocetna_pozicija_i+2][pocetna_pozicija_j].t4.m_z);
							glEnd();
							}
							}
							if(pocetna_pozicija_j>0&&tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-1].figura!=NULL&&tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-1].figura->ind!=tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura->ind)
							{
								glColor3f(0.9,0.5,0.5);
								glBegin(GL_POLYGON);
									glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-1].t1.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-1].t1.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-1].t1.m_z);
									glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-1].t2.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-1].t2.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-1].t2.m_z);
									glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-1].t3.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-1].t3.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-1].t3.m_z);
									glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-1].t4.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-1].t4.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-1].t4.m_z);
								glEnd();
							}
							if(pocetna_pozicija_j<tabla_n-1&&tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+1].figura!=NULL&&tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+1].figura->ind!=tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura->ind)
							{
								glColor3f(0.9,0.5,0.5);
								glBegin(GL_POLYGON);
									glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+1].t1.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+1].t1.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+1].t1.m_z);
									glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+1].t2.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+1].t2.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+1].t2.m_z);
									glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+1].t3.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+1].t3.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+1].t3.m_z);
									glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+1].t4.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+1].t4.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+1].t4.m_z);
								glEnd();
							}

						
					}


			}
			 if(strcmp(typeid(*tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura).name(),"class Top")==0)
			{
				for(int i=pocetna_pozicija_i+1;i<tabla_n;i++)
				{
					if(tabla[i][pocetna_pozicija_j].figura==NULL)
					{
						glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
						glColor3f(0,0,0);
						glBegin(GL_POLYGON);
								glVertex3f(tabla[i][pocetna_pozicija_j].t1.m_x,tabla[i][pocetna_pozicija_j].t1.m_y,tabla[i][pocetna_pozicija_j].t1.m_z);
								glVertex3f(tabla[i][pocetna_pozicija_j].t2.m_x,tabla[i][pocetna_pozicija_j].t2.m_y,tabla[i][pocetna_pozicija_j].t2.m_z);
								glVertex3f(tabla[i][pocetna_pozicija_j].t3.m_x,tabla[i][pocetna_pozicija_j].t3.m_y,tabla[i][pocetna_pozicija_j].t3.m_z);
								glVertex3f(tabla[i][pocetna_pozicija_j].t4.m_x,tabla[i][pocetna_pozicija_j].t4.m_y,tabla[i][pocetna_pozicija_j].t4.m_z);
						glEnd();
						glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
							glColor3f(0.6,0.9,0.6);
							glBegin(GL_POLYGON);
								glVertex3f(tabla[i][pocetna_pozicija_j].t1.m_x,tabla[i][pocetna_pozicija_j].t1.m_y,tabla[i][pocetna_pozicija_j].t1.m_z);
								glVertex3f(tabla[i][pocetna_pozicija_j].t2.m_x,tabla[i][pocetna_pozicija_j].t2.m_y,tabla[i][pocetna_pozicija_j].t2.m_z);
								glVertex3f(tabla[i][pocetna_pozicija_j].t3.m_x,tabla[i][pocetna_pozicija_j].t3.m_y,tabla[i][pocetna_pozicija_j].t3.m_z);
								glVertex3f(tabla[i][pocetna_pozicija_j].t4.m_x,tabla[i][pocetna_pozicija_j].t4.m_y,tabla[i][pocetna_pozicija_j].t4.m_z);
							glEnd();
					}
					if(tabla[i][pocetna_pozicija_j].figura!=NULL)
					{
						if(tabla[i][pocetna_pozicija_j].figura->ind==tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura->ind)break;
						glColor3f(0.9,0.5,0.5);
						glBegin(GL_POLYGON);
								glVertex3f(tabla[i][pocetna_pozicija_j].t1.m_x,tabla[i][pocetna_pozicija_j].t1.m_y,tabla[i][pocetna_pozicija_j].t1.m_z);
								glVertex3f(tabla[i][pocetna_pozicija_j].t2.m_x,tabla[i][pocetna_pozicija_j].t2.m_y,tabla[i][pocetna_pozicija_j].t2.m_z);
								glVertex3f(tabla[i][pocetna_pozicija_j].t3.m_x,tabla[i][pocetna_pozicija_j].t3.m_y,tabla[i][pocetna_pozicija_j].t3.m_z);
								glVertex3f(tabla[i][pocetna_pozicija_j].t4.m_x,tabla[i][pocetna_pozicija_j].t4.m_y,tabla[i][pocetna_pozicija_j].t4.m_z);
						glEnd();
						break;
					}
				}

				for(int i=pocetna_pozicija_i-1;i>-1;i--)
				{
					if(tabla[i][pocetna_pozicija_j].figura==NULL)
					{
						glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
						glColor3f(0,0,0);
						glBegin(GL_POLYGON);
								glVertex3f(tabla[i][pocetna_pozicija_j].t1.m_x,tabla[i][pocetna_pozicija_j].t1.m_y,tabla[i][pocetna_pozicija_j].t1.m_z);
								glVertex3f(tabla[i][pocetna_pozicija_j].t2.m_x,tabla[i][pocetna_pozicija_j].t2.m_y,tabla[i][pocetna_pozicija_j].t2.m_z);
								glVertex3f(tabla[i][pocetna_pozicija_j].t3.m_x,tabla[i][pocetna_pozicija_j].t3.m_y,tabla[i][pocetna_pozicija_j].t3.m_z);
								glVertex3f(tabla[i][pocetna_pozicija_j].t4.m_x,tabla[i][pocetna_pozicija_j].t4.m_y,tabla[i][pocetna_pozicija_j].t4.m_z);
						glEnd();
						glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
							glColor3f(0.6,0.9,0.6);
							glBegin(GL_POLYGON);
								glVertex3f(tabla[i][pocetna_pozicija_j].t1.m_x,tabla[i][pocetna_pozicija_j].t1.m_y,tabla[i][pocetna_pozicija_j].t1.m_z);
								glVertex3f(tabla[i][pocetna_pozicija_j].t2.m_x,tabla[i][pocetna_pozicija_j].t2.m_y,tabla[i][pocetna_pozicija_j].t2.m_z);
								glVertex3f(tabla[i][pocetna_pozicija_j].t3.m_x,tabla[i][pocetna_pozicija_j].t3.m_y,tabla[i][pocetna_pozicija_j].t3.m_z);
								glVertex3f(tabla[i][pocetna_pozicija_j].t4.m_x,tabla[i][pocetna_pozicija_j].t4.m_y,tabla[i][pocetna_pozicija_j].t4.m_z);
							glEnd();
					}
					if(tabla[i][pocetna_pozicija_j].figura!=NULL)
					{

						if(tabla[i][pocetna_pozicija_j].figura->ind==tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura->ind)break;
						
						glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
						glColor3f(0,0,0);
						glBegin(GL_POLYGON);
								glVertex3f(tabla[i][pocetna_pozicija_j].t1.m_x,tabla[i][pocetna_pozicija_j].t1.m_y,tabla[i][pocetna_pozicija_j].t1.m_z);
								glVertex3f(tabla[i][pocetna_pozicija_j].t2.m_x,tabla[i][pocetna_pozicija_j].t2.m_y,tabla[i][pocetna_pozicija_j].t2.m_z);
								glVertex3f(tabla[i][pocetna_pozicija_j].t3.m_x,tabla[i][pocetna_pozicija_j].t3.m_y,tabla[i][pocetna_pozicija_j].t3.m_z);
								glVertex3f(tabla[i][pocetna_pozicija_j].t4.m_x,tabla[i][pocetna_pozicija_j].t4.m_y,tabla[i][pocetna_pozicija_j].t4.m_z);
						glEnd();
						glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
						glColor3f(0.9,0.5,0.5);
						glBegin(GL_POLYGON);
								glVertex3f(tabla[i][pocetna_pozicija_j].t1.m_x,tabla[i][pocetna_pozicija_j].t1.m_y,tabla[i][pocetna_pozicija_j].t1.m_z);
								glVertex3f(tabla[i][pocetna_pozicija_j].t2.m_x,tabla[i][pocetna_pozicija_j].t2.m_y,tabla[i][pocetna_pozicija_j].t2.m_z);
								glVertex3f(tabla[i][pocetna_pozicija_j].t3.m_x,tabla[i][pocetna_pozicija_j].t3.m_y,tabla[i][pocetna_pozicija_j].t3.m_z);
								glVertex3f(tabla[i][pocetna_pozicija_j].t4.m_x,tabla[i][pocetna_pozicija_j].t4.m_y,tabla[i][pocetna_pozicija_j].t4.m_z);
						glEnd();
						break;
					}

				}


				for(int j=pocetna_pozicija_j+1;j<tabla_n;j++)
				{
					if(tabla[pocetna_pozicija_i][j].figura==NULL)
					{
						glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
						glColor3f(0,0,0);
						glBegin(GL_POLYGON);
								glVertex3f(tabla[pocetna_pozicija_i][j].t1.m_x,tabla[pocetna_pozicija_i][j].t1.m_y,tabla[pocetna_pozicija_i][j].t1.m_z);
								glVertex3f(tabla[pocetna_pozicija_i][j].t2.m_x,tabla[pocetna_pozicija_i][j].t2.m_y,tabla[pocetna_pozicija_i][j].t2.m_z);
								glVertex3f(tabla[pocetna_pozicija_i][j].t3.m_x,tabla[pocetna_pozicija_i][j].t3.m_y,tabla[pocetna_pozicija_i][j].t3.m_z);
								glVertex3f(tabla[pocetna_pozicija_i][j].t4.m_x,tabla[pocetna_pozicija_i][j].t4.m_y,tabla[pocetna_pozicija_i][j].t4.m_z);
						glEnd();
						glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
							glColor3f(0.6,0.9,0.6);
							glBegin(GL_POLYGON);
								glVertex3f(tabla[pocetna_pozicija_i][j].t1.m_x,tabla[pocetna_pozicija_i][j].t1.m_y,tabla[pocetna_pozicija_i][j].t1.m_z);
								glVertex3f(tabla[pocetna_pozicija_i][j].t2.m_x,tabla[pocetna_pozicija_i][j].t2.m_y,tabla[pocetna_pozicija_i][j].t2.m_z);
								glVertex3f(tabla[pocetna_pozicija_i][j].t3.m_x,tabla[pocetna_pozicija_i][j].t3.m_y,tabla[pocetna_pozicija_i][j].t3.m_z);
								glVertex3f(tabla[pocetna_pozicija_i][j].t4.m_x,tabla[pocetna_pozicija_i][j].t4.m_y,tabla[pocetna_pozicija_i][j].t4.m_z);
							glEnd();
					}
					if(tabla[pocetna_pozicija_i][j].figura!=NULL)
					{
						if(tabla[pocetna_pozicija_i][j].figura->ind==tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura->ind)break;
						glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
						glColor3f(0,0,0);
						glBegin(GL_POLYGON);
								glVertex3f(tabla[pocetna_pozicija_i][j].t1.m_x,tabla[pocetna_pozicija_i][j].t1.m_y,tabla[pocetna_pozicija_i][j].t1.m_z);
								glVertex3f(tabla[pocetna_pozicija_i][j].t2.m_x,tabla[pocetna_pozicija_i][j].t2.m_y,tabla[pocetna_pozicija_i][j].t2.m_z);
								glVertex3f(tabla[pocetna_pozicija_i][j].t3.m_x,tabla[pocetna_pozicija_i][j].t3.m_y,tabla[pocetna_pozicija_i][j].t3.m_z);
								glVertex3f(tabla[pocetna_pozicija_i][j].t4.m_x,tabla[pocetna_pozicija_i][j].t4.m_y,tabla[pocetna_pozicija_i][j].t4.m_z);
						glEnd();
						glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
						glColor3f(0.9,0.5,0.5);
						glBegin(GL_POLYGON);
								glVertex3f(tabla[pocetna_pozicija_i][j].t1.m_x,tabla[pocetna_pozicija_i][j].t1.m_y,tabla[pocetna_pozicija_i][j].t1.m_z);
								glVertex3f(tabla[pocetna_pozicija_i][j].t2.m_x,tabla[pocetna_pozicija_i][j].t2.m_y,tabla[pocetna_pozicija_i][j].t2.m_z);
								glVertex3f(tabla[pocetna_pozicija_i][j].t3.m_x,tabla[pocetna_pozicija_i][j].t3.m_y,tabla[pocetna_pozicija_i][j].t3.m_z);
								glVertex3f(tabla[pocetna_pozicija_i][j].t4.m_x,tabla[pocetna_pozicija_i][j].t4.m_y,tabla[pocetna_pozicija_i][j].t4.m_z);
						glEnd();
						break;
					}

				}


				for(int j=pocetna_pozicija_j-1;j>-1;j--)
				{
					if(tabla[pocetna_pozicija_i][j].figura==NULL)
					{
						glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
						glColor3f(0,0,0);
						glBegin(GL_POLYGON);
								glVertex3f(tabla[pocetna_pozicija_i][j].t1.m_x,tabla[pocetna_pozicija_i][j].t1.m_y,tabla[pocetna_pozicija_i][j].t1.m_z);
								glVertex3f(tabla[pocetna_pozicija_i][j].t2.m_x,tabla[pocetna_pozicija_i][j].t2.m_y,tabla[pocetna_pozicija_i][j].t2.m_z);
								glVertex3f(tabla[pocetna_pozicija_i][j].t3.m_x,tabla[pocetna_pozicija_i][j].t3.m_y,tabla[pocetna_pozicija_i][j].t3.m_z);
								glVertex3f(tabla[pocetna_pozicija_i][j].t4.m_x,tabla[pocetna_pozicija_i][j].t4.m_y,tabla[pocetna_pozicija_i][j].t4.m_z);
						glEnd();
						glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);	
						glColor3f(0.6,0.9,0.6);
							glBegin(GL_POLYGON);
								glVertex3f(tabla[pocetna_pozicija_i][j].t1.m_x,tabla[pocetna_pozicija_i][j].t1.m_y,tabla[pocetna_pozicija_i][j].t1.m_z);
								glVertex3f(tabla[pocetna_pozicija_i][j].t2.m_x,tabla[pocetna_pozicija_i][j].t2.m_y,tabla[pocetna_pozicija_i][j].t2.m_z);
								glVertex3f(tabla[pocetna_pozicija_i][j].t3.m_x,tabla[pocetna_pozicija_i][j].t3.m_y,tabla[pocetna_pozicija_i][j].t3.m_z);
								glVertex3f(tabla[pocetna_pozicija_i][j].t4.m_x,tabla[pocetna_pozicija_i][j].t4.m_y,tabla[pocetna_pozicija_i][j].t4.m_z);
							glEnd();
					}
					if(tabla[pocetna_pozicija_i][j].figura!=NULL)
					{
						if(tabla[pocetna_pozicija_i][j].figura->ind==tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura->ind)break;
						glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
						glColor3f(0,0,0);
						glBegin(GL_POLYGON);
								glVertex3f(tabla[pocetna_pozicija_i][j].t1.m_x,tabla[pocetna_pozicija_i][j].t1.m_y,tabla[pocetna_pozicija_i][j].t1.m_z);
								glVertex3f(tabla[pocetna_pozicija_i][j].t2.m_x,tabla[pocetna_pozicija_i][j].t2.m_y,tabla[pocetna_pozicija_i][j].t2.m_z);
								glVertex3f(tabla[pocetna_pozicija_i][j].t3.m_x,tabla[pocetna_pozicija_i][j].t3.m_y,tabla[pocetna_pozicija_i][j].t3.m_z);
								glVertex3f(tabla[pocetna_pozicija_i][j].t4.m_x,tabla[pocetna_pozicija_i][j].t4.m_y,tabla[pocetna_pozicija_i][j].t4.m_z);
						glEnd();
						glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
						glColor3f(0.9,0.5,0.5);
						glBegin(GL_POLYGON);
								glVertex3f(tabla[pocetna_pozicija_i][j].t1.m_x,tabla[pocetna_pozicija_i][j].t1.m_y,tabla[pocetna_pozicija_i][j].t1.m_z);
								glVertex3f(tabla[pocetna_pozicija_i][j].t2.m_x,tabla[pocetna_pozicija_i][j].t2.m_y,tabla[pocetna_pozicija_i][j].t2.m_z);
								glVertex3f(tabla[pocetna_pozicija_i][j].t3.m_x,tabla[pocetna_pozicija_i][j].t3.m_y,tabla[pocetna_pozicija_i][j].t3.m_z);
								glVertex3f(tabla[pocetna_pozicija_i][j].t4.m_x,tabla[pocetna_pozicija_i][j].t4.m_y,tabla[pocetna_pozicija_i][j].t4.m_z);
						glEnd();
						break;
					}

				}

			}



			if(strcmp(typeid(*tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura).name(),"class Lovac")==0)
			{
				int i=pocetna_pozicija_i+1;
				int j=pocetna_pozicija_j+1;
				while(i<tabla_n&&j<tabla_n)
				{
					if(tabla[i][j].figura==NULL)
					{
						glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
						glColor3f(0,0,0);
						glBegin(GL_POLYGON);
								glVertex3f(tabla[i][j].t1.m_x,tabla[i][j].t1.m_y,tabla[i][j].t1.m_z);
								glVertex3f(tabla[i][j].t2.m_x,tabla[i][j].t2.m_y,tabla[i][j].t2.m_z);
								glVertex3f(tabla[i][j].t3.m_x,tabla[i][j].t3.m_y,tabla[i][j].t3.m_z);
								glVertex3f(tabla[i][j].t4.m_x,tabla[i][j].t4.m_y,tabla[i][j].t4.m_z);
						glEnd();
						glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
								glColor3f(0.6,0.9,0.6);
								glBegin(GL_POLYGON);
								glVertex3f(tabla[i][j].t1.m_x,tabla[i][j].t1.m_y,tabla[i][j].t1.m_z);
								glVertex3f(tabla[i][j].t2.m_x,tabla[i][j].t2.m_y,tabla[i][j].t2.m_z);
								glVertex3f(tabla[i][j].t3.m_x,tabla[i][j].t3.m_y,tabla[i][j].t3.m_z);
								glVertex3f(tabla[i][j].t4.m_x,tabla[i][j].t4.m_y,tabla[i][j].t4.m_z);
								glEnd();

					}
					if(tabla[i][j].figura!=NULL)
					{
						if(tabla[i][j].figura->ind==tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura->ind)break;
						glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
						glColor3f(0,0,0);
						glBegin(GL_POLYGON);
								glVertex3f(tabla[i][j].t1.m_x,tabla[i][j].t1.m_y,tabla[i][j].t1.m_z);
								glVertex3f(tabla[i][j].t2.m_x,tabla[i][j].t2.m_y,tabla[i][j].t2.m_z);
								glVertex3f(tabla[i][j].t3.m_x,tabla[i][j].t3.m_y,tabla[i][j].t3.m_z);
								glVertex3f(tabla[i][j].t4.m_x,tabla[i][j].t4.m_y,tabla[i][j].t4.m_z);
						glEnd();
						glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);		
						glColor3f(0.9,0.5,0.5);
								glBegin(GL_POLYGON);
								glVertex3f(tabla[i][j].t1.m_x,tabla[i][j].t1.m_y,tabla[i][j].t1.m_z);
								glVertex3f(tabla[i][j].t2.m_x,tabla[i][j].t2.m_y,tabla[i][j].t2.m_z);
								glVertex3f(tabla[i][j].t3.m_x,tabla[i][j].t3.m_y,tabla[i][j].t3.m_z);
								glVertex3f(tabla[i][j].t4.m_x,tabla[i][j].t4.m_y,tabla[i][j].t4.m_z);
								glEnd();
						break;
					}
					i++;
					j++;
				}

				i=pocetna_pozicija_i-1;
				j=pocetna_pozicija_j-1;
				while(i>=0&&j>=0)
				{
					if(tabla[i][j].figura==NULL)
					{
						glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
						glColor3f(0,0,0);
						glBegin(GL_POLYGON);
								glVertex3f(tabla[i][j].t1.m_x,tabla[i][j].t1.m_y,tabla[i][j].t1.m_z);
								glVertex3f(tabla[i][j].t2.m_x,tabla[i][j].t2.m_y,tabla[i][j].t2.m_z);
								glVertex3f(tabla[i][j].t3.m_x,tabla[i][j].t3.m_y,tabla[i][j].t3.m_z);
								glVertex3f(tabla[i][j].t4.m_x,tabla[i][j].t4.m_y,tabla[i][j].t4.m_z);
						glEnd();
						glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
								glColor3f(0.6,0.9,0.6);
								glBegin(GL_POLYGON);
								glVertex3f(tabla[i][j].t1.m_x,tabla[i][j].t1.m_y,tabla[i][j].t1.m_z);
								glVertex3f(tabla[i][j].t2.m_x,tabla[i][j].t2.m_y,tabla[i][j].t2.m_z);
								glVertex3f(tabla[i][j].t3.m_x,tabla[i][j].t3.m_y,tabla[i][j].t3.m_z);
								glVertex3f(tabla[i][j].t4.m_x,tabla[i][j].t4.m_y,tabla[i][j].t4.m_z);
								glEnd();

					}
					if(tabla[i][j].figura!=NULL)
					{
						if(tabla[i][j].figura->ind==tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura->ind)break;
						glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
						glColor3f(0,0,0);
						glBegin(GL_POLYGON);
								glVertex3f(tabla[i][j].t1.m_x,tabla[i][j].t1.m_y,tabla[i][j].t1.m_z);
								glVertex3f(tabla[i][j].t2.m_x,tabla[i][j].t2.m_y,tabla[i][j].t2.m_z);
								glVertex3f(tabla[i][j].t3.m_x,tabla[i][j].t3.m_y,tabla[i][j].t3.m_z);
								glVertex3f(tabla[i][j].t4.m_x,tabla[i][j].t4.m_y,tabla[i][j].t4.m_z);
						glEnd();
						glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);		
						glColor3f(0.9,0.5,0.5);
								glBegin(GL_POLYGON);
								glVertex3f(tabla[i][j].t1.m_x,tabla[i][j].t1.m_y,tabla[i][j].t1.m_z);
								glVertex3f(tabla[i][j].t2.m_x,tabla[i][j].t2.m_y,tabla[i][j].t2.m_z);
								glVertex3f(tabla[i][j].t3.m_x,tabla[i][j].t3.m_y,tabla[i][j].t3.m_z);
								glVertex3f(tabla[i][j].t4.m_x,tabla[i][j].t4.m_y,tabla[i][j].t4.m_z);
								glEnd();
						break;
					}
					i--;
					j--;
				}


				i=pocetna_pozicija_i+1;
				j=pocetna_pozicija_j-1;
				while(i<tabla_n&&j>=0)
				{
					if(tabla[i][j].figura==NULL)
					{
						glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
						glColor3f(0,0,0);
						glBegin(GL_POLYGON);
								glVertex3f(tabla[i][j].t1.m_x,tabla[i][j].t1.m_y,tabla[i][j].t1.m_z);
								glVertex3f(tabla[i][j].t2.m_x,tabla[i][j].t2.m_y,tabla[i][j].t2.m_z);
								glVertex3f(tabla[i][j].t3.m_x,tabla[i][j].t3.m_y,tabla[i][j].t3.m_z);
								glVertex3f(tabla[i][j].t4.m_x,tabla[i][j].t4.m_y,tabla[i][j].t4.m_z);
						glEnd();
						glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
								glColor3f(0.6,0.9,0.6);
								glBegin(GL_POLYGON);
								glVertex3f(tabla[i][j].t1.m_x,tabla[i][j].t1.m_y,tabla[i][j].t1.m_z);
								glVertex3f(tabla[i][j].t2.m_x,tabla[i][j].t2.m_y,tabla[i][j].t2.m_z);
								glVertex3f(tabla[i][j].t3.m_x,tabla[i][j].t3.m_y,tabla[i][j].t3.m_z);
								glVertex3f(tabla[i][j].t4.m_x,tabla[i][j].t4.m_y,tabla[i][j].t4.m_z);
								glEnd();

					}
					if(tabla[i][j].figura!=NULL)
					{
						if(tabla[i][j].figura->ind==tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura->ind)break;
						glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
						glColor3f(0,0,0);
						glBegin(GL_POLYGON);
								glVertex3f(tabla[i][j].t1.m_x,tabla[i][j].t1.m_y,tabla[i][j].t1.m_z);
								glVertex3f(tabla[i][j].t2.m_x,tabla[i][j].t2.m_y,tabla[i][j].t2.m_z);
								glVertex3f(tabla[i][j].t3.m_x,tabla[i][j].t3.m_y,tabla[i][j].t3.m_z);
								glVertex3f(tabla[i][j].t4.m_x,tabla[i][j].t4.m_y,tabla[i][j].t4.m_z);
						glEnd();
						glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);		
						glColor3f(0.9,0.5,0.5);
								glBegin(GL_POLYGON);
								glVertex3f(tabla[i][j].t1.m_x,tabla[i][j].t1.m_y,tabla[i][j].t1.m_z);
								glVertex3f(tabla[i][j].t2.m_x,tabla[i][j].t2.m_y,tabla[i][j].t2.m_z);
								glVertex3f(tabla[i][j].t3.m_x,tabla[i][j].t3.m_y,tabla[i][j].t3.m_z);
								glVertex3f(tabla[i][j].t4.m_x,tabla[i][j].t4.m_y,tabla[i][j].t4.m_z);
								glEnd();
						break;
					}
					i++;
					j--;
				}

			



				i=pocetna_pozicija_i-1;
				j=pocetna_pozicija_j+1;
				while(i>=0&&j<tabla_n)
				{
					if(tabla[i][j].figura==NULL)
					{
						glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
						glColor3f(0,0,0);
						glBegin(GL_POLYGON);
								glVertex3f(tabla[i][j].t1.m_x,tabla[i][j].t1.m_y,tabla[i][j].t1.m_z);
								glVertex3f(tabla[i][j].t2.m_x,tabla[i][j].t2.m_y,tabla[i][j].t2.m_z);
								glVertex3f(tabla[i][j].t3.m_x,tabla[i][j].t3.m_y,tabla[i][j].t3.m_z);
								glVertex3f(tabla[i][j].t4.m_x,tabla[i][j].t4.m_y,tabla[i][j].t4.m_z);
						glEnd();
						glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);		
						glColor3f(0.6,0.9,0.6);
								glBegin(GL_POLYGON);
								glVertex3f(tabla[i][j].t1.m_x,tabla[i][j].t1.m_y,tabla[i][j].t1.m_z);
								glVertex3f(tabla[i][j].t2.m_x,tabla[i][j].t2.m_y,tabla[i][j].t2.m_z);
								glVertex3f(tabla[i][j].t3.m_x,tabla[i][j].t3.m_y,tabla[i][j].t3.m_z);
								glVertex3f(tabla[i][j].t4.m_x,tabla[i][j].t4.m_y,tabla[i][j].t4.m_z);
								glEnd();

					}
					if(tabla[i][j].figura!=NULL)
					{
						if(tabla[i][j].figura->ind==tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura->ind)break;
						glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
						glColor3f(0,0,0);
						glBegin(GL_POLYGON);
								glVertex3f(tabla[i][j].t1.m_x,tabla[i][j].t1.m_y,tabla[i][j].t1.m_z);
								glVertex3f(tabla[i][j].t2.m_x,tabla[i][j].t2.m_y,tabla[i][j].t2.m_z);
								glVertex3f(tabla[i][j].t3.m_x,tabla[i][j].t3.m_y,tabla[i][j].t3.m_z);
								glVertex3f(tabla[i][j].t4.m_x,tabla[i][j].t4.m_y,tabla[i][j].t4.m_z);
						glEnd();
						glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);		
						glColor3f(0.9,0.5,0.5);
								glBegin(GL_POLYGON);
								glVertex3f(tabla[i][j].t1.m_x,tabla[i][j].t1.m_y,tabla[i][j].t1.m_z);
								glVertex3f(tabla[i][j].t2.m_x,tabla[i][j].t2.m_y,tabla[i][j].t2.m_z);
								glVertex3f(tabla[i][j].t3.m_x,tabla[i][j].t3.m_y,tabla[i][j].t3.m_z);
								glVertex3f(tabla[i][j].t4.m_x,tabla[i][j].t4.m_y,tabla[i][j].t4.m_z);
								glEnd();
						break;
					}
					i--;
					j++;
				}
			}



			if(strcmp(typeid(*tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura).name(),"class Kraljica")==0)
			{

				for(int i=pocetna_pozicija_i+1;i<tabla_n;i++)
				{
					if(tabla[i][pocetna_pozicija_j].figura==NULL)
					{
						glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
						glColor3f(0,0,0);
						glBegin(GL_POLYGON);
								glVertex3f(tabla[i][pocetna_pozicija_j].t1.m_x,tabla[i][pocetna_pozicija_j].t1.m_y,tabla[i][pocetna_pozicija_j].t1.m_z);
								glVertex3f(tabla[i][pocetna_pozicija_j].t2.m_x,tabla[i][pocetna_pozicija_j].t2.m_y,tabla[i][pocetna_pozicija_j].t2.m_z);
								glVertex3f(tabla[i][pocetna_pozicija_j].t3.m_x,tabla[i][pocetna_pozicija_j].t3.m_y,tabla[i][pocetna_pozicija_j].t3.m_z);
								glVertex3f(tabla[i][pocetna_pozicija_j].t4.m_x,tabla[i][pocetna_pozicija_j].t4.m_y,tabla[i][pocetna_pozicija_j].t4.m_z);
						glEnd();
						glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
							glColor3f(0.6,0.9,0.6);
							glBegin(GL_POLYGON);
								glVertex3f(tabla[i][pocetna_pozicija_j].t1.m_x,tabla[i][pocetna_pozicija_j].t1.m_y,tabla[i][pocetna_pozicija_j].t1.m_z);
								glVertex3f(tabla[i][pocetna_pozicija_j].t2.m_x,tabla[i][pocetna_pozicija_j].t2.m_y,tabla[i][pocetna_pozicija_j].t2.m_z);
								glVertex3f(tabla[i][pocetna_pozicija_j].t3.m_x,tabla[i][pocetna_pozicija_j].t3.m_y,tabla[i][pocetna_pozicija_j].t3.m_z);
								glVertex3f(tabla[i][pocetna_pozicija_j].t4.m_x,tabla[i][pocetna_pozicija_j].t4.m_y,tabla[i][pocetna_pozicija_j].t4.m_z);
							glEnd();
					}
					if(tabla[i][pocetna_pozicija_j].figura!=NULL)
					{
						if(tabla[i][pocetna_pozicija_j].figura->ind==tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura->ind)break;
						glColor3f(0.9,0.5,0.5);
						glBegin(GL_POLYGON);
								glVertex3f(tabla[i][pocetna_pozicija_j].t1.m_x,tabla[i][pocetna_pozicija_j].t1.m_y,tabla[i][pocetna_pozicija_j].t1.m_z);
								glVertex3f(tabla[i][pocetna_pozicija_j].t2.m_x,tabla[i][pocetna_pozicija_j].t2.m_y,tabla[i][pocetna_pozicija_j].t2.m_z);
								glVertex3f(tabla[i][pocetna_pozicija_j].t3.m_x,tabla[i][pocetna_pozicija_j].t3.m_y,tabla[i][pocetna_pozicija_j].t3.m_z);
								glVertex3f(tabla[i][pocetna_pozicija_j].t4.m_x,tabla[i][pocetna_pozicija_j].t4.m_y,tabla[i][pocetna_pozicija_j].t4.m_z);
						glEnd();
						break;
					}
				}

				for(int i=pocetna_pozicija_i-1;i>-1;i--)
				{
					if(tabla[i][pocetna_pozicija_j].figura==NULL)
					{
						glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
						glColor3f(0,0,0);
						glBegin(GL_POLYGON);
								glVertex3f(tabla[i][pocetna_pozicija_j].t1.m_x,tabla[i][pocetna_pozicija_j].t1.m_y,tabla[i][pocetna_pozicija_j].t1.m_z);
								glVertex3f(tabla[i][pocetna_pozicija_j].t2.m_x,tabla[i][pocetna_pozicija_j].t2.m_y,tabla[i][pocetna_pozicija_j].t2.m_z);
								glVertex3f(tabla[i][pocetna_pozicija_j].t3.m_x,tabla[i][pocetna_pozicija_j].t3.m_y,tabla[i][pocetna_pozicija_j].t3.m_z);
								glVertex3f(tabla[i][pocetna_pozicija_j].t4.m_x,tabla[i][pocetna_pozicija_j].t4.m_y,tabla[i][pocetna_pozicija_j].t4.m_z);
						glEnd();
						glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
							glColor3f(0.6,0.9,0.6);
							glBegin(GL_POLYGON);
								glVertex3f(tabla[i][pocetna_pozicija_j].t1.m_x,tabla[i][pocetna_pozicija_j].t1.m_y,tabla[i][pocetna_pozicija_j].t1.m_z);
								glVertex3f(tabla[i][pocetna_pozicija_j].t2.m_x,tabla[i][pocetna_pozicija_j].t2.m_y,tabla[i][pocetna_pozicija_j].t2.m_z);
								glVertex3f(tabla[i][pocetna_pozicija_j].t3.m_x,tabla[i][pocetna_pozicija_j].t3.m_y,tabla[i][pocetna_pozicija_j].t3.m_z);
								glVertex3f(tabla[i][pocetna_pozicija_j].t4.m_x,tabla[i][pocetna_pozicija_j].t4.m_y,tabla[i][pocetna_pozicija_j].t4.m_z);
							glEnd();
					}
					if(tabla[i][pocetna_pozicija_j].figura!=NULL)
					{

						if(tabla[i][pocetna_pozicija_j].figura->ind==tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura->ind)break;
						
						glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
						glColor3f(0,0,0);
						glBegin(GL_POLYGON);
								glVertex3f(tabla[i][pocetna_pozicija_j].t1.m_x,tabla[i][pocetna_pozicija_j].t1.m_y,tabla[i][pocetna_pozicija_j].t1.m_z);
								glVertex3f(tabla[i][pocetna_pozicija_j].t2.m_x,tabla[i][pocetna_pozicija_j].t2.m_y,tabla[i][pocetna_pozicija_j].t2.m_z);
								glVertex3f(tabla[i][pocetna_pozicija_j].t3.m_x,tabla[i][pocetna_pozicija_j].t3.m_y,tabla[i][pocetna_pozicija_j].t3.m_z);
								glVertex3f(tabla[i][pocetna_pozicija_j].t4.m_x,tabla[i][pocetna_pozicija_j].t4.m_y,tabla[i][pocetna_pozicija_j].t4.m_z);
						glEnd();
						glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
						glColor3f(0.9,0.5,0.5);
						glBegin(GL_POLYGON);
								glVertex3f(tabla[i][pocetna_pozicija_j].t1.m_x,tabla[i][pocetna_pozicija_j].t1.m_y,tabla[i][pocetna_pozicija_j].t1.m_z);
								glVertex3f(tabla[i][pocetna_pozicija_j].t2.m_x,tabla[i][pocetna_pozicija_j].t2.m_y,tabla[i][pocetna_pozicija_j].t2.m_z);
								glVertex3f(tabla[i][pocetna_pozicija_j].t3.m_x,tabla[i][pocetna_pozicija_j].t3.m_y,tabla[i][pocetna_pozicija_j].t3.m_z);
								glVertex3f(tabla[i][pocetna_pozicija_j].t4.m_x,tabla[i][pocetna_pozicija_j].t4.m_y,tabla[i][pocetna_pozicija_j].t4.m_z);
						glEnd();
						break;
					}

				}


				for(int j=pocetna_pozicija_j+1;j<tabla_n;j++)
				{
					if(tabla[pocetna_pozicija_i][j].figura==NULL)
					{
						glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
						glColor3f(0,0,0);
						glBegin(GL_POLYGON);
								glVertex3f(tabla[pocetna_pozicija_i][j].t1.m_x,tabla[pocetna_pozicija_i][j].t1.m_y,tabla[pocetna_pozicija_i][j].t1.m_z);
								glVertex3f(tabla[pocetna_pozicija_i][j].t2.m_x,tabla[pocetna_pozicija_i][j].t2.m_y,tabla[pocetna_pozicija_i][j].t2.m_z);
								glVertex3f(tabla[pocetna_pozicija_i][j].t3.m_x,tabla[pocetna_pozicija_i][j].t3.m_y,tabla[pocetna_pozicija_i][j].t3.m_z);
								glVertex3f(tabla[pocetna_pozicija_i][j].t4.m_x,tabla[pocetna_pozicija_i][j].t4.m_y,tabla[pocetna_pozicija_i][j].t4.m_z);
						glEnd();
						glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
							glColor3f(0.6,0.9,0.6);
							glBegin(GL_POLYGON);
								glVertex3f(tabla[pocetna_pozicija_i][j].t1.m_x,tabla[pocetna_pozicija_i][j].t1.m_y,tabla[pocetna_pozicija_i][j].t1.m_z);
								glVertex3f(tabla[pocetna_pozicija_i][j].t2.m_x,tabla[pocetna_pozicija_i][j].t2.m_y,tabla[pocetna_pozicija_i][j].t2.m_z);
								glVertex3f(tabla[pocetna_pozicija_i][j].t3.m_x,tabla[pocetna_pozicija_i][j].t3.m_y,tabla[pocetna_pozicija_i][j].t3.m_z);
								glVertex3f(tabla[pocetna_pozicija_i][j].t4.m_x,tabla[pocetna_pozicija_i][j].t4.m_y,tabla[pocetna_pozicija_i][j].t4.m_z);
							glEnd();
					}
					if(tabla[pocetna_pozicija_i][j].figura!=NULL)
					{
						if(tabla[pocetna_pozicija_i][j].figura->ind==tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura->ind)break;
						glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
						glColor3f(0,0,0);
						glBegin(GL_POLYGON);
								glVertex3f(tabla[pocetna_pozicija_i][j].t1.m_x,tabla[pocetna_pozicija_i][j].t1.m_y,tabla[pocetna_pozicija_i][j].t1.m_z);
								glVertex3f(tabla[pocetna_pozicija_i][j].t2.m_x,tabla[pocetna_pozicija_i][j].t2.m_y,tabla[pocetna_pozicija_i][j].t2.m_z);
								glVertex3f(tabla[pocetna_pozicija_i][j].t3.m_x,tabla[pocetna_pozicija_i][j].t3.m_y,tabla[pocetna_pozicija_i][j].t3.m_z);
								glVertex3f(tabla[pocetna_pozicija_i][j].t4.m_x,tabla[pocetna_pozicija_i][j].t4.m_y,tabla[pocetna_pozicija_i][j].t4.m_z);
						glEnd();
						glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
						glColor3f(0.9,0.5,0.5);
						glBegin(GL_POLYGON);
								glVertex3f(tabla[pocetna_pozicija_i][j].t1.m_x,tabla[pocetna_pozicija_i][j].t1.m_y,tabla[pocetna_pozicija_i][j].t1.m_z);
								glVertex3f(tabla[pocetna_pozicija_i][j].t2.m_x,tabla[pocetna_pozicija_i][j].t2.m_y,tabla[pocetna_pozicija_i][j].t2.m_z);
								glVertex3f(tabla[pocetna_pozicija_i][j].t3.m_x,tabla[pocetna_pozicija_i][j].t3.m_y,tabla[pocetna_pozicija_i][j].t3.m_z);
								glVertex3f(tabla[pocetna_pozicija_i][j].t4.m_x,tabla[pocetna_pozicija_i][j].t4.m_y,tabla[pocetna_pozicija_i][j].t4.m_z);
						glEnd();
						break;
					}

				}


				for(int j=pocetna_pozicija_j-1;j>-1;j--)
				{
					if(tabla[pocetna_pozicija_i][j].figura==NULL)
					{
						glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
						glColor3f(0,0,0);
						glBegin(GL_POLYGON);
								glVertex3f(tabla[pocetna_pozicija_i][j].t1.m_x,tabla[pocetna_pozicija_i][j].t1.m_y,tabla[pocetna_pozicija_i][j].t1.m_z);
								glVertex3f(tabla[pocetna_pozicija_i][j].t2.m_x,tabla[pocetna_pozicija_i][j].t2.m_y,tabla[pocetna_pozicija_i][j].t2.m_z);
								glVertex3f(tabla[pocetna_pozicija_i][j].t3.m_x,tabla[pocetna_pozicija_i][j].t3.m_y,tabla[pocetna_pozicija_i][j].t3.m_z);
								glVertex3f(tabla[pocetna_pozicija_i][j].t4.m_x,tabla[pocetna_pozicija_i][j].t4.m_y,tabla[pocetna_pozicija_i][j].t4.m_z);
						glEnd();
						glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);	
						glColor3f(0.6,0.9,0.6);
							glBegin(GL_POLYGON);
								glVertex3f(tabla[pocetna_pozicija_i][j].t1.m_x,tabla[pocetna_pozicija_i][j].t1.m_y,tabla[pocetna_pozicija_i][j].t1.m_z);
								glVertex3f(tabla[pocetna_pozicija_i][j].t2.m_x,tabla[pocetna_pozicija_i][j].t2.m_y,tabla[pocetna_pozicija_i][j].t2.m_z);
								glVertex3f(tabla[pocetna_pozicija_i][j].t3.m_x,tabla[pocetna_pozicija_i][j].t3.m_y,tabla[pocetna_pozicija_i][j].t3.m_z);
								glVertex3f(tabla[pocetna_pozicija_i][j].t4.m_x,tabla[pocetna_pozicija_i][j].t4.m_y,tabla[pocetna_pozicija_i][j].t4.m_z);
							glEnd();
					}
					if(tabla[pocetna_pozicija_i][j].figura!=NULL)
					{
						if(tabla[pocetna_pozicija_i][j].figura->ind==tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura->ind)break;
						glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
						glColor3f(0,0,0);
						glBegin(GL_POLYGON);
								glVertex3f(tabla[pocetna_pozicija_i][j].t1.m_x,tabla[pocetna_pozicija_i][j].t1.m_y,tabla[pocetna_pozicija_i][j].t1.m_z);
								glVertex3f(tabla[pocetna_pozicija_i][j].t2.m_x,tabla[pocetna_pozicija_i][j].t2.m_y,tabla[pocetna_pozicija_i][j].t2.m_z);
								glVertex3f(tabla[pocetna_pozicija_i][j].t3.m_x,tabla[pocetna_pozicija_i][j].t3.m_y,tabla[pocetna_pozicija_i][j].t3.m_z);
								glVertex3f(tabla[pocetna_pozicija_i][j].t4.m_x,tabla[pocetna_pozicija_i][j].t4.m_y,tabla[pocetna_pozicija_i][j].t4.m_z);
						glEnd();
						glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
						glColor3f(0.9,0.5,0.5);
						glBegin(GL_POLYGON);
								glVertex3f(tabla[pocetna_pozicija_i][j].t1.m_x,tabla[pocetna_pozicija_i][j].t1.m_y,tabla[pocetna_pozicija_i][j].t1.m_z);
								glVertex3f(tabla[pocetna_pozicija_i][j].t2.m_x,tabla[pocetna_pozicija_i][j].t2.m_y,tabla[pocetna_pozicija_i][j].t2.m_z);
								glVertex3f(tabla[pocetna_pozicija_i][j].t3.m_x,tabla[pocetna_pozicija_i][j].t3.m_y,tabla[pocetna_pozicija_i][j].t3.m_z);
								glVertex3f(tabla[pocetna_pozicija_i][j].t4.m_x,tabla[pocetna_pozicija_i][j].t4.m_y,tabla[pocetna_pozicija_i][j].t4.m_z);
						glEnd();
						break;
					}

				}	





				int i=pocetna_pozicija_i+1;
				int j=pocetna_pozicija_j+1;
				while(i<tabla_n&&j<tabla_n)
				{
					if(tabla[i][j].figura==NULL)
					{
						glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
						glColor3f(0,0,0);
						glBegin(GL_POLYGON);
								glVertex3f(tabla[i][j].t1.m_x,tabla[i][j].t1.m_y,tabla[i][j].t1.m_z);
								glVertex3f(tabla[i][j].t2.m_x,tabla[i][j].t2.m_y,tabla[i][j].t2.m_z);
								glVertex3f(tabla[i][j].t3.m_x,tabla[i][j].t3.m_y,tabla[i][j].t3.m_z);
								glVertex3f(tabla[i][j].t4.m_x,tabla[i][j].t4.m_y,tabla[i][j].t4.m_z);
						glEnd();
						glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
								glColor3f(0.6,0.9,0.6);
								glBegin(GL_POLYGON);
								glVertex3f(tabla[i][j].t1.m_x,tabla[i][j].t1.m_y,tabla[i][j].t1.m_z);
								glVertex3f(tabla[i][j].t2.m_x,tabla[i][j].t2.m_y,tabla[i][j].t2.m_z);
								glVertex3f(tabla[i][j].t3.m_x,tabla[i][j].t3.m_y,tabla[i][j].t3.m_z);
								glVertex3f(tabla[i][j].t4.m_x,tabla[i][j].t4.m_y,tabla[i][j].t4.m_z);
								glEnd();

					}
					if(tabla[i][j].figura!=NULL)
					{
						if(tabla[i][j].figura->ind==tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura->ind)break;
						glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
						glColor3f(0,0,0);
						glBegin(GL_POLYGON);
								glVertex3f(tabla[i][j].t1.m_x,tabla[i][j].t1.m_y,tabla[i][j].t1.m_z);
								glVertex3f(tabla[i][j].t2.m_x,tabla[i][j].t2.m_y,tabla[i][j].t2.m_z);
								glVertex3f(tabla[i][j].t3.m_x,tabla[i][j].t3.m_y,tabla[i][j].t3.m_z);
								glVertex3f(tabla[i][j].t4.m_x,tabla[i][j].t4.m_y,tabla[i][j].t4.m_z);
						glEnd();
						glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);		
						glColor3f(0.9,0.5,0.5);
								glBegin(GL_POLYGON);
								glVertex3f(tabla[i][j].t1.m_x,tabla[i][j].t1.m_y,tabla[i][j].t1.m_z);
								glVertex3f(tabla[i][j].t2.m_x,tabla[i][j].t2.m_y,tabla[i][j].t2.m_z);
								glVertex3f(tabla[i][j].t3.m_x,tabla[i][j].t3.m_y,tabla[i][j].t3.m_z);
								glVertex3f(tabla[i][j].t4.m_x,tabla[i][j].t4.m_y,tabla[i][j].t4.m_z);
								glEnd();
						break;
					}
					i++;
					j++;
				}

				i=pocetna_pozicija_i-1;
				j=pocetna_pozicija_j-1;
				while(i>=0&&j>=0)
				{
					if(tabla[i][j].figura==NULL)
					{
						glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
						glColor3f(0,0,0);
						glBegin(GL_POLYGON);
								glVertex3f(tabla[i][j].t1.m_x,tabla[i][j].t1.m_y,tabla[i][j].t1.m_z);
								glVertex3f(tabla[i][j].t2.m_x,tabla[i][j].t2.m_y,tabla[i][j].t2.m_z);
								glVertex3f(tabla[i][j].t3.m_x,tabla[i][j].t3.m_y,tabla[i][j].t3.m_z);
								glVertex3f(tabla[i][j].t4.m_x,tabla[i][j].t4.m_y,tabla[i][j].t4.m_z);
						glEnd();
						glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
								glColor3f(0.6,0.9,0.6);
								glBegin(GL_POLYGON);
								glVertex3f(tabla[i][j].t1.m_x,tabla[i][j].t1.m_y,tabla[i][j].t1.m_z);
								glVertex3f(tabla[i][j].t2.m_x,tabla[i][j].t2.m_y,tabla[i][j].t2.m_z);
								glVertex3f(tabla[i][j].t3.m_x,tabla[i][j].t3.m_y,tabla[i][j].t3.m_z);
								glVertex3f(tabla[i][j].t4.m_x,tabla[i][j].t4.m_y,tabla[i][j].t4.m_z);
								glEnd();

					}
					if(tabla[i][j].figura!=NULL)
					{
						if(tabla[i][j].figura->ind==tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura->ind)break;
						glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
						glColor3f(0,0,0);
						glBegin(GL_POLYGON);
								glVertex3f(tabla[i][j].t1.m_x,tabla[i][j].t1.m_y,tabla[i][j].t1.m_z);
								glVertex3f(tabla[i][j].t2.m_x,tabla[i][j].t2.m_y,tabla[i][j].t2.m_z);
								glVertex3f(tabla[i][j].t3.m_x,tabla[i][j].t3.m_y,tabla[i][j].t3.m_z);
								glVertex3f(tabla[i][j].t4.m_x,tabla[i][j].t4.m_y,tabla[i][j].t4.m_z);
						glEnd();
						glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);		
						glColor3f(0.9,0.5,0.5);
								glBegin(GL_POLYGON);
								glVertex3f(tabla[i][j].t1.m_x,tabla[i][j].t1.m_y,tabla[i][j].t1.m_z);
								glVertex3f(tabla[i][j].t2.m_x,tabla[i][j].t2.m_y,tabla[i][j].t2.m_z);
								glVertex3f(tabla[i][j].t3.m_x,tabla[i][j].t3.m_y,tabla[i][j].t3.m_z);
								glVertex3f(tabla[i][j].t4.m_x,tabla[i][j].t4.m_y,tabla[i][j].t4.m_z);
								glEnd();
						break;
					}
					i--;
					j--;
				}


				i=pocetna_pozicija_i+1;
				j=pocetna_pozicija_j-1;
				while(i<tabla_n&&j>=0)
				{
					if(tabla[i][j].figura==NULL)
					{
						glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
						glColor3f(0,0,0);
						glBegin(GL_POLYGON);
								glVertex3f(tabla[i][j].t1.m_x,tabla[i][j].t1.m_y,tabla[i][j].t1.m_z);
								glVertex3f(tabla[i][j].t2.m_x,tabla[i][j].t2.m_y,tabla[i][j].t2.m_z);
								glVertex3f(tabla[i][j].t3.m_x,tabla[i][j].t3.m_y,tabla[i][j].t3.m_z);
								glVertex3f(tabla[i][j].t4.m_x,tabla[i][j].t4.m_y,tabla[i][j].t4.m_z);
						glEnd();
						glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
								glColor3f(0.6,0.9,0.6);
								glBegin(GL_POLYGON);
								glVertex3f(tabla[i][j].t1.m_x,tabla[i][j].t1.m_y,tabla[i][j].t1.m_z);
								glVertex3f(tabla[i][j].t2.m_x,tabla[i][j].t2.m_y,tabla[i][j].t2.m_z);
								glVertex3f(tabla[i][j].t3.m_x,tabla[i][j].t3.m_y,tabla[i][j].t3.m_z);
								glVertex3f(tabla[i][j].t4.m_x,tabla[i][j].t4.m_y,tabla[i][j].t4.m_z);
								glEnd();

					}
					if(tabla[i][j].figura!=NULL)
					{
						if(tabla[i][j].figura->ind==tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura->ind)break;
						glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
						glColor3f(0,0,0);
						glBegin(GL_POLYGON);
								glVertex3f(tabla[i][j].t1.m_x,tabla[i][j].t1.m_y,tabla[i][j].t1.m_z);
								glVertex3f(tabla[i][j].t2.m_x,tabla[i][j].t2.m_y,tabla[i][j].t2.m_z);
								glVertex3f(tabla[i][j].t3.m_x,tabla[i][j].t3.m_y,tabla[i][j].t3.m_z);
								glVertex3f(tabla[i][j].t4.m_x,tabla[i][j].t4.m_y,tabla[i][j].t4.m_z);
						glEnd();
						glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);		
						glColor3f(0.9,0.5,0.5);
								glBegin(GL_POLYGON);
								glVertex3f(tabla[i][j].t1.m_x,tabla[i][j].t1.m_y,tabla[i][j].t1.m_z);
								glVertex3f(tabla[i][j].t2.m_x,tabla[i][j].t2.m_y,tabla[i][j].t2.m_z);
								glVertex3f(tabla[i][j].t3.m_x,tabla[i][j].t3.m_y,tabla[i][j].t3.m_z);
								glVertex3f(tabla[i][j].t4.m_x,tabla[i][j].t4.m_y,tabla[i][j].t4.m_z);
								glEnd();
						break;
					}
					i++;
					j--;
				}

			



				i=pocetna_pozicija_i-1;
				j=pocetna_pozicija_j+1;
				while(i>=0&&j<tabla_n)
				{
					if(tabla[i][j].figura==NULL)
					{
						glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
						glColor3f(0,0,0);
						glBegin(GL_POLYGON);
								glVertex3f(tabla[i][j].t1.m_x,tabla[i][j].t1.m_y,tabla[i][j].t1.m_z);
								glVertex3f(tabla[i][j].t2.m_x,tabla[i][j].t2.m_y,tabla[i][j].t2.m_z);
								glVertex3f(tabla[i][j].t3.m_x,tabla[i][j].t3.m_y,tabla[i][j].t3.m_z);
								glVertex3f(tabla[i][j].t4.m_x,tabla[i][j].t4.m_y,tabla[i][j].t4.m_z);
						glEnd();
						glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);		
						glColor3f(0.6,0.9,0.6);
								glBegin(GL_POLYGON);
								glVertex3f(tabla[i][j].t1.m_x,tabla[i][j].t1.m_y,tabla[i][j].t1.m_z);
								glVertex3f(tabla[i][j].t2.m_x,tabla[i][j].t2.m_y,tabla[i][j].t2.m_z);
								glVertex3f(tabla[i][j].t3.m_x,tabla[i][j].t3.m_y,tabla[i][j].t3.m_z);
								glVertex3f(tabla[i][j].t4.m_x,tabla[i][j].t4.m_y,tabla[i][j].t4.m_z);
								glEnd();

					}
					if(tabla[i][j].figura!=NULL)
					{
						if(tabla[i][j].figura->ind==tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura->ind)break;
						glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
						glColor3f(0,0,0);
						glBegin(GL_POLYGON);
								glVertex3f(tabla[i][j].t1.m_x,tabla[i][j].t1.m_y,tabla[i][j].t1.m_z);
								glVertex3f(tabla[i][j].t2.m_x,tabla[i][j].t2.m_y,tabla[i][j].t2.m_z);
								glVertex3f(tabla[i][j].t3.m_x,tabla[i][j].t3.m_y,tabla[i][j].t3.m_z);
								glVertex3f(tabla[i][j].t4.m_x,tabla[i][j].t4.m_y,tabla[i][j].t4.m_z);
						glEnd();
						glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);		
						glColor3f(0.9,0.5,0.5);
								glBegin(GL_POLYGON);
								glVertex3f(tabla[i][j].t1.m_x,tabla[i][j].t1.m_y,tabla[i][j].t1.m_z);
								glVertex3f(tabla[i][j].t2.m_x,tabla[i][j].t2.m_y,tabla[i][j].t2.m_z);
								glVertex3f(tabla[i][j].t3.m_x,tabla[i][j].t3.m_y,tabla[i][j].t3.m_z);
								glVertex3f(tabla[i][j].t4.m_x,tabla[i][j].t4.m_y,tabla[i][j].t4.m_z);
								glEnd();
						break;
					}
					i--;
					j++;
				}

			}


			if(strcmp(typeid(*tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura).name(),"class Kralj")==0)
			{


				if(pocetna_pozicija_i<tabla_n-1)
				{

						if(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j].figura==NULL)
						{

						glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
									glColor3f(0,0,0);
									glBegin(GL_POLYGON);
											glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j].t1.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j].t1.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j].t1.m_z);
											glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j].t2.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j].t2.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j].t2.m_z);
											glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j].t3.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j].t3.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j].t3.m_z);
											glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j].t4.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j].t4.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j].t4.m_z);
									glEnd();
						glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);	

							glColor3f(0.6,0.9,0.6);
								glBegin(GL_POLYGON);
									glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j].t1.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j].t1.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j].t1.m_z);
									glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j].t2.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j].t2.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j].t2.m_z);
									glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j].t3.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j].t3.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j].t3.m_z);
									glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j].t4.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j].t4.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j].t4.m_z);
								glEnd();
						}
						else 
							{
								if(tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura->ind!=tabla[pocetna_pozicija_i+1][pocetna_pozicija_j].figura->ind)
								{
									glColor3f(0.9,0.5,0.5);
								glBegin(GL_POLYGON);
									glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j].t1.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j].t1.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j].t1.m_z);
									glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j].t2.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j].t2.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j].t2.m_z);
									glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j].t3.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j].t3.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j].t3.m_z);
									glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j].t4.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j].t4.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j].t4.m_z);
								glEnd();
								}
							}

					}


				if(pocetna_pozicija_i<tabla_n-1&&pocetna_pozicija_j<tabla_n-1)
				{

						if(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+1].figura==NULL)
						{

							glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
										glColor3f(0,0,0);
										glBegin(GL_POLYGON);
												glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+1].t1.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+1].t1.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+1].t1.m_z);
												glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+1].t2.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+1].t2.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+1].t2.m_z);
												glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+1].t3.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+1].t3.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+1].t3.m_z);
												glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+1].t4.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+1].t4.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+1].t4.m_z);
										glEnd();
										glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);	
							glColor3f(0.6,0.9,0.6);
							glBegin(GL_POLYGON);
									glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+1].t1.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+1].t1.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+1].t1.m_z);
									glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+1].t2.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+1].t2.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+1].t2.m_z);
									glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+1].t3.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+1].t3.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+1].t3.m_z);
									glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+1].t4.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+1].t4.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+1].t4.m_z);
							glEnd();
						}
						else 
							{
								if(tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura->ind!=tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+1].figura->ind)
								{
									glColor3f(0.9,0.5,0.5);
									glBegin(GL_POLYGON);
											glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+1].t1.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+1].t1.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+1].t1.m_z);
											glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+1].t2.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+1].t2.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+1].t2.m_z);
											glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+1].t3.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+1].t3.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+1].t3.m_z);
											glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+1].t4.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+1].t4.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+1].t4.m_z);
									glEnd();
								}
							}

					}


				if(pocetna_pozicija_i<tabla_n-1&&pocetna_pozicija_j>0)
				{

						if(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-1].figura==NULL)
						{

								glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
								glColor3f(0,0,0);
								glBegin(GL_POLYGON);
										glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-1].t1.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-1].t1.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-1].t1.m_z);
										glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-1].t2.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-1].t2.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-1].t2.m_z);
										glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-1].t3.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-1].t3.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-1].t3.m_z);
										glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-1].t4.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-1].t4.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-1].t4.m_z);
								glEnd();
								glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);	
							glColor3f(0.6,0.9,0.6);
							glBegin(GL_POLYGON);
									glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-1].t1.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-1].t1.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-1].t1.m_z);
									glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-1].t2.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-1].t2.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-1].t2.m_z);
									glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-1].t3.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-1].t3.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-1].t3.m_z);
									glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-1].t4.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-1].t4.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-1].t4.m_z);
							glEnd();
						}
						else 
							{
								if(tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura->ind!=tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-1].figura->ind)
								{
									glColor3f(0.9,0.5,0.5);
									glBegin(GL_POLYGON);
											glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-1].t1.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-1].t1.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-1].t1.m_z);
											glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-1].t2.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-1].t2.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-1].t2.m_z);
											glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-1].t3.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-1].t3.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-1].t3.m_z);
											glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-1].t4.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-1].t4.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-1].t4.m_z);
									glEnd();
								}
							}

					}



				if(pocetna_pozicija_i>0)
				{

						if(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j].figura==NULL)
						{

													glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
						glColor3f(0,0,0);
						glBegin(GL_POLYGON);
								glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j].t1.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j].t1.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j].t1.m_z);
								glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j].t2.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j].t2.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j].t2.m_z);
								glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j].t3.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j].t3.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j].t3.m_z);
								glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j].t4.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j].t4.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j].t4.m_z);
						glEnd();
						glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);	
							glColor3f(0.6,0.9,0.6);
						glBegin(GL_POLYGON);
								glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j].t1.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j].t1.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j].t1.m_z);
								glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j].t2.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j].t2.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j].t2.m_z);
								glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j].t3.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j].t3.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j].t3.m_z);
								glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j].t4.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j].t4.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j].t4.m_z);
						glEnd();
						}
						else 
							{
								if(tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura->ind!=tabla[pocetna_pozicija_i-1][pocetna_pozicija_j].figura->ind)
								{
									glColor3f(0.9,0.5,0.5);
						glBegin(GL_POLYGON);
								glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j].t1.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j].t1.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j].t1.m_z);
								glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j].t2.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j].t2.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j].t2.m_z);
								glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j].t3.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j].t3.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j].t3.m_z);
								glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j].t4.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j].t4.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j].t4.m_z);
						glEnd();
								}
							}

					}


				if(pocetna_pozicija_i>0&&pocetna_pozicija_j<tabla_n-1)
				{

						glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);	
						if(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+1].figura==NULL)
						{

													glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
						glColor3f(0,0,0);
						glBegin(GL_POLYGON);
								glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+1].t1.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+1].t1.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+1].t1.m_z);
								glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+1].t2.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+1].t2.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+1].t2.m_z);
								glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+1].t3.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+1].t3.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+1].t3.m_z);
								glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+1].t4.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+1].t4.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+1].t4.m_z);
						glEnd();
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
							glColor3f(0.6,0.9,0.6);
							glBegin(GL_POLYGON);
									glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+1].t1.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+1].t1.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+1].t1.m_z);
									glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+1].t2.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+1].t2.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+1].t2.m_z);
									glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+1].t3.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+1].t3.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+1].t3.m_z);
									glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+1].t4.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+1].t4.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+1].t4.m_z);
							glEnd();
						}
						else 
							{
								if(tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura->ind!=tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+1].figura->ind)
								{
									glColor3f(0.9,0.5,0.5);
									glBegin(GL_POLYGON);
											glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+1].t1.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+1].t1.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+1].t1.m_z);
											glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+1].t2.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+1].t2.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+1].t2.m_z);
											glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+1].t3.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+1].t3.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+1].t3.m_z);
											glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+1].t4.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+1].t4.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+1].t4.m_z);
									glEnd();
								}
							}

					}

				if(pocetna_pozicija_i>0&&pocetna_pozicija_j>0)
				{

						if(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-1].figura==NULL)
						{

													glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
						glColor3f(0,0,0);
						glBegin(GL_POLYGON);
								glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-1].t1.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-1].t1.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-1].t1.m_z);
								glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-1].t2.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-1].t2.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-1].t2.m_z);
								glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-1].t3.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-1].t3.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-1].t3.m_z);
								glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-1].t4.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-1].t4.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-1].t4.m_z);
						glEnd();
						glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);	
							glColor3f(0.6,0.9,0.6);
							glBegin(GL_POLYGON);
									glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-1].t1.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-1].t1.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-1].t1.m_z);
									glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-1].t2.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-1].t2.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-1].t2.m_z);
									glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-1].t3.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-1].t3.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-1].t3.m_z);
									glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-1].t4.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-1].t4.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-1].t4.m_z);
							glEnd();
						}
						else 
							{
								if(tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura->ind!=tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-1].figura->ind)
								{
									glColor3f(0.9,0.5,0.5);
									glBegin(GL_POLYGON);
											glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-1].t1.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-1].t1.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-1].t1.m_z);
											glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-1].t2.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-1].t2.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-1].t2.m_z);
											glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-1].t3.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-1].t3.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-1].t3.m_z);
											glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-1].t4.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-1].t4.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-1].t4.m_z);
									glEnd();
								}
							}

					}





				if(pocetna_pozicija_j<tabla_n-1)
				{

						if(tabla[pocetna_pozicija_i][pocetna_pozicija_j+1].figura==NULL)
						{
													glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
						glColor3f(0,0,0);
						glBegin(GL_POLYGON);
								glVertex3f(tabla[pocetna_pozicija_i][pocetna_pozicija_j+1].t1.m_x,tabla[pocetna_pozicija_i][pocetna_pozicija_j+1].t1.m_y,tabla[pocetna_pozicija_i][pocetna_pozicija_j+1].t1.m_z);
								glVertex3f(tabla[pocetna_pozicija_i][pocetna_pozicija_j+1].t2.m_x,tabla[pocetna_pozicija_i][pocetna_pozicija_j+1].t2.m_y,tabla[pocetna_pozicija_i][pocetna_pozicija_j+1].t2.m_z);
								glVertex3f(tabla[pocetna_pozicija_i][pocetna_pozicija_j+1].t3.m_x,tabla[pocetna_pozicija_i][pocetna_pozicija_j+1].t3.m_y,tabla[pocetna_pozicija_i][pocetna_pozicija_j+1].t3.m_z);
								glVertex3f(tabla[pocetna_pozicija_i][pocetna_pozicija_j+1].t4.m_x,tabla[pocetna_pozicija_i][pocetna_pozicija_j+1].t4.m_y,tabla[pocetna_pozicija_i][pocetna_pozicija_j+1].t4.m_z);
						glEnd();
						glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);	
							glColor3f(0.6,0.9,0.6);
							glBegin(GL_POLYGON);
									glVertex3f(tabla[pocetna_pozicija_i][pocetna_pozicija_j+1].t1.m_x,tabla[pocetna_pozicija_i][pocetna_pozicija_j+1].t1.m_y,tabla[pocetna_pozicija_i][pocetna_pozicija_j+1].t1.m_z);
									glVertex3f(tabla[pocetna_pozicija_i][pocetna_pozicija_j+1].t2.m_x,tabla[pocetna_pozicija_i][pocetna_pozicija_j+1].t2.m_y,tabla[pocetna_pozicija_i][pocetna_pozicija_j+1].t2.m_z);
									glVertex3f(tabla[pocetna_pozicija_i][pocetna_pozicija_j+1].t3.m_x,tabla[pocetna_pozicija_i][pocetna_pozicija_j+1].t3.m_y,tabla[pocetna_pozicija_i][pocetna_pozicija_j+1].t3.m_z);
									glVertex3f(tabla[pocetna_pozicija_i][pocetna_pozicija_j+1].t4.m_x,tabla[pocetna_pozicija_i][pocetna_pozicija_j+1].t4.m_y,tabla[pocetna_pozicija_i][pocetna_pozicija_j+1].t4.m_z);
							glEnd();
						}
						else 
							{
								if(tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura->ind!=tabla[pocetna_pozicija_i][pocetna_pozicija_j+1].figura->ind)
								{
									glColor3f(0.9,0.5,0.5);
									glBegin(GL_POLYGON);
											glVertex3f(tabla[pocetna_pozicija_i][pocetna_pozicija_j+1].t1.m_x,tabla[pocetna_pozicija_i][pocetna_pozicija_j+1].t1.m_y,tabla[pocetna_pozicija_i][pocetna_pozicija_j+1].t1.m_z);
											glVertex3f(tabla[pocetna_pozicija_i][pocetna_pozicija_j+1].t2.m_x,tabla[pocetna_pozicija_i][pocetna_pozicija_j+1].t2.m_y,tabla[pocetna_pozicija_i][pocetna_pozicija_j+1].t2.m_z);
											glVertex3f(tabla[pocetna_pozicija_i][pocetna_pozicija_j+1].t3.m_x,tabla[pocetna_pozicija_i][pocetna_pozicija_j+1].t3.m_y,tabla[pocetna_pozicija_i][pocetna_pozicija_j+1].t3.m_z);
											glVertex3f(tabla[pocetna_pozicija_i][pocetna_pozicija_j+1].t4.m_x,tabla[pocetna_pozicija_i][pocetna_pozicija_j+1].t4.m_y,tabla[pocetna_pozicija_i][pocetna_pozicija_j+1].t4.m_z);
									glEnd();
								}
							}

					}

				
				if(pocetna_pozicija_j>0)
				{

						if(tabla[pocetna_pozicija_i][pocetna_pozicija_j-1].figura==NULL)
						{
													glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
						glColor3f(0,0,0);
						glBegin(GL_POLYGON);
								glVertex3f(tabla[pocetna_pozicija_i][pocetna_pozicija_j-1].t1.m_x,tabla[pocetna_pozicija_i][pocetna_pozicija_j-1].t1.m_y,tabla[pocetna_pozicija_i][pocetna_pozicija_j-1].t1.m_z);
								glVertex3f(tabla[pocetna_pozicija_i][pocetna_pozicija_j-1].t2.m_x,tabla[pocetna_pozicija_i][pocetna_pozicija_j-1].t2.m_y,tabla[pocetna_pozicija_i][pocetna_pozicija_j-1].t2.m_z);
								glVertex3f(tabla[pocetna_pozicija_i][pocetna_pozicija_j-1].t3.m_x,tabla[pocetna_pozicija_i][pocetna_pozicija_j-1].t3.m_y,tabla[pocetna_pozicija_i][pocetna_pozicija_j-1].t3.m_z);
								glVertex3f(tabla[pocetna_pozicija_i][pocetna_pozicija_j-1].t4.m_x,tabla[pocetna_pozicija_i][pocetna_pozicija_j-1].t4.m_y,tabla[pocetna_pozicija_i][pocetna_pozicija_j-1].t4.m_z);
						glEnd();
						glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);	
							glColor3f(0.6,0.9,0.6);
							glBegin(GL_POLYGON);
									glVertex3f(tabla[pocetna_pozicija_i][pocetna_pozicija_j-1].t1.m_x,tabla[pocetna_pozicija_i][pocetna_pozicija_j-1].t1.m_y,tabla[pocetna_pozicija_i][pocetna_pozicija_j-1].t1.m_z);
									glVertex3f(tabla[pocetna_pozicija_i][pocetna_pozicija_j-1].t2.m_x,tabla[pocetna_pozicija_i][pocetna_pozicija_j-1].t2.m_y,tabla[pocetna_pozicija_i][pocetna_pozicija_j-1].t2.m_z);
									glVertex3f(tabla[pocetna_pozicija_i][pocetna_pozicija_j-1].t3.m_x,tabla[pocetna_pozicija_i][pocetna_pozicija_j-1].t3.m_y,tabla[pocetna_pozicija_i][pocetna_pozicija_j-1].t3.m_z);
									glVertex3f(tabla[pocetna_pozicija_i][pocetna_pozicija_j-1].t4.m_x,tabla[pocetna_pozicija_i][pocetna_pozicija_j-1].t4.m_y,tabla[pocetna_pozicija_i][pocetna_pozicija_j-1].t4.m_z);
							glEnd();
						}
						else 
							{
								if(tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura->ind!=tabla[pocetna_pozicija_i][pocetna_pozicija_j-1].figura->ind)
								{
									glColor3f(0.9,0.5,0.5);
									glBegin(GL_POLYGON);
											glVertex3f(tabla[pocetna_pozicija_i][pocetna_pozicija_j-1].t1.m_x,tabla[pocetna_pozicija_i][pocetna_pozicija_j-1].t1.m_y,tabla[pocetna_pozicija_i][pocetna_pozicija_j-1].t1.m_z);
											glVertex3f(tabla[pocetna_pozicija_i][pocetna_pozicija_j-1].t2.m_x,tabla[pocetna_pozicija_i][pocetna_pozicija_j-1].t2.m_y,tabla[pocetna_pozicija_i][pocetna_pozicija_j-1].t2.m_z);
											glVertex3f(tabla[pocetna_pozicija_i][pocetna_pozicija_j-1].t3.m_x,tabla[pocetna_pozicija_i][pocetna_pozicija_j-1].t3.m_y,tabla[pocetna_pozicija_i][pocetna_pozicija_j-1].t3.m_z);
											glVertex3f(tabla[pocetna_pozicija_i][pocetna_pozicija_j-1].t4.m_x,tabla[pocetna_pozicija_i][pocetna_pozicija_j-1].t4.m_y,tabla[pocetna_pozicija_i][pocetna_pozicija_j-1].t4.m_z);
									glEnd();
								}
							}

					}


				if(pocetna_pozicija_j+3<tabla_n&&(pocetna_pozicija_i==0||pocetna_pozicija_i==7)&&tabla[pocetna_pozicija_i][pocetna_pozicija_j+1].figura==NULL&&tabla[pocetna_pozicija_i][pocetna_pozicija_j+2].figura==NULL&&tabla[pocetna_pozicija_i][pocetna_pozicija_j+3].figura!=NULL)
				{
					if(strcmp(typeid(*tabla[pocetna_pozicija_i][pocetna_pozicija_j+3].figura).name(),"class Top")==0)
					{
									glColor3f(0.3,0.3,0.9);
									glBegin(GL_POLYGON);
											glVertex3f(tabla[pocetna_pozicija_i][pocetna_pozicija_j+3].t1.m_x,tabla[pocetna_pozicija_i][pocetna_pozicija_j+3].t1.m_y,tabla[pocetna_pozicija_i][pocetna_pozicija_j+3].t1.m_z);
											glVertex3f(tabla[pocetna_pozicija_i][pocetna_pozicija_j+3].t2.m_x,tabla[pocetna_pozicija_i][pocetna_pozicija_j+3].t2.m_y,tabla[pocetna_pozicija_i][pocetna_pozicija_j+3].t2.m_z);
											glVertex3f(tabla[pocetna_pozicija_i][pocetna_pozicija_j+3].t3.m_x,tabla[pocetna_pozicija_i][pocetna_pozicija_j+3].t3.m_y,tabla[pocetna_pozicija_i][pocetna_pozicija_j+3].t3.m_z);
											glVertex3f(tabla[pocetna_pozicija_i][pocetna_pozicija_j+3].t4.m_x,tabla[pocetna_pozicija_i][pocetna_pozicija_j+3].t4.m_y,tabla[pocetna_pozicija_i][pocetna_pozicija_j+3].t4.m_z);
									glEnd();
					}
				}
				if(pocetna_pozicija_j-3>-1&&(pocetna_pozicija_i==0||pocetna_pozicija_i==7)&&tabla[pocetna_pozicija_i][pocetna_pozicija_j-1].figura==NULL&&tabla[pocetna_pozicija_i][pocetna_pozicija_j-2].figura==NULL&&tabla[pocetna_pozicija_i][pocetna_pozicija_j-3].figura!=NULL)
				{
					if(strcmp(typeid(*tabla[pocetna_pozicija_i][pocetna_pozicija_j-3].figura).name(),"class Top")==0)
					{
									glColor3f(0.3,0.3,0.9);
									glBegin(GL_POLYGON);
											glVertex3f(tabla[pocetna_pozicija_i][pocetna_pozicija_j-3].t1.m_x,tabla[pocetna_pozicija_i][pocetna_pozicija_j-3].t1.m_y,tabla[pocetna_pozicija_i][pocetna_pozicija_j-3].t1.m_z);
											glVertex3f(tabla[pocetna_pozicija_i][pocetna_pozicija_j-3].t2.m_x,tabla[pocetna_pozicija_i][pocetna_pozicija_j-3].t2.m_y,tabla[pocetna_pozicija_i][pocetna_pozicija_j-3].t2.m_z);
											glVertex3f(tabla[pocetna_pozicija_i][pocetna_pozicija_j-3].t3.m_x,tabla[pocetna_pozicija_i][pocetna_pozicija_j-3].t3.m_y,tabla[pocetna_pozicija_i][pocetna_pozicija_j-3].t3.m_z);
											glVertex3f(tabla[pocetna_pozicija_i][pocetna_pozicija_j-3].t4.m_x,tabla[pocetna_pozicija_i][pocetna_pozicija_j-3].t4.m_y,tabla[pocetna_pozicija_i][pocetna_pozicija_j-3].t4.m_z);
									glEnd();
					}
				}
			}






			if(strcmp(typeid(*tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura).name(),"class Konj")==0)
			{


			//--------------------------NAPRED
				if((pocetna_pozicija_i+2<tabla_n&&pocetna_pozicija_j+1<tabla_n)&&tabla[pocetna_pozicija_i+2][pocetna_pozicija_j+1].figura==NULL)
				{

					
						glColor3f(0.6,0.9,0.6);
						glBegin(GL_POLYGON);
								glVertex3f(tabla[pocetna_pozicija_i+2][pocetna_pozicija_j+1].t1.m_x,tabla[pocetna_pozicija_i+2][pocetna_pozicija_j+1].t1.m_y,tabla[pocetna_pozicija_i+2][pocetna_pozicija_j+1].t1.m_z);
								glVertex3f(tabla[pocetna_pozicija_i+2][pocetna_pozicija_j+1].t2.m_x,tabla[pocetna_pozicija_i+2][pocetna_pozicija_j+1].t2.m_y,tabla[pocetna_pozicija_i+2][pocetna_pozicija_j+1].t2.m_z);
								glVertex3f(tabla[pocetna_pozicija_i+2][pocetna_pozicija_j+1].t3.m_x,tabla[pocetna_pozicija_i+2][pocetna_pozicija_j+1].t3.m_y,tabla[pocetna_pozicija_i+2][pocetna_pozicija_j+1].t3.m_z);
								glVertex3f(tabla[pocetna_pozicija_i+2][pocetna_pozicija_j+1].t4.m_x,tabla[pocetna_pozicija_i+2][pocetna_pozicija_j+1].t4.m_y,tabla[pocetna_pozicija_i+2][pocetna_pozicija_j+1].t4.m_z);
						glEnd();	
				}

				if((pocetna_pozicija_i+2<tabla_n&&pocetna_pozicija_j+1<tabla_n)&&tabla[pocetna_pozicija_i+2][pocetna_pozicija_j+1].figura!=NULL&&tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura->ind!=tabla[pocetna_pozicija_i+2][pocetna_pozicija_j+1].figura->ind)
				{

					
						glColor3f(0.9,0.5,0.5);
						glBegin(GL_POLYGON);
								glVertex3f(tabla[pocetna_pozicija_i+2][pocetna_pozicija_j+1].t1.m_x,tabla[pocetna_pozicija_i+2][pocetna_pozicija_j+1].t1.m_y,tabla[pocetna_pozicija_i+2][pocetna_pozicija_j+1].t1.m_z);
								glVertex3f(tabla[pocetna_pozicija_i+2][pocetna_pozicija_j+1].t2.m_x,tabla[pocetna_pozicija_i+2][pocetna_pozicija_j+1].t2.m_y,tabla[pocetna_pozicija_i+2][pocetna_pozicija_j+1].t2.m_z);
								glVertex3f(tabla[pocetna_pozicija_i+2][pocetna_pozicija_j+1].t3.m_x,tabla[pocetna_pozicija_i+2][pocetna_pozicija_j+1].t3.m_y,tabla[pocetna_pozicija_i+2][pocetna_pozicija_j+1].t3.m_z);
								glVertex3f(tabla[pocetna_pozicija_i+2][pocetna_pozicija_j+1].t4.m_x,tabla[pocetna_pozicija_i+2][pocetna_pozicija_j+1].t4.m_y,tabla[pocetna_pozicija_i+2][pocetna_pozicija_j+1].t4.m_z);
						glEnd();	
				}



				if((pocetna_pozicija_i+2<tabla_n&&pocetna_pozicija_j-1>-1)&&tabla[pocetna_pozicija_i+2][pocetna_pozicija_j-1].figura==NULL)
				{

					
						glColor3f(0.6,0.9,0.6);
						glBegin(GL_POLYGON);
								glVertex3f(tabla[pocetna_pozicija_i+2][pocetna_pozicija_j-1].t1.m_x,tabla[pocetna_pozicija_i+2][pocetna_pozicija_j-1].t1.m_y,tabla[pocetna_pozicija_i+2][pocetna_pozicija_j-1].t1.m_z);
								glVertex3f(tabla[pocetna_pozicija_i+2][pocetna_pozicija_j-1].t2.m_x,tabla[pocetna_pozicija_i+2][pocetna_pozicija_j-1].t2.m_y,tabla[pocetna_pozicija_i+2][pocetna_pozicija_j-1].t2.m_z);
								glVertex3f(tabla[pocetna_pozicija_i+2][pocetna_pozicija_j-1].t3.m_x,tabla[pocetna_pozicija_i+2][pocetna_pozicija_j-1].t3.m_y,tabla[pocetna_pozicija_i+2][pocetna_pozicija_j-1].t3.m_z);
								glVertex3f(tabla[pocetna_pozicija_i+2][pocetna_pozicija_j-1].t4.m_x,tabla[pocetna_pozicija_i+2][pocetna_pozicija_j-1].t4.m_y,tabla[pocetna_pozicija_i+2][pocetna_pozicija_j-1].t4.m_z);
						glEnd();	
				}

				if((pocetna_pozicija_i+2<tabla_n&&pocetna_pozicija_j-1>-1)&&tabla[pocetna_pozicija_i+2][pocetna_pozicija_j-1].figura!=NULL&&tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura->ind!=tabla[pocetna_pozicija_i+2][pocetna_pozicija_j-1].figura->ind)
				{

					
						glColor3f(0.9,0.5,0.5);
						glBegin(GL_POLYGON);
								glVertex3f(tabla[pocetna_pozicija_i+2][pocetna_pozicija_j-1].t1.m_x,tabla[pocetna_pozicija_i+2][pocetna_pozicija_j-1].t1.m_y,tabla[pocetna_pozicija_i+2][pocetna_pozicija_j-1].t1.m_z);
								glVertex3f(tabla[pocetna_pozicija_i+2][pocetna_pozicija_j-1].t2.m_x,tabla[pocetna_pozicija_i+2][pocetna_pozicija_j-1].t2.m_y,tabla[pocetna_pozicija_i+2][pocetna_pozicija_j-1].t2.m_z);
								glVertex3f(tabla[pocetna_pozicija_i+2][pocetna_pozicija_j-1].t3.m_x,tabla[pocetna_pozicija_i+2][pocetna_pozicija_j-1].t3.m_y,tabla[pocetna_pozicija_i+2][pocetna_pozicija_j-1].t3.m_z);
								glVertex3f(tabla[pocetna_pozicija_i+2][pocetna_pozicija_j-1].t4.m_x,tabla[pocetna_pozicija_i+2][pocetna_pozicija_j-1].t4.m_y,tabla[pocetna_pozicija_i+2][pocetna_pozicija_j-1].t4.m_z);
						glEnd();		
				}



				//-----------------------------------Nazad

				if((pocetna_pozicija_i-2>-1&&pocetna_pozicija_j+1<tabla_n)&&tabla[pocetna_pozicija_i-2][pocetna_pozicija_j+1].figura==NULL)
				{

					
						glColor3f(0.6,0.9,0.6);
						glBegin(GL_POLYGON);
								glVertex3f(tabla[pocetna_pozicija_i-2][pocetna_pozicija_j+1].t1.m_x,tabla[pocetna_pozicija_i-2][pocetna_pozicija_j+1].t1.m_y,tabla[pocetna_pozicija_i-2][pocetna_pozicija_j+1].t1.m_z);
								glVertex3f(tabla[pocetna_pozicija_i-2][pocetna_pozicija_j+1].t2.m_x,tabla[pocetna_pozicija_i-2][pocetna_pozicija_j+1].t2.m_y,tabla[pocetna_pozicija_i-2][pocetna_pozicija_j+1].t2.m_z);
								glVertex3f(tabla[pocetna_pozicija_i-2][pocetna_pozicija_j+1].t3.m_x,tabla[pocetna_pozicija_i-2][pocetna_pozicija_j+1].t3.m_y,tabla[pocetna_pozicija_i-2][pocetna_pozicija_j+1].t3.m_z);
								glVertex3f(tabla[pocetna_pozicija_i-2][pocetna_pozicija_j+1].t4.m_x,tabla[pocetna_pozicija_i-2][pocetna_pozicija_j+1].t4.m_y,tabla[pocetna_pozicija_i-2][pocetna_pozicija_j+1].t4.m_z);
						glEnd();	
				}

				if((pocetna_pozicija_i-2>-1&&pocetna_pozicija_j+1<tabla_n)&&tabla[pocetna_pozicija_i-2][pocetna_pozicija_j+1].figura!=NULL&&tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura->ind!=tabla[pocetna_pozicija_i-2][pocetna_pozicija_j+1].figura->ind)
				{

					
						glColor3f(0.9,0.5,0.5);
						glBegin(GL_POLYGON);
								glVertex3f(tabla[pocetna_pozicija_i-2][pocetna_pozicija_j+1].t1.m_x,tabla[pocetna_pozicija_i-2][pocetna_pozicija_j+1].t1.m_y,tabla[pocetna_pozicija_i-2][pocetna_pozicija_j+1].t1.m_z);
								glVertex3f(tabla[pocetna_pozicija_i-2][pocetna_pozicija_j+1].t2.m_x,tabla[pocetna_pozicija_i-2][pocetna_pozicija_j+1].t2.m_y,tabla[pocetna_pozicija_i-2][pocetna_pozicija_j+1].t2.m_z);
								glVertex3f(tabla[pocetna_pozicija_i-2][pocetna_pozicija_j+1].t3.m_x,tabla[pocetna_pozicija_i-2][pocetna_pozicija_j+1].t3.m_y,tabla[pocetna_pozicija_i-2][pocetna_pozicija_j+1].t3.m_z);
								glVertex3f(tabla[pocetna_pozicija_i-2][pocetna_pozicija_j+1].t4.m_x,tabla[pocetna_pozicija_i-2][pocetna_pozicija_j+1].t4.m_y,tabla[pocetna_pozicija_i-2][pocetna_pozicija_j+1].t4.m_z);
						glEnd();	
				}



				if((pocetna_pozicija_i-2>-1&&pocetna_pozicija_j-1>-1)&&tabla[pocetna_pozicija_i-2][pocetna_pozicija_j-1].figura==NULL)
				{

					
						glColor3f(0.6,0.9,0.6);
						glBegin(GL_POLYGON);
								glVertex3f(tabla[pocetna_pozicija_i-2][pocetna_pozicija_j-1].t1.m_x,tabla[pocetna_pozicija_i-2][pocetna_pozicija_j-1].t1.m_y,tabla[pocetna_pozicija_i-2][pocetna_pozicija_j-1].t1.m_z);
								glVertex3f(tabla[pocetna_pozicija_i-2][pocetna_pozicija_j-1].t2.m_x,tabla[pocetna_pozicija_i-2][pocetna_pozicija_j-1].t2.m_y,tabla[pocetna_pozicija_i-2][pocetna_pozicija_j-1].t2.m_z);
								glVertex3f(tabla[pocetna_pozicija_i-2][pocetna_pozicija_j-1].t3.m_x,tabla[pocetna_pozicija_i-2][pocetna_pozicija_j-1].t3.m_y,tabla[pocetna_pozicija_i-2][pocetna_pozicija_j-1].t3.m_z);
								glVertex3f(tabla[pocetna_pozicija_i-2][pocetna_pozicija_j-1].t4.m_x,tabla[pocetna_pozicija_i-2][pocetna_pozicija_j-1].t4.m_y,tabla[pocetna_pozicija_i-2][pocetna_pozicija_j-1].t4.m_z);
						glEnd();	
				}

				if((pocetna_pozicija_i-2>-1&&pocetna_pozicija_j-1>-1)&&tabla[pocetna_pozicija_i-2][pocetna_pozicija_j-1].figura!=NULL&&tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura->ind!=tabla[pocetna_pozicija_i-2][pocetna_pozicija_j-1].figura->ind)
				{

					
						glColor3f(0.9,0.5,0.5);
						glBegin(GL_POLYGON);
								glVertex3f(tabla[pocetna_pozicija_i-2][pocetna_pozicija_j-1].t1.m_x,tabla[pocetna_pozicija_i-2][pocetna_pozicija_j-1].t1.m_y,tabla[pocetna_pozicija_i-2][pocetna_pozicija_j-1].t1.m_z);
								glVertex3f(tabla[pocetna_pozicija_i-2][pocetna_pozicija_j-1].t2.m_x,tabla[pocetna_pozicija_i-2][pocetna_pozicija_j-1].t2.m_y,tabla[pocetna_pozicija_i-2][pocetna_pozicija_j-1].t2.m_z);
								glVertex3f(tabla[pocetna_pozicija_i-2][pocetna_pozicija_j-1].t3.m_x,tabla[pocetna_pozicija_i-2][pocetna_pozicija_j-1].t3.m_y,tabla[pocetna_pozicija_i-2][pocetna_pozicija_j-1].t3.m_z);
								glVertex3f(tabla[pocetna_pozicija_i-2][pocetna_pozicija_j-1].t4.m_x,tabla[pocetna_pozicija_i-2][pocetna_pozicija_j-1].t4.m_y,tabla[pocetna_pozicija_i-2][pocetna_pozicija_j-1].t4.m_z);
						glEnd();			
				}


				//-----------------------------------------DESNO

				if((pocetna_pozicija_i+1<tabla_n&&pocetna_pozicija_j+2<tabla_n)&&tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+2].figura==NULL)
				{

					
						glColor3f(0.6,0.9,0.6);
						glBegin(GL_POLYGON);
								glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+2].t1.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+2].t1.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+2].t1.m_z);
								glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+2].t2.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+2].t2.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+2].t2.m_z);
								glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+2].t3.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+2].t3.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+2].t3.m_z);
								glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+2].t4.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+2].t4.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+2].t4.m_z);
						glEnd();	
				}

				if((pocetna_pozicija_i+1<tabla_n&&pocetna_pozicija_j+2<tabla_n)&&tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+2].figura!=NULL&&tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura->ind!=tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+2].figura->ind)
				{

					
						glColor3f(0.9,0.5,0.5);
						glBegin(GL_POLYGON);
								glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+2].t1.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+2].t1.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+2].t1.m_z);
								glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+2].t2.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+2].t2.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+2].t2.m_z);
								glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+2].t3.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+2].t3.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+2].t3.m_z);
								glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+2].t4.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+2].t4.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j+2].t4.m_z);
						glEnd();	
				}



				if((pocetna_pozicija_i-1>-1&&pocetna_pozicija_j+2<tabla_n)&&tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+2].figura==NULL)
				{

					
						glColor3f(0.6,0.9,0.6);
						glBegin(GL_POLYGON);
								glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+2].t1.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+2].t1.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+2].t1.m_z);
								glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+2].t2.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+2].t2.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+2].t2.m_z);
								glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+2].t3.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+2].t3.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+2].t3.m_z);
								glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+2].t4.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+2].t4.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+2].t4.m_z);
						glEnd();	
				}

				if((pocetna_pozicija_i-1>-1&&pocetna_pozicija_j+2<tabla_n)&&tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+2].figura!=NULL&&tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura->ind!=tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+2].figura->ind)
				{

					
						glColor3f(0.9,0.5,0.5);
						glBegin(GL_POLYGON);
								glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+2].t1.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+2].t1.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+2].t1.m_z);
								glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+2].t2.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+2].t2.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+2].t2.m_z);
								glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+2].t3.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+2].t3.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+2].t3.m_z);
								glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+2].t4.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+2].t4.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j+2].t4.m_z);
						glEnd();
				}

			//-----------------------------------------------------LEVO
				if((pocetna_pozicija_i+1<tabla_n&&pocetna_pozicija_j-2>-1)&&tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-2].figura==NULL)
				{

					
						glColor3f(0.6,0.9,0.6);
						glBegin(GL_POLYGON);
								glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-2].t1.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-2].t1.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-2].t1.m_z);
								glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-2].t2.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-2].t2.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-2].t2.m_z);
								glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-2].t3.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-2].t3.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-2].t3.m_z);
								glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-2].t4.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-2].t4.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-2].t4.m_z);
						glEnd();	
				}

				if((pocetna_pozicija_i+1<tabla_n&&pocetna_pozicija_j-2>-1)&&tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-2].figura!=NULL&&tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura->ind!=tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-2].figura->ind)
				{

					
						glColor3f(0.9,0.5,0.5);
						glBegin(GL_POLYGON);
								glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-2].t1.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-2].t1.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-2].t1.m_z);
								glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-2].t2.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-2].t2.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-2].t2.m_z);
								glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-2].t3.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-2].t3.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-2].t3.m_z);
								glVertex3f(tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-2].t4.m_x,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-2].t4.m_y,tabla[pocetna_pozicija_i+1][pocetna_pozicija_j-2].t4.m_z);
						glEnd();	
				}



				if((pocetna_pozicija_i-1>-1&&pocetna_pozicija_j-2>-1)&&tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-2].figura==NULL)
				{

					
						glColor3f(0.6,0.9,0.6);
						glBegin(GL_POLYGON);
								glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-2].t1.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-2].t1.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-2].t1.m_z);
								glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-2].t2.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-2].t2.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-2].t2.m_z);
								glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-2].t3.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-2].t3.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-2].t3.m_z);
								glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-2].t4.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-2].t4.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-2].t4.m_z);
						glEnd();	
				}

				if((pocetna_pozicija_i-1>-1&&pocetna_pozicija_j-2>-1)&&tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-2].figura!=NULL&&tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura->ind!=tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-2].figura->ind)
				{

					
						glColor3f(0.9,0.5,0.5);
						glBegin(GL_POLYGON);
								glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-2].t1.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-2].t1.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-2].t1.m_z);
								glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-2].t2.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-2].t2.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-2].t2.m_z);
								glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-2].t3.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-2].t3.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-2].t3.m_z);
								glVertex3f(tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-2].t4.m_x,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-2].t4.m_y,tabla[pocetna_pozicija_i-1][pocetna_pozicija_j-2].t4.m_z);
						glEnd();	
				}

			}
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glColor3f(0, 1, 0);
			glBegin(GL_POLYGON);
			glVertex3f(tabla[pocetna_pozicija_i][pocetna_pozicija_j].t1.m_x, tabla[pocetna_pozicija_i][pocetna_pozicija_j].t1.m_y, tabla[pocetna_pozicija_i][pocetna_pozicija_j].t1.m_z);
			glVertex3f(tabla[pocetna_pozicija_i][pocetna_pozicija_j].t2.m_x, tabla[pocetna_pozicija_i][pocetna_pozicija_j].t2.m_y, tabla[pocetna_pozicija_i][pocetna_pozicija_j].t2.m_z);
			glVertex3f(tabla[pocetna_pozicija_i][pocetna_pozicija_j].t3.m_x, tabla[pocetna_pozicija_i][pocetna_pozicija_j].t3.m_y, tabla[pocetna_pozicija_i][pocetna_pozicija_j].t3.m_z);
			glVertex3f(tabla[pocetna_pozicija_i][pocetna_pozicija_j].t4.m_x, tabla[pocetna_pozicija_i][pocetna_pozicija_j].t4.m_y, tabla[pocetna_pozicija_i][pocetna_pozicija_j].t4.m_z);
			glEnd();

}
void Engine::drawIspis()
{

	/*	glColor3f(0.7,0.7,0.7);
	glBegin(GL_POLYGON);
	glVertex3f(0.65,1,0);
	glVertex3f(1,1,0);
	glVertex3f(1,0,0);
	glVertex3f(0.65,0,0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(-0.65,1,0);
	glVertex3f(-1,1,0);
	glVertex3f(-1,0,0);
	glVertex3f(-0.65,0,0);
	glEnd();

	
		glColor3f(0,0,0);
	char s[50];
	
	if(igrac_igra==1)
	{
		sprintf_s(s,"Igrac 1\n\n\n Beli\n Igraj");
	}else sprintf_s(s,"Igrac 1\n\n\n Beli\n Cekaj");
	glRasterPos2f(-0.9,0.8);
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,(const unsigned char*) s);

	if(igrac_igra==0)
	{
		sprintf_s(s,"Igrac 2\n\n\n Crni\n Igraj");
	}else sprintf_s(s,"Igrac 2\n\n\n Crni\n Cekaj");
	glRasterPos2f(0.75,0.8);
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,(const unsigned char*) s);

	if(beli_sah>0)
	{
		glColor3f(1,0,0);
		sprintf_s(s,"Sah");
		glRasterPos2f(-0.9,0.1);
		glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,(const unsigned char*) s);
	}


	if(crni_sah>0)
	{
		glColor3f(1,0,0);
		sprintf_s(s,"Sah");
		glRasterPos2f(0.8,0.1);
		glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,(const unsigned char*) s);
	}*/


	glColor3f(0.7,0.7,0.7);
	glBegin(GL_POLYGON);
	glVertex3f(-1,-0.85,0);
	glVertex3f(1,-0.85,0);
	glVertex3f(1,-1,0);
	glVertex3f(-1,-1,0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(-1,0.85,0);
	glVertex3f(1,0.85,0);
	glVertex3f(1,1,0);
	glVertex3f(-1,1,0);
	glEnd();

	
		glColor3f(0,0,0);
	char s[50];
	
	if(igrac_igra==1)
	{
		sprintf_s(s,"		Igrac 1\t\t\t Beli\t          Na potezu");
	}else sprintf_s(s,"		Igrac 1\t\t\t Beli\t          Cekaj");
	glRasterPos2f(-0.9,-0.95);
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,(const unsigned char*) s);

	if(igrac_igra==0)
	{
		sprintf_s(s,"		Igrac 2\t\t\t Crni\t         Na potezu");
	}else sprintf_s(s,"		Igrac 2\t\t\t Crni\t         Cekaj");
	glRasterPos2f(-0.9,0.9);
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,(const unsigned char*) s);

	if(beli_sah>0)
	{
		glColor3f(1,0,0);
		sprintf_s(s,"Sah");
		glRasterPos2f(0.6,-0.95);
		glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,(const unsigned char*) s);
	}


	if(crni_sah>0)
	{
		glColor3f(1,0,0);
		sprintf_s(s,"Sah");
		glRasterPos2f(0.6,0.9);
		glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,(const unsigned char*) s);
	}


}
void Engine::proveriSah()
{
		glColor3f(1,0,0);
	for(int i=0;i<tabla_n;i++)
		for(int j=0;j<tabla_n;j++)
			{
				if(tabla[i][j].figura!=NULL&&strcmp(typeid(*tabla[i][j].figura).name(),"class Kralj")==0)
				{
					int sah=0;
					//-GORE
					for(int k=i+1;k<tabla_n;k++)
						if(tabla[k][j].figura!=NULL)
								{
									if(tabla[k][j].figura->ind!=tabla[i][j].figura->ind)
									{
										if(strcmp(typeid(*tabla[k][j].figura).name(),"class Kralj")==0&&abs(k-i)==1)sah++;
										if(strcmp(typeid(*tabla[k][j].figura).name(),"class Kraljica")==0)sah++;
										if(strcmp(typeid(*tabla[k][j].figura).name(),"class Top")==0)sah++;
									}
										
									break;
								}
					//-DOLE
					for(int k=i-1;k>-1;k--)
						if(tabla[k][j].figura!=NULL)
								{
									if(tabla[k][j].figura->ind!=tabla[i][j].figura->ind)
									{
										if(strcmp(typeid(*tabla[k][j].figura).name(),"class Kralj")==0&&abs(k-i)==1)sah++;
										if(strcmp(typeid(*tabla[k][j].figura).name(),"class Kraljica")==0)sah++;
										if(strcmp(typeid(*tabla[k][j].figura).name(),"class Top")==0)sah++;
									}
									break;
								}
					//-DESNO
					for(int k=j+1;k<tabla_n;k++)
						if(tabla[i][k].figura!=NULL)
								{
									if(tabla[i][k].figura->ind!=tabla[i][j].figura->ind)
									{
										if(strcmp(typeid(*tabla[i][k].figura).name(),"class Kralj")==0&&abs(k-i)==1)sah++;
										if(strcmp(typeid(*tabla[i][k].figura).name(),"class Kraljica")==0)sah++;
										if(strcmp(typeid(*tabla[i][k].figura).name(),"class Top")==0)sah++;
									}
										
									break;
								}
					//-LEVO
					for(int k=j-1;k>-1;k--)
						if(tabla[i][k].figura!=NULL)
								{
									if(tabla[i][k].figura->ind!=tabla[i][j].figura->ind)
									{
										if(strcmp(typeid(*tabla[i][k].figura).name(),"class Kralj")==0&&abs(k-i)==1)sah++;
										if(strcmp(typeid(*tabla[i][k].figura).name(),"class Kraljica")==0)sah++;
										if(strcmp(typeid(*tabla[i][k].figura).name(),"class Top")==0)sah++;
									}
									break;
								}


					//GORE-DESNO
					int k=i+1;
					int l=j+1;
					while(k<tabla_n&&l<tabla_n)
					{
						if(tabla[k][l].figura!=NULL)
								{
									if(tabla[k][l].figura->ind!=tabla[i][j].figura->ind)
									{
										if(strcmp(typeid(*tabla[k][l].figura).name(),"class Pesak")==0&&abs(k-i)==1&&abs(l-j)==1&&tabla[k][l].figura->ind==0)sah++;
										if(strcmp(typeid(*tabla[k][l].figura).name(),"class Kraljica")==0)sah++;
										if(strcmp(typeid(*tabla[k][l].figura).name(),"class Lovac")==0)sah++;
									}
									break;
								}
						k++;
						l++;
					}

					//GORE-Levo
					k=i+1;
					l=j-1;
					while(k<tabla_n&&l>-1)
					{
						if(tabla[k][l].figura!=NULL)
								{
									if(tabla[k][l].figura->ind!=tabla[i][j].figura->ind)
									{
										if(strcmp(typeid(*tabla[k][l].figura).name(),"class Pesak")==0&&abs(k-i)==1&&abs(l-j)==1&&tabla[k][l].figura->ind==0)sah++;
										if(strcmp(typeid(*tabla[k][l].figura).name(),"class Kraljica")==0)sah++;
										if(strcmp(typeid(*tabla[k][l].figura).name(),"class Lovac")==0)sah++;
									}
									break;
								}
						k++;
						l--;
					}



					
					//DOLE-Levo
					 k=i-1;
					 l=j-1;
					while(k>-1&&l>-1)
					{
						if(tabla[k][l].figura!=NULL)
								{
									if(tabla[k][l].figura->ind!=tabla[i][j].figura->ind)
									{
										if(strcmp(typeid(*tabla[k][l].figura).name(),"class Pesak")==0&&abs(k-i)==1&&abs(l-j)==1&&tabla[k][l].figura->ind==1)sah++;
										if(strcmp(typeid(*tabla[k][l].figura).name(),"class Kraljica")==0)sah++;
										if(strcmp(typeid(*tabla[k][l].figura).name(),"class Lovac")==0)sah++;
									}
									break;
								}
						k--;
						l--;
					}

					//Dole-Desno
					k=i-1;
					l=j+1;
					while(k>-1&&l<tabla_n)
					{
						if(tabla[k][l].figura!=NULL)
								{
									if(tabla[k][l].figura->ind!=tabla[i][j].figura->ind)
									{
										if(strcmp(typeid(*tabla[k][l].figura).name(),"class Pesak")==0&&abs(k-i)==1&&abs(l-j)==1&&tabla[k][l].figura->ind==1)sah++;
										if(strcmp(typeid(*tabla[k][l].figura).name(),"class Kraljica")==0)sah++;
										if(strcmp(typeid(*tabla[k][l].figura).name(),"class Lovac")==0)sah++;
									}
									break;
								}
						k--;
						l++;
					}


					//Konj-Gore-Dole
					k=i+2;
					l=j+1;
					if(k<tabla_n&&l<tabla_n&&tabla[k][l].figura!=NULL&&tabla[k][l].figura->ind!=tabla[i][j].figura->ind&&strcmp(typeid(*tabla[k][l].figura).name(),"class Konj")==0)sah++;
					k=i+2;
					l=j-1;
					if(k<tabla_n&&l>-1&&tabla[k][l].figura!=NULL&&tabla[k][l].figura->ind!=tabla[i][j].figura->ind&&strcmp(typeid(*tabla[k][l].figura).name(),"class Konj")==0)sah++;
					k=i-2;
					l=j-1;
					if(k>-1&&l>-1&&tabla[k][l].figura!=NULL&&tabla[k][l].figura->ind!=tabla[i][j].figura->ind&&strcmp(typeid(*tabla[k][l].figura).name(),"class Konj")==0)sah++;
					k=i-2;
					l=j+1;
					if(k>-1&&l<tabla_n&&tabla[k][l].figura!=NULL&&tabla[k][l].figura->ind!=tabla[i][j].figura->ind&&strcmp(typeid(*tabla[k][l].figura).name(),"class Konj")==0)sah++;

					//Konj-Levo-Desno
					k=i+1;
					l=j+2;
					if(k<tabla_n&&l<tabla_n&&tabla[k][l].figura!=NULL&&tabla[k][l].figura->ind!=tabla[i][j].figura->ind&&strcmp(typeid(*tabla[k][l].figura).name(),"class Konj")==0)sah++;
					k=i-1;
					l=j+2;
					if(k>-1&&l<tabla_n&&tabla[k][l].figura!=NULL&&tabla[k][l].figura->ind!=tabla[i][j].figura->ind&&strcmp(typeid(*tabla[k][l].figura).name(),"class Konj")==0)sah++;
					k=i+1;
					l=j-2;
					if(k<tabla_n&&l>-1&&tabla[k][l].figura!=NULL&&tabla[k][l].figura->ind!=tabla[i][j].figura->ind&&strcmp(typeid(*tabla[k][l].figura).name(),"class Konj")==0)sah++;
					k=i-1;
					l=j-2;
					if(k>-1&&l>-1&&tabla[k][l].figura!=NULL&&tabla[k][l].figura->ind!=tabla[i][j].figura->ind&&strcmp(typeid(*tabla[k][l].figura).name(),"class Konj")==0)sah++;

					if(sah>0)
					{
						if(tabla[i][j].figura->ind==1)beli_sah++;
						else crni_sah++;
						if(beli_sah==2)
						{
							igra_u_toku=1;
							cout<<"Pobedio IGRAC 1-Crni\n"<<endl;
						}
						if(crni_sah==2)
						{
							igra_u_toku=1;
							cout<<"Pobedio IGRAC 2-Beli\n"<<endl;
						}


					}else
					{
						if(tabla[i][j].figura->ind==1)beli_sah=0;
						else crni_sah=0;
					}

				}
			}
}
void Engine::PomeriPesak(int i, int j, int jedi_j, int k)
{
	Vector3D pomeraj;


	Figura *pom1;
	Figura *pom2;
	pom1 = tabla[i][j].figura;

	pom2 = tabla[k][jedi_j].figura;
	if (tabla[i][j].figura->ind == 0)
	{


		if (abs(k - i) == 1)
		{

			if (tabla[i - 1][j].figura == NULL && j == jedi_j && k < i)
			{
				pomeraj.Set(0, 0, 1);
				tabla[i][j].figura->Move(pomeraj);
				tabla[i - 1][j].figura = tabla[i][j].figura;
				tabla[i][j].figura = NULL;
				if (i - 1 == 0)
				{
					Vector3D d1((tabla[i - 1][jedi_j].t1.m_x + tabla[i - 1][jedi_j].t2.m_x) / 2, 0, (tabla[i - 1][jedi_j].t1.m_z + tabla[i - 1][j].t4.m_z) / 2);
					tabla[i - 1][jedi_j].figura = new Kraljica(20, 0.5, 0.4, 0.3, 0.2, 0.3, 0.6, d1, 0);
				}
			}
			if (j != jedi_j && tabla[i - 1][jedi_j].figura != NULL && k < i&&tabla[i - 1][jedi_j].figura->ind == 1)
			{
				if (jedi_j > j)pomeraj.Set(1, 0, 1);
				else pomeraj.Set(-1, 0, 1);
				tabla[i][j].figura->Move(pomeraj);
				tabla[i - 1][jedi_j].figura = tabla[i][j].figura;
				tabla[i][j].figura = NULL;
				if (i - 1 == 0)
				{
					Vector3D d1((tabla[i - 1][jedi_j].t1.m_x + tabla[i - 1][jedi_j].t2.m_x) / 2, 0, (tabla[i - 1][jedi_j].t1.m_z + tabla[i - 1][j].t4.m_z) / 2);
					tabla[i - 1][jedi_j].figura = new Kraljica(20, 0.5, 0.4, 0.3, 0.2, 0.3, 0.6, d1, 0);
				}

			}


		}
		else if (tabla[i - 1][j].figura == NULL && jedi_j == j && tabla[k][j].figura == NULL && k < i&&i == 6)
		{
			pomeraj.Set(0, 0, i - k);
			tabla[i][j].figura->Move(pomeraj);
			tabla[k][j].figura = tabla[i][j].figura;
			tabla[i][j].figura = NULL;
		}

	}
	else
	{

		if (abs(k - i) == 1)
		{
			if (tabla[i + 1][j].figura == NULL && j == jedi_j && k > i)
			{
				pomeraj.Set(0, 0, -1);
				tabla[i][j].figura->Move(pomeraj);
				tabla[i + 1][j].figura = tabla[i][j].figura;
				tabla[i][j].figura = NULL;
				if (i + 1 == tabla_n - 1)
				{
					Vector3D d1((tabla[i + 1][jedi_j].t1.m_x + tabla[i + 1][jedi_j].t2.m_x) / 2, 0, (tabla[i + 1][jedi_j].t1.m_z + tabla[i + 1][j].t4.m_z) / 2);
					tabla[i + 1][jedi_j].figura = new Kraljica(20, 0.5, 0.4, 0.3, 0.2, 0.3, 0.6, d1, 1);
				}
			}

			if (j != jedi_j && tabla[i + 1][jedi_j].figura != NULL && k > i&&tabla[i + 1][jedi_j].figura->ind == 0)
			{
				if (jedi_j > j)pomeraj.Set(1, 0, -1);
				else pomeraj.Set(-1, 0, -1);
				tabla[i][j].figura->Move(pomeraj);
				tabla[i + 1][jedi_j].figura = tabla[i][j].figura;
				tabla[i][j].figura = NULL;
				if (i + 1 == tabla_n - 1)
				{
					Vector3D d1((tabla[i + 1][jedi_j].t1.m_x + tabla[i + 1][jedi_j].t2.m_x) / 2, 0, (tabla[i + 1][jedi_j].t1.m_z + tabla[i + 1][j].t4.m_z) / 2);
					tabla[i + 1][jedi_j].figura = new Kraljica(20, 0.5, 0.4, 0.3, 0.2, 0.3, 0.6, d1, 1);
				}


			}

			if (tabla[i + 1][j].figura == NULL && jedi_j == j && tabla[k][j].figura == NULL && beli_broj_poteza == 0)
			{
				pomeraj.Set(0, 0, k - j);
				tabla[i][j].figura->Move(pomeraj);
				tabla[k][j].figura = tabla[i][j].figura;
				tabla[i][j].figura = NULL;
			}

		}
		else if (tabla[i + 1][j].figura == NULL && jedi_j == j && tabla[k][j].figura == NULL && k > i&&i == 1)
		{
			pomeraj.Set(0, 0, i - k);
			tabla[i][j].figura->Move(pomeraj);
			tabla[k][j].figura = tabla[i][j].figura;
			tabla[i][j].figura = NULL;
		}
	}




	if (tabla[i][j].figura == NULL)
	{
		proveriSah();
		if (pom1->ind == 0 && crni_sah == 1)
		{
			pomeraj = pomeraj * (-1);
			tabla[k][jedi_j].figura->Move(pomeraj);
			crni_sah = 0;
			tabla[i][j].figura = pom1;
			tabla[k][jedi_j].figura = pom2;
		}
		if (pom1->ind == 1 && beli_sah == 1)
		{
			pomeraj = pomeraj * (-1);
			tabla[k][jedi_j].figura->Move(pomeraj);
			beli_sah = 0;
			tabla[i][j].figura = pom1;
			tabla[k][jedi_j].figura = pom2;
		}

	}
}
void Engine::PomeriTop(int i,int j,int k_i,int k_j)
{
								            Vector3D pomeraj;
																								
											Figura *pom1;
											Figura *pom2;
											pom1=tabla[i][j].figura;

											pom2=tabla[k_i][k_j].figura;
										if(i==k_i)
										{
											int test=0;
											if(k_j>j)for(int k=j+1;k<=k_j;k++)if(tabla[i][k].figura!=NULL){test++;}
											if(k_j<j)for(int k=j-1;k>=k_j;k--)if(tabla[i][k].figura!=NULL){test++;}
											if(tabla[k_i][k_j].figura!=NULL&&test==1&&tabla[i][j].figura->ind!=tabla[k_i][k_j].figura->ind)test=0;
											if(test==0)
											{
												pomeraj.Set(k_j-j,0,0);
												tabla[i][j].figura->Move(pomeraj);
												tabla[i][k_j].figura=tabla[i][j].figura;
												tabla[i][j].figura=NULL;
											}
										}
										
										if(j==k_j)
										{
											int test=0;
											if(k_i>i)for(int k=i+1;k<=k_i;k++)if(tabla[k][j].figura!=NULL){test++;}
											if(k_i<i)for(int k=i-1;k>=k_i;k--)if(tabla[k][j].figura!=NULL){test++;}
											if(tabla[k_i][k_j].figura!=NULL&&test==1&&tabla[i][j].figura->ind!=tabla[k_i][k_j].figura->ind)test=0;
											if(test==0)
											{
												pomeraj.Set(0,0,-(k_i-i));
												tabla[i][j].figura->Move(pomeraj);
												tabla[k_i][j].figura=tabla[i][j].figura;
												tabla[i][j].figura=NULL;
											}
										}	


										if (tabla[i][j].figura == NULL)
										{
											proveriSah();
											if (pom1->ind == 0 && crni_sah == 1)
											{
												pomeraj = pomeraj * (-1);
												tabla[k_i][k_j].figura->Move(pomeraj);
												crni_sah = 0;
												tabla[i][j].figura = pom1;
												tabla[k_i][k_j].figura = pom2;
											}
											if (pom1->ind == 1 && beli_sah == 1)
											{
												pomeraj = pomeraj * (-1);
												tabla[k_i][k_j].figura->Move(pomeraj);
												beli_sah = 0;
												tabla[i][j].figura = pom1;
												tabla[k_i][k_j].figura = pom2;
											}
										}

}
void Engine::PomeriLovac(int i,int j, int k_i,int k_j)
{
		Vector3D pomeraj;
																										
											Figura *pom1;
											Figura *pom2;
											pom1=tabla[i][j].figura;

											pom2=tabla[k_i][k_j].figura;

			
			int test=0;
			int k,g;
			k=i+1;
			g=j+1;
			if(i<k_i&&j<k_j)  while(k<=k_i&&g<=k_j)
						{
							if(tabla[k][g].figura!=NULL){test++;}
							k++;
							g++;
						}


			k=i-1;
			g=j+1;
			if(i>k_i&&j<k_j)  while(k>=k_i&&g<=k_j)
						{
							if(tabla[k][g].figura!=NULL){test++;}
							k--;
							g++;
						}
			k=i-1;
			g=j-1;
			if(i>k_i&&j>k_j) while(k>=k_i&&g>=k_j)
						{
							if(tabla[k][g].figura!=NULL){test++;}
							k--;
							g--;
						}
			k=i+1;
			g=j-1;
			if(i<k_i&&j>k_j) while(k<=k_i&&g>=k_j)
						{
							if(tabla[k][g].figura!=NULL){test++;}
							k++;
							g--;
						}
			
			if(tabla[k_i][k_j].figura!=NULL&&test==1&&tabla[i][j].figura->ind!=tabla[k_i][k_j].figura->ind)test=0;

			if(test==0)
		     {

				if((i<k_i&&j<k_j)||(i>k_i&&j>k_j))pomeraj.Set((k_i-i),0,-(k_j-j));
				else pomeraj.Set(-(k_i-i),0,(k_j-j));
				tabla[i][j].figura->Move(pomeraj);
				tabla[k_i][k_j].figura=tabla[i][j].figura;
				tabla[i][j].figura=NULL;
			}
			if (tabla[i][j].figura == NULL)
			{
				proveriSah();
				if (pom1->ind == 0 && crni_sah == 1)
				{
					pomeraj = pomeraj * (-1);
					tabla[k_i][k_j].figura->Move(pomeraj);
					crni_sah = 0;
					tabla[i][j].figura = pom1;
					tabla[k_i][k_j].figura = pom2;
				}
				if (pom1->ind == 1 && beli_sah == 1)
				{
					pomeraj = pomeraj * (-1);
					tabla[k_i][k_j].figura->Move(pomeraj);
					beli_sah = 0;
					tabla[i][j].figura = pom1;
					tabla[k_i][k_j].figura = pom2;
				}
			}
		
}
void Engine::PomeriKralja_i_Kraljicu(int i,int j,int k_i,int k_j)
{
		Vector3D pomeraj;

																								
											Figura *pom1;
											Figura *pom2;
											pom1=tabla[i][j].figura;

											pom2=tabla[k_i][k_j].figura;
			

			if(i!=k_i&&j!=k_j)
			{
				int test=0;
				int k,g;
				k=i+1;
				g=j+1;
				if(i<k_i&&j<k_j)  while(k<=k_i&&g<=k_j)
							{
								if(tabla[k][g].figura!=NULL){test++;}
								k++;
								g++;
							}


				k=i-1;
				g=j+1;
				if(i>k_i&&j<k_j)  while(k>=k_i&&g<=k_j)
							{
								if(tabla[k][g].figura!=NULL){test++;}
								k--;
								g++;
							}
				k=i-1;
				g=j-1;
				if(i>k_i&&j>k_j) while(k>=k_i&&g>=k_j)
							{
								if(tabla[k][g].figura!=NULL){test++;}
								k--;
								g--;
							}
				k=i+1;
				g=j-1;
				if(i<k_i&&j>k_j) while(k<=k_i&&g>=k_j)
							{
								if(tabla[k][g].figura!=NULL){test++;}
								k++;
								g--;
							}
			
				if(tabla[k_i][k_j].figura!=NULL&&test==1&&tabla[i][j].figura->ind!=tabla[k_i][k_j].figura->ind)test=0;

				if(test==0)
				 {

					if((i<k_i&&j<k_j)||(i>k_i&&j>k_j))pomeraj.Set((k_i-i),0,-(k_j-j));
					else pomeraj.Set(-(k_i-i),0,(k_j-j));
					tabla[i][j].figura->Move(pomeraj);
					tabla[k_i][k_j].figura=tabla[i][j].figura;
					tabla[i][j].figura=NULL;
				}
			}
			else
			{
										if(i==k_i)
										{
											int test=0;
											if(k_j>j)for(int k=j+1;k<=k_j;k++)if(tabla[i][k].figura!=NULL){test++;}
											if(k_j<j)for(int k=j-1;k>=k_j;k--)if(tabla[i][k].figura!=NULL){test++;}
											if(tabla[k_i][k_j].figura!=NULL&&test==1&&tabla[i][j].figura->ind!=tabla[k_i][k_j].figura->ind)test=0;
											if(test==0)
											{
												pomeraj.Set(k_j-j,0,0);
												tabla[i][j].figura->Move(pomeraj);
												tabla[i][k_j].figura=tabla[i][j].figura;
												tabla[i][j].figura=NULL;
											}
										}
										
										if(j==k_j)
										{
											int test=0;
											if(k_i>i)for(int k=i+1;k<=k_i;k++)if(tabla[k][j].figura!=NULL){test++;}
											if(k_i<i)for(int k=i-1;k>=k_i;k--)if(tabla[k][j].figura!=NULL){test++;}
											if(tabla[k_i][k_j].figura!=NULL&&test==1&&tabla[i][j].figura->ind!=tabla[k_i][k_j].figura->ind)test=0;
											if(test==0)
											{
												pomeraj.Set(0,0,-(k_i-i));
												tabla[i][j].figura->Move(pomeraj);
												tabla[k_i][j].figura=tabla[i][j].figura;
												tabla[i][j].figura=NULL;
											}
										}
			}


			if (tabla[i][j].figura == NULL)
			{
				proveriSah();
				if (pom1->ind == 0 && crni_sah == 1)
				{
					pomeraj = pomeraj * (-1);
					tabla[k_i][k_j].figura->Move(pomeraj);
					crni_sah = 0;
					tabla[i][j].figura = pom1;
					tabla[k_i][k_j].figura = pom2;
				}
				if (pom1->ind == 1 && beli_sah == 1)
				{
					pomeraj = pomeraj * (-1);
					tabla[k_i][k_j].figura->Move(pomeraj);
					beli_sah = 0;
					tabla[i][j].figura = pom1;
					tabla[k_i][k_j].figura = pom2;
				}
			}
}
void Engine::PomeriKonja(int i,int j,int k_i,int k_j)
{
																									
											Figura *pom1;
											Figura *pom2;
											pom1=tabla[i][j].figura;

											pom2=tabla[k_i][k_j].figura;
	if(tabla[k_i][k_j].figura!=NULL&&tabla[k_i][k_j].figura->ind==tabla[i][j].figura->ind)return;
	Vector3D pomeraj;
	pomeraj.Set((k_j-j),0,-(k_i-i));
	tabla[i][j].figura->Move(pomeraj);
    tabla[k_i][k_j].figura=tabla[i][j].figura;
	tabla[i][j].figura=NULL;

	if (tabla[i][j].figura == NULL)
	{
		proveriSah();
		if (pom1->ind == 0 && crni_sah == 1)
		{
			pomeraj = pomeraj * (-1);
			tabla[k_i][k_j].figura->Move(pomeraj);
			crni_sah = 0;
			tabla[i][j].figura = pom1;
			tabla[k_i][k_j].figura = pom2;
		}
		if (pom1->ind == 1 && beli_sah == 1)
		{
			pomeraj = pomeraj * (-1);
			tabla[k_i][k_j].figura->Move(pomeraj);
			beli_sah = 0;
			tabla[i][j].figura = pom1;
			tabla[k_i][k_j].figura = pom2;
		}
	}
}
void Engine::Zamena(int i,int j,int K_i,int k_j)
{
	Figura *pom1;
	Figura *pom2;
	pom1 = tabla[i][j].figura;

	pom2 = tabla[K_i][k_j].figura;
	Vector3D pomeraj;
	
	if(i==0||i==7)
	{
		if(k_j>j)
		{
			if(tabla[i][j+1].figura==NULL&&tabla[i][j+2].figura==NULL)
			{
				pomeraj.Set(2,0,0);
				tabla[i][j].figura->Move(pomeraj);
				pomeraj.Set(-2,0,0);
				tabla[i][k_j].figura->Move(pomeraj);
				tabla[i][j+1].figura=tabla[K_i][k_j].figura;
				tabla[i][j+2].figura=tabla[i][j].figura;
				tabla[i][j].figura=NULL;
				tabla[i][k_j].figura=NULL;
				
			}
		}
		else
		{
			if(tabla[i][j-1].figura==NULL&&tabla[i][j-2].figura==NULL)
			{
				pomeraj.Set(-2,0,0);
				tabla[i][j].figura->Move(pomeraj);
				pomeraj.Set(+2,0,0);
				tabla[i][k_j].figura->Move(pomeraj);
				tabla[i][j-1].figura=tabla[K_i][k_j].figura;
				tabla[i][j-2].figura=tabla[i][j].figura;
				tabla[i][j].figura=NULL;
				tabla[i][k_j].figura=NULL;
				
			}
		}
	}

	if (tabla[i][j].figura == NULL)
	{
		proveriSah();

		if (pom1->ind == 0 && crni_sah == 1)
		{
			if (k_j > j)
			{

				tabla[i][j + 2].figura->Move(pomeraj);
				tabla[i][j + 2].figura = NULL;
				pomeraj = pomeraj * (-1);
				tabla[i][j + 1].figura->Move(pomeraj);
				tabla[i][j + 1].figura = NULL;
			}
			else
			{
				tabla[i][j - 2].figura->Move(pomeraj);
				tabla[i][j - 2].figura = NULL;
				pomeraj = pomeraj * (-1);
				tabla[i][j - 1].figura->Move(pomeraj);
				tabla[i][j - 1].figura = NULL;

			}
			crni_sah = 0;
			tabla[i][j].figura = pom1;
			tabla[K_i][k_j].figura = pom2;
		}

		if (pom1->ind == 1 && beli_sah == 1)
		{
			if (k_j > j)
			{

				tabla[i][j + 2].figura->Move(pomeraj);
				tabla[i][j + 2].figura = NULL;
				pomeraj = pomeraj * (-1);
				tabla[i][j + 1].figura->Move(pomeraj);
				tabla[i][j + 1].figura = NULL;
			}
			else
			{
				tabla[i][j - 2].figura->Move(pomeraj);
				tabla[i][j - 2].figura = NULL;
				pomeraj = pomeraj * (-1);
				tabla[i][j - 1].figura->Move(pomeraj);
				tabla[i][j - 1].figura = NULL;

			}
			beli_sah = 0;
			tabla[i][j].figura = pom1;
			tabla[K_i][k_j].figura = pom2;
		}

	}

}
void Engine::ispisPobednika()
{
	glColor3f(0.7,0.7,0.7);
	glBegin(GL_POLYGON);
	glVertex3f(-0.5,0.5,0);
	glVertex3f(0.5,0.5,0);
	glVertex3f(0.5,-0.5,0);
	glVertex3f(-0.5,-0.5,0);
	glEnd();
	glColor3f(0.2,0.2,0.7);
	
	if(crni_sah==2)
	{
		char s[200];
		sprintf_s(s,"          Partija broj:%d\n      Sah-mat u %d poteza\n             Pobednik\n           Igrac 1 - Beli\n\n   Za novu igru pritisni R ",partija_broj,beli_broj_poteza);
		glRasterPos2f(-0.4,0.3);
		glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,(const unsigned char*) s);
	}
	if(beli_sah==2)
	{
		char s[200];
		sprintf_s(s,"          Partija broj:%d\n      Sah-mat u %d poteza\n             Pobednik\n           Igrac 2 - Crni\n\n   Za novu igru pritisni R ",partija_broj,crni_broj_poteza);
		glRasterPos2f(-0.4,0.3);
		glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,(const unsigned char*) s);
	}

}
void Engine::Proveri(int x,int y,int mode)
{
	int i, j;
	if (mode == 0)
	{


		unsigned char res[4];
		GLint viewport[4];

		glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
		glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
		glGetIntegerv(GL_VIEWPORT, viewport);
		glReadPixels(x, viewport[3] - y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &res);

		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

	
		i = res[0] / 10 - 1;
		j = res[0] % 10;
		if (res[0] == 0)
		{
			//pocetna_pozicija_i=-1;
			//pocetna_pozicija_j=-1;
			return;
		}


		GLdouble modelview[16];
		GLdouble projection[16];
		GLfloat winX, winY, winZ;
		GLdouble posX, posY, posZ;

		glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
		glGetDoublev(GL_PROJECTION_MATRIX, projection);

		Vector4D proba;
		winX = (float)x;
		winY = (float)viewport[3] - (float)y;
		glReadPixels(x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

		gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

		//if(posX>0.65&&posY>0)return;
		//if(posX<-0.65&&posY>0)return;
		if (posY >= 0.85 || posY <= -0.85)return;
	}
	else
	{
		i = x;
		j = y;
	}


										if(tabla[i][j].figura!=NULL&&tabla[i][j].figura->ind==igrac_igra)
										{
											if (pocetna_pozicija_i!=-1&&tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura != NULL && strcmp(typeid(*tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura).name(), "class Kralj") == 0 && abs(j - pocetna_pozicija_j) == 3 && pocetna_pozicija_i == i && strcmp(typeid(*tabla[i][j].figura).name(), "class Top") == 0)
											{
												Zamena(pocetna_pozicija_i, pocetna_pozicija_j, i, j);
												if (tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura == NULL && igrac_igra == 1)
												{
													igrac_igra = 0;

													beli_broj_poteza++;
													pocetna_pozicija_i = -1;
													pocetna_pozicija_j = -1;
												}
												else if (tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura == NULL && igrac_igra == 0)
												{
													igrac_igra = 1;

													crni_broj_poteza++;
													pocetna_pozicija_i = -1;
													pocetna_pozicija_j = -1;
												}
												else
												{

													pocetna_pozicija_i = i;
													pocetna_pozicija_j = j;
												}
											}
											else
											{

												pocetna_pozicija_i = i;
												pocetna_pozicija_j = j;
											}
											
										}
										else if(pocetna_pozicija_i!=-1)
										{

											if(strcmp(typeid(*tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura).name(),"class Pesak")==0)
												{
													if(abs(i-pocetna_pozicija_i)==1||abs(i-pocetna_pozicija_i)==2)
													{
														if(tabla[i][j].figura!=NULL&&strcmp(typeid(*tabla[i][j].figura).name(),"class Kralj")!=0)PomeriPesak(pocetna_pozicija_i,pocetna_pozicija_j,j,i);
														if(tabla[i][j].figura==NULL)PomeriPesak(pocetna_pozicija_i,pocetna_pozicija_j,j,i);
														if(tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura==NULL&&igrac_igra==1)
															{
																igrac_igra=0;
																
																beli_broj_poteza++;
															}
														else if(tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura==NULL&&igrac_igra==0)
															{
																igrac_igra=1;
															
																crni_broj_poteza++;
															}
													}
												}
											else if(strcmp(typeid(*tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura).name(),"class Top")==0)
												{
													if((pocetna_pozicija_i!=i&&pocetna_pozicija_j==j)||(pocetna_pozicija_j!=j&&pocetna_pozicija_i==i))
														{
															if(tabla[i][j].figura!=NULL&&strcmp(typeid(*tabla[i][j].figura).name(),"class Kralj")!=0)PomeriTop(pocetna_pozicija_i,pocetna_pozicija_j,i,j);
															if(tabla[i][j].figura==NULL)PomeriTop(pocetna_pozicija_i,pocetna_pozicija_j,i,j);
															if(tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura==NULL&&igrac_igra==1)
																{
																	igrac_igra=0;
																	
																	beli_broj_poteza++;
																}
															else if(tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura==NULL&&igrac_igra==0)
																{
																	igrac_igra=1;
																	
																	crni_broj_poteza++;
																}
														}
												}
											else if(strcmp(typeid(*tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura).name(),"class Lovac")==0)
												{ 
													if(pocetna_pozicija_i!=i&&pocetna_pozicija_j!=j&&(abs(pocetna_pozicija_i-i)==abs(pocetna_pozicija_j-j))) 
														{
															if(tabla[i][j].figura!=NULL&&strcmp(typeid(*tabla[i][j].figura).name(),"class Kralj")!=0)PomeriLovac(pocetna_pozicija_i,pocetna_pozicija_j,i,j);
															if(tabla[i][j].figura==NULL)PomeriLovac(pocetna_pozicija_i,pocetna_pozicija_j,i,j);
															if(tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura==NULL&&igrac_igra==1)
																{
																	igrac_igra=0;
																	
																	beli_broj_poteza++;
																}
															else if(tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura==NULL&&igrac_igra==0)
																{
																	igrac_igra=1;
																	
																	crni_broj_poteza++;
																}
														}
												}
											else if(strcmp(typeid(*tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura).name(),"class Kraljica")==0)
												{
													if(((pocetna_pozicija_i!=i&&pocetna_pozicija_j==j)||(pocetna_pozicija_j!=j&&pocetna_pozicija_i==i))||(pocetna_pozicija_i!=i&&pocetna_pozicija_j!=j&&(abs(pocetna_pozicija_i-i)==abs(pocetna_pozicija_j-j))))
														{
															if(tabla[i][j].figura!=NULL&&strcmp(typeid(*tabla[i][j].figura).name(),"class Kralj")!=0)PomeriKralja_i_Kraljicu(pocetna_pozicija_i,pocetna_pozicija_j,i,j);
															if(tabla[i][j].figura==NULL)PomeriKralja_i_Kraljicu(pocetna_pozicija_i,pocetna_pozicija_j,i,j);
															if(tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura==NULL&&igrac_igra==1)
																{
																	igrac_igra=0;
																
																	beli_broj_poteza++;
																}
															else if(tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura==NULL&&igrac_igra==0)
																{
																	igrac_igra=1;
																
																	crni_broj_poteza++;
																}
														}
												}
											else if(strcmp(typeid(*tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura).name(),"class Kralj")==0)
												{ 
													if((((pocetna_pozicija_i!=i&&pocetna_pozicija_j==j)||(pocetna_pozicija_j!=j&&pocetna_pozicija_i==i))||(pocetna_pozicija_i!=i&&pocetna_pozicija_j!=j&&(abs(pocetna_pozicija_i-i)==abs(pocetna_pozicija_j-j))))&&(abs(i-pocetna_pozicija_i)<=1)&&(abs(j-pocetna_pozicija_j)<=1))
														{
														    if(tabla[i][j].figura!=NULL&&strcmp(typeid(*tabla[i][j].figura).name(),"class Kralj")!=0)PomeriKralja_i_Kraljicu(pocetna_pozicija_i,pocetna_pozicija_j,i,j);
															if(tabla[i][j].figura==NULL)PomeriKralja_i_Kraljicu(pocetna_pozicija_i,pocetna_pozicija_j,i,j);
															if(tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura==NULL&&igrac_igra==1)
																{
																	igrac_igra=0;
																	
																	beli_broj_poteza++;
																}
															else if(tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura==NULL&&igrac_igra==0)
																{
																	igrac_igra=1;
																	
																	crni_broj_poteza++;
																}
														}


												}
											else if(strcmp(typeid(*tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura).name(),"class Konj")==0)
												{ 
													if((abs(i-pocetna_pozicija_i)==2&&abs(pocetna_pozicija_j-j)==1)||(abs(i-pocetna_pozicija_i)==1&&abs(pocetna_pozicija_j-j)==2)) 
														{
															if(tabla[i][j].figura!=NULL&&strcmp(typeid(*tabla[i][j].figura).name(),"class Kralj")!=0)PomeriKonja(pocetna_pozicija_i,pocetna_pozicija_j,i,j);
															if(tabla[i][j].figura==NULL)PomeriKonja(pocetna_pozicija_i,pocetna_pozicija_j,i,j);
															if(tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura==NULL&&igrac_igra==1)
																{
																	igrac_igra=0;
																
																	beli_broj_poteza++;
																}
															else if(tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura==NULL&&igrac_igra==0)
																{
																	igrac_igra=1;
																	
																	crni_broj_poteza++;
																}
														}
												}

											if (tabla[pocetna_pozicija_i][pocetna_pozicija_j].figura == NULL)
											{
												pocetna_pozicija_i = -1;
												pocetna_pozicija_j = -1;
											}
									}
		

	
		
				glutPostRedisplay();
}