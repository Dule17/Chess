#include "GL\glew.h"
#include "Pozadina.h"
#include <GL/freeglut.h>  
#include <math.h>
#include "vector3d.h"
#include "vector4d.h"
#include "matrix4x4.h"
#include <vector>
#include <SOIL.h>
#include <cstdlib> 
#include <time.h>
Pozadina::Pozadina(void)
{




}

void Pozadina::createWorld()
{

	glGenTextures(1, &texture);


	int width, height;
	unsigned char* image;
	srand(time(NULL));
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	int ind=rand()%3+1;

	for(int i=0;i<6;i++)
	{
		if(ind==1)
		{
					if(i==0)	image = SOIL_load_image("Skybox/1/posx.jpg", &width, &height, 0, SOIL_LOAD_RGB);
					else if(i==1)	image = SOIL_load_image("Skybox/1/negx.jpg", &width, &height, 0, SOIL_LOAD_RGB);
					else if(i==2)	image = SOIL_load_image("Skybox/1/posy.jpg", &width, &height, 0, SOIL_LOAD_RGB);
					else if(i==3)	image = SOIL_load_image("Skybox/1/negy.jpg", &width, &height, 0, SOIL_LOAD_RGB);
					else if(i==4)	image = SOIL_load_image("Skybox/1/posz.jpg", &width, &height, 0, SOIL_LOAD_RGB);
					else 	image = SOIL_load_image("Skybox/1/negz.jpg", &width, &height, 0, SOIL_LOAD_RGB);
		}else if(ind==2)
		{
					if(i==0)	image = SOIL_load_image("Skybox/2/posx.jpg", &width, &height, 0, SOIL_LOAD_RGB);
					else if(i==1)	image = SOIL_load_image("Skybox/2/negx.jpg", &width, &height, 0, SOIL_LOAD_RGB);
					else if(i==2)	image = SOIL_load_image("Skybox/2/posy.jpg", &width, &height, 0, SOIL_LOAD_RGB);
					else if(i==3)	image = SOIL_load_image("Skybox/2/negy.jpg", &width, &height, 0, SOIL_LOAD_RGB);
					else if(i==4)	image = SOIL_load_image("Skybox/2/posz.jpg", &width, &height, 0, SOIL_LOAD_RGB);
					else 	image = SOIL_load_image("Skybox/2/negz.jpg", &width, &height, 0, SOIL_LOAD_RGB);
		}
		else
		{
					if(i==0)	image = SOIL_load_image("Skybox/3/posx.jpg", &width, &height, 0, SOIL_LOAD_RGB);
					else if(i==1)	image = SOIL_load_image("Skybox/3/negx.jpg", &width, &height, 0, SOIL_LOAD_RGB);
					else if(i==2)	image = SOIL_load_image("Skybox/3/posy.jpg", &width, &height, 0, SOIL_LOAD_RGB);
					else if(i==3)	image = SOIL_load_image("Skybox/3/negy.jpg", &width, &height, 0, SOIL_LOAD_RGB);
					else if(i==4)	image = SOIL_load_image("Skybox/3/posz.jpg", &width, &height, 0, SOIL_LOAD_RGB);
					else 	image = SOIL_load_image("Skybox/3/negz.jpg", &width, &height, 0, SOIL_LOAD_RGB);
		}

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
	}
glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
glBindTexture(GL_TEXTURE_CUBE_MAP, 0);



	
}
void Pozadina::Draw()
{
	glColor3f(1,1,1);
	glEnable(GL_TEXTURE_CUBE_MAP);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
 GLfloat fExtent = 10.0f;
 
    glBegin(GL_QUADS);
        //////////////////////////////////////////////
        // Negative X
        glTexCoord3f(-1.0f, -1.0f, 1.0f);
        glVertex3f(-fExtent, -0.1f, fExtent);
        
        glTexCoord3f(-1.0f, -1.0f, -1.0f);
        glVertex3f(-fExtent, -0.1f, -fExtent);
        
        glTexCoord3f(-1.0f, 1.0f, -1.0f);
        glVertex3f(-fExtent, fExtent, -fExtent);
        
        glTexCoord3f(-1.0f, 1.0f, 1.0f);
        glVertex3f(-fExtent, fExtent, fExtent);


        ///////////////////////////////////////////////
        //  Postive X
        glTexCoord3f(1.0f, -1.0f, -1.0f);
        glVertex3f(fExtent, -0.1f, -fExtent);
        
        glTexCoord3f(1.0f, -1.0f, 1.0f);
        glVertex3f(fExtent, -0.1f, fExtent);
        
        glTexCoord3f(1.0f, 1.0f, 1.0f);
        glVertex3f(fExtent, fExtent, fExtent);
        
        glTexCoord3f(1.0f, 1.0f, -1.0f);
        glVertex3f(fExtent, fExtent, -fExtent);
 

        ////////////////////////////////////////////////
        // Negative Z 
        glTexCoord3f(-1.0f, -1.0f, -1.0f);
        glVertex3f(-fExtent, -0.1f, -fExtent);
        
        glTexCoord3f(1.0f, -1.0f, -1.0f);
        glVertex3f(fExtent,-0.1f, -fExtent);
        
        glTexCoord3f(1.0f, 1.0f, -1.0f);
        glVertex3f(fExtent, fExtent, -fExtent);
        
        glTexCoord3f(-1.0f, 1.0f, -1.0f);
        glVertex3f(-fExtent, fExtent, -fExtent);


        ////////////////////////////////////////////////
        // Positive Z 
        glTexCoord3f(1.0f, -1.0f, 1.0f);
        glVertex3f(fExtent, -0.1f, fExtent);
        
        glTexCoord3f(-1.0f, -1.0f, 1.0f);
        glVertex3f(-fExtent, -0.1f, fExtent);
        
        glTexCoord3f(-1.0f, 1.0f, 1.0f);
        glVertex3f(-fExtent, fExtent, fExtent);
        
        glTexCoord3f(1.0f, 1.0f, 1.0f);
        glVertex3f(fExtent, fExtent, fExtent);


        //////////////////////////////////////////////////
        // Positive Y
        glTexCoord3f(-1.0f, 1.0f, 1.0f);
        glVertex3f(-fExtent, fExtent, fExtent);
        
        glTexCoord3f(-1.0f, 1.0f, -1.0f);
        glVertex3f(-fExtent, fExtent, -fExtent);
        
        glTexCoord3f(1.0f, 1.0f, -1.0f);
        glVertex3f(fExtent, fExtent, -fExtent);
        
        glTexCoord3f(1.0f, 1.0f, 1.0f);
        glVertex3f(fExtent, fExtent, fExtent);
  
    
        ///////////////////////////////////////////////////
        // Negative Y
        glTexCoord3f(-1.0f, -1.0f, -1.0f);
        glVertex3f(-fExtent, -0.1f, -fExtent);
        
        glTexCoord3f(-1.0f, -1.0f, 1.0f);
        glVertex3f(-fExtent, -0.1f, fExtent);
        
        glTexCoord3f(1.0f, -1.0f, 1.0f);
        glVertex3f(fExtent, -0.1f, fExtent);
        
        glTexCoord3f(1.0f, -1.0f, -1.0f);
        glVertex3f(fExtent, -0.1f, -fExtent);
    glEnd();
	glDisable(GL_TEXTURE_CUBE_MAP);

}
Pozadina::~Pozadina(void)
{
}
