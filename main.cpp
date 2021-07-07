#include<GL\glew.h>
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
#include "Engine.h"
#include "Pozadina.h"
#include <GLFW/glfw3.h>

#define M_PI 3.14
using namespace std;
double brzina = 1.0;
/* Global variables */
char title[] = "Sah";

Vector3D camera(0.0, 7.0, 0.0);
//Vector3D camera(0.0, 0.0, 6.0);
Vector3D lookAt(0.0, 0.0, 0.0);
Vector3D lookUp(0.0, 0.0,- 0.1);
int FPS = 60;
double WINDOW_HEIGHT = 480;

double aspectRatio = 640.0f / 480.0f;
double zoom = 80.0;
#define MOVING_CONST 0.3
#define ROTATION_CONST 3.14/45.0

void Transform(vector<Vector3D> v, Matrix4x4 mt)
{
	for (int i = 0; i < v.size(); i++)v[i] = mt.Transform(v[i]);
}
Engine *e;

void draw_axis()
{

    glColor3f(1.0,0.0,0.0);
    glBegin(GL_LINES);
		glVertex3f(0, 0, 0);
		glVertex3f(2, 0, 0);
    glEnd();

    glColor3f(0.0,1.0,0.0);
    glBegin(GL_LINES);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 2, 0);
    glEnd();

    glColor3f(0.0,0.0,1.0);
    glBegin(GL_LINES);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 2);
    glEnd();
}

int i_joystick = 4, j_joystick = 4;
int joystick_con;
void drawJoystick()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor3f(0.7, 1, 0.2);
	glBegin(GL_POLYGON);
	glVertex3f(e->tabla[i_joystick][j_joystick].t1.m_x, e->tabla[i_joystick][j_joystick].t1.m_y, e->tabla[i_joystick][j_joystick].t1.m_z);
	glVertex3f(e->tabla[i_joystick][j_joystick].t2.m_x, e->tabla[i_joystick][j_joystick].t2.m_y, e->tabla[i_joystick][j_joystick].t2.m_z);
	glVertex3f(e->tabla[i_joystick][j_joystick].t3.m_x, e->tabla[i_joystick][j_joystick].t3.m_y, e->tabla[i_joystick][j_joystick].t3.m_z);
	glVertex3f(e->tabla[i_joystick][j_joystick].t4.m_x, e->tabla[i_joystick][j_joystick].t4.m_y, e->tabla[i_joystick][j_joystick].t4.m_z);
	glEnd();
}
void display()
{




	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, e->fbo);

	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(zoom, aspectRatio, 0.1f, 50.0f);
	gluLookAt(camera.m_x, camera.m_y, camera.m_z,
		lookAt.m_x, lookAt.m_y, lookAt.m_z,
		lookUp.m_x, lookUp.m_y, lookUp.m_z);

	glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT); 
	e->drawTabla(1);
		e->drawFigure(1);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();


	gluPerspective(zoom, aspectRatio, 0.1f, 50.0f);
	gluLookAt(camera.m_x, camera.m_y, camera.m_z,
		lookAt.m_x, lookAt.m_y, lookAt.m_z,
		lookUp.m_x, lookUp.m_y, lookUp.m_z);
	if (joystick_con == 1)drawJoystick();
if(e->pocetna_pozicija_i!=-1)e->drawMogucniPokreti();
	e->drawTabla(0);
	e->drawFigure(0);
	
		e->pozadina.Draw();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);

	e->drawIspis();
	if(e->igra_u_toku==1)e->ispisPobednika();
	glEnable(GL_DEPTH_TEST);
    glutSwapBuffers();  // Swap the front and back frame buffers (double buffering)
}

void SetCamera()
{
	glViewport(0, 0, WINDOW_HEIGHT*aspectRatio, WINDOW_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(80.0f, aspectRatio, 0.1f, 50.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(camera.m_x, camera.m_y, camera.m_z,
		0.0, 1.0, 0.0,
		0.0, 1.0, 0.0);
}

void initGL()
{
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
   glShadeModel(GL_FLAT);

   glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
}

void reshape(GLsizei width, GLsizei height)
{
	if (height == 0) height = 1;
	GLfloat aspect = (GLfloat)width / (GLfloat)height;

	// Set the viewport to cover the new window
	glViewport(0, 0, width, height);

	// Set the aspect ratio of the clipping volume to match the viewport
	glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
	glLoadIdentity();             // Reset
	gluPerspective(80.0f, aspect, 0.1f, 50.0f);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(camera.m_x, camera.m_y, camera.m_z,
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0);
	glDeleteFramebuffers(1,&e->fbo);
	glDeleteTextures(1,&e->texture);
	glDeleteTextures(1,&e->depthTexture);
	e->createFrameBufferObject(glutGet(GLUT_WINDOW_WIDTH),glutGet(GLUT_WINDOW_HEIGHT));

}




void idiGore()
{
	Matrix4x4 mt;
	Vector3D v;

	v = lookUp * MOVING_CONST;
	v.Normalize();
	mt.loadTranslate(v.m_x, v.m_y, v.m_z);
	if (camera.m_y + v.m_y > 0.2&&camera.m_y + v.m_y<9 && camera.m_x + v.m_x>-9 && camera.m_x + v.m_x<9 && camera.m_z + v.m_z>-9 && camera.m_z + v.m_z < 9)
	{
		int test = 0;
		for (int i = 0; i < e->tabla_n; i++)
			for (int j = 0; j < e->tabla_n; j++)
			{
				Vector3D d1((e->tabla[i][j].t1.m_x + e->tabla[i][j].t2.m_x) / 2, 0, (e->tabla[i][j].t1.m_z + e->tabla[i][j].t4.m_z) / 2);

				if (e->tabla[i][j].figura != NULL && e->tabla[i][j].figura->visina >= camera.m_y + v.m_y && (sqrt((camera.m_x + v.m_x - d1.m_x)*(camera.m_x + v.m_x - d1.m_x) + (camera.m_z + v.m_z - d1.m_z)*(camera.m_z + v.m_z - d1.m_z)) <= 0.6)) { test = 1; break; }
			}

		if (test == 0)
		{
			camera = mt * camera;
			lookAt = mt * lookAt;
		}
	}

}
void idiDole()
{
	Matrix4x4 mt;
	Vector3D v;

	v = lookUp * (-MOVING_CONST);
	v.Normalize();
	mt.loadTranslate(v.m_x, v.m_y, v.m_z);
	if (camera.m_y + v.m_y > 0.2&&camera.m_y + v.m_y<9 && camera.m_x + v.m_x>-9 && camera.m_x + v.m_x<9 && camera.m_z + v.m_z>-9 && camera.m_z + v.m_z < 9)
	{
		int test = 0;
		for (int i = 0; i < e->tabla_n; i++)
			for (int j = 0; j < e->tabla_n; j++)
			{
				Vector3D d1((e->tabla[i][j].t1.m_x + e->tabla[i][j].t2.m_x) / 2, 0, (e->tabla[i][j].t1.m_z + e->tabla[i][j].t4.m_z) / 2);

				if (e->tabla[i][j].figura != NULL && e->tabla[i][j].figura->visina >= camera.m_y + v.m_y && (sqrt((camera.m_x + v.m_x - d1.m_x)*(camera.m_x + v.m_x - d1.m_x) + (camera.m_z + v.m_z - d1.m_z)*(camera.m_z + v.m_z - d1.m_z)) <= 0.6)) { test = 1; break; }
			}

		if (test == 0)
		{
			camera = mt * camera;
			lookAt = mt * lookAt;
		}
	}

}


void idiNapred()
{
	Matrix4x4 mt;
	Vector3D v;
	v=lookAt-camera;
	v.Normalize();
	v*= MOVING_CONST;
	mt.loadTranslate(v.m_x,v.m_y,v.m_z);
	if(camera.m_y+v.m_y>0.2&&camera.m_y+v.m_y<9&&camera.m_x+v.m_x>-9&&camera.m_x+v.m_x<9&&camera.m_z+v.m_z>-9&&camera.m_z+v.m_z<9)
	{
		int test=0;
		for(int i=0;i<e->tabla_n;i++)
			for(int j=0;j<e->tabla_n;j++)
			{
				Vector3D d1((e->tabla[i][j].t1.m_x+e->tabla[i][j].t2.m_x)/2,0,(e->tabla[i][j].t1.m_z+e->tabla[i][j].t4.m_z)/2);

				if(e->tabla[i][j].figura!=NULL&&e->tabla[i][j].figura->visina>=camera.m_y+v.m_y&&(sqrt( (camera.m_x+v.m_x-d1.m_x)*(camera.m_x+v.m_x-d1.m_x)+(camera.m_z+v.m_z-d1.m_z)*(camera.m_z+v.m_z-d1.m_z))<=0.6)){test=1;break;}
			}

		if(test==0)
			{
				camera=mt*camera;
				lookAt=mt*lookAt;
			}
	}
	
}
void idiNazad()
{
	Matrix4x4 mt;
	Vector3D v;
	v=lookAt-camera;
	v.Normalize();
	v*=-MOVING_CONST;
	mt.loadTranslate(v.m_x,v.m_y,v.m_z);
	if(camera.m_y+v.m_y>0.2&&camera.m_y+v.m_y<9&&camera.m_x+v.m_x>-9&&camera.m_x+v.m_x<9&&camera.m_z+v.m_z>-9&&camera.m_z+v.m_z<9)
	{
		int test=0;
		for(int i=0;i<e->tabla_n;i++)
			for(int j=0;j<e->tabla_n;j++)
			{
				Vector3D d1((e->tabla[i][j].t1.m_x+e->tabla[i][j].t2.m_x)/2,0,(e->tabla[i][j].t1.m_z+e->tabla[i][j].t4.m_z)/2);

				if(e->tabla[i][j].figura!=NULL&&e->tabla[i][j].figura->visina>=camera.m_y+v.m_y&&(sqrt( (camera.m_x+v.m_x-d1.m_x)*(camera.m_x+v.m_x-d1.m_x)+(camera.m_z+v.m_z-d1.m_z)*(camera.m_z+v.m_z-d1.m_z))<=0.6)){test=1;break;}
			}

		if(test==0)
			{
				camera=mt*camera;
				lookAt=mt*lookAt;
			}
	}
	
}
void idiLevo()
{
	Matrix4x4 mt;
	Vector3D v;
	v=lookAt-camera;
	v=lookUp.Cross(v);
	v.Normalize();
	v*= MOVING_CONST;
	mt.loadTranslate(v.m_x,v.m_y,v.m_z);
	if(camera.m_y+v.m_y>0.2&&camera.m_y+v.m_y<9&&camera.m_x+v.m_x>-9&&camera.m_x+v.m_x<9&&camera.m_z+v.m_z>-9&&camera.m_z+v.m_z<9)
	{
		int test=0;
		for(int i=0;i<e->tabla_n;i++)
			for(int j=0;j<e->tabla_n;j++)
			{
				Vector3D d1((e->tabla[i][j].t1.m_x+e->tabla[i][j].t2.m_x)/2,0,(e->tabla[i][j].t1.m_z+e->tabla[i][j].t4.m_z)/2);

				if(e->tabla[i][j].figura!=NULL&&e->tabla[i][j].figura->visina>=camera.m_y+v.m_y&&(sqrt( (camera.m_x+v.m_x-d1.m_x)*(camera.m_x+v.m_x-d1.m_x)+(camera.m_z+v.m_z-d1.m_z)*(camera.m_z+v.m_z-d1.m_z))<=0.6)){test=1;break;}
			}

		if(test==0)
			{
				camera=mt*camera;
				lookAt=mt*lookAt;
			}
	}
	
}
void idiDesno()
{
	Matrix4x4 mt;
	Vector3D v;
	v=lookAt-camera;
	v=lookUp.Cross(v);
	v.Normalize();
	v*=-MOVING_CONST;
	mt.loadTranslate(v.m_x,v.m_y,v.m_z);
	if(camera.m_y+v.m_y>0.2&&camera.m_y+v.m_y<9&&camera.m_x+v.m_x>-9&&camera.m_x+v.m_x<9&&camera.m_z+v.m_z>-9&&camera.m_z+v.m_z<9)
	{
		int test=0;
		for(int i=0;i<e->tabla_n;i++)
			for(int j=0;j<e->tabla_n;j++)
			{
				Vector3D d1((e->tabla[i][j].t1.m_x+e->tabla[i][j].t2.m_x)/2,0,(e->tabla[i][j].t1.m_z+e->tabla[i][j].t4.m_z)/2);

				if(e->tabla[i][j].figura!=NULL&&e->tabla[i][j].figura->visina>=camera.m_y+v.m_y&&(sqrt( (camera.m_x+v.m_x-d1.m_x)*(camera.m_x+v.m_x-d1.m_x)+(camera.m_z+v.m_z-d1.m_z)*(camera.m_z+v.m_z-d1.m_z))<=0.6)){test=1;break;}
			}

		if(test==0)
			{
				camera=mt*camera;
				lookAt=mt*lookAt;
			}
	}
	
}

void gledajGore()
{
	Matrix4x4 mt;
	Vector3D v;
	v=lookAt-camera;
	v=lookUp.Cross(v);
	v.Normalize();
	mt.loadRotate(camera,v,-ROTATION_CONST);
	lookAt=mt*lookAt;
	Vector4D t=lookUp;
	t.m_w=0;
	lookUp=mt*t;

}
void gledajDole()
{
	Matrix4x4 mt;
	Vector3D v;
	v=lookAt-camera;
	v=lookUp.Cross(v);
	v.Normalize();
	mt.loadRotate(camera,v, ROTATION_CONST);
	lookAt=mt*lookAt;
	Vector4D t=lookUp;
	t.m_w=0;
	lookUp=mt*t;

}

void rotateCameraOkoSvojeOseLevo()
{
	Matrix4x4 mt, mv1, mv2, mr;
	mt.loadRotate(camera, lookUp, ROTATION_CONST);
	lookAt = mt * lookAt;
	Vector4D t = lookUp;
	t.m_w = 0;
	lookUp = mt * t;
}
void rotateCameraOkoSvojeOseDesno()
{
	Matrix4x4 mt, mv1, mv2, mr;
	mt.loadRotate(camera, lookUp, -ROTATION_CONST);
	lookAt = mt * lookAt;
	Vector4D t = lookUp;
	t.m_w = 0;
	lookUp = mt * t;
}
void rotateLevo()
{
		Matrix4x4 mt,mv1,mv2,mr;
		mv1.loadTranslate(-camera.m_x,-camera.m_y,-camera.m_z);
		mv2.loadTranslate(camera.m_x,camera.m_y,camera.m_z);
		mr.loadRotateY(ROTATION_CONST);
		mt=mv2*mr*mv1;
		
		lookAt=mt*lookAt;
		Vector4D t=lookUp;
		t.m_w=0;
		lookUp=mt*t;

}
void rotateDesno()
{
		Matrix4x4 mt,mv1,mv2,mr;
		mv1.loadTranslate(-camera.m_x,-camera.m_y,-camera.m_z);
		mv2.loadTranslate(camera.m_x,camera.m_y,camera.m_z);
		mr.loadRotateY(-ROTATION_CONST);
		mt=mv2*mr*mv1;
		lookAt=mt*lookAt;
		Vector4D t=lookUp;
		t.m_w=0;
		lookUp=mt*t;

}
int mode = 0;
void KeyboardKeyPressed(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'r':	if(e->igra_u_toku==1)
					{
						e->postaviFigure();
						e->igra_u_toku=0;
						e->igrac_igra=1;
						e->beli_sah=0;
						e->crni_sah=0;
						e->beli_broj_poteza=0;
						e->crni_broj_poteza=0;
						e->partija_broj++;
					 }
					break;
		case 't' :idiGore(); break;
		case 'g': idiDole(); break;
		case 'w': idiNapred(); break;
		case 's': idiNazad(); break;
		case 'a': idiLevo(); break;
		case 'd': idiDesno(); break;
		case '8': gledajGore();break;
		case '2': gledajDole();break;
		case '4': rotateLevo();break;
		case '6': rotateDesno();break;
		case '7': rotateCameraOkoSvojeOseLevo(); break;
		case '9': rotateCameraOkoSvojeOseDesno(); break;
		case 'm':if (mode == 0)mode = 1;
				 else mode = 0; break;

		default:
			break;
	}
	glutPostRedisplay();
}
int pomx = -1;
int pomy = -1;
int misRotacija = -1;

void Test(int x, int y)
{
	if (misRotacija ==1 &&mode==0)
	{

	/*	Matrix4x4  mr1,mr2,mt;

		if (y < pomy)mr1.loadRotateX(-ROTATION_CONST);
		else mr1.loadRotateX(ROTATION_CONST);
		if (x < pomx)mr2.loadRotateZ(-ROTATION_CONST);
		else mr2.loadRotateZ(ROTATION_CONST);
		mt = mr1 * mr2;
		lookAt = mt * lookAt;
		Vector4D t = lookUp;
		t.m_w = 0;
		camera = mt * camera;
		lookUp = mt * t;

		pomx = x;
		pomy = y;*/

			Matrix4x4 mt,mx,my;
			Vector3D v;
			my.identity();
			mx.identity();
			v = lookAt - camera;
			v = lookUp.Cross(v);
			v.Normalize();

			my.loadRotate(camera, v,(y-pomy)*0.0020f);


			mx.loadRotate(camera, lookUp, (pomx-x)*0.0020f);


			mt = mx * my;
			
			
			lookAt = mt * lookAt;
			Vector4D t = lookUp;
			t.m_w = 0;
			lookUp = mt * t;
			pomx = x;
			pomy = y;
	glutPostRedisplay();
	}
	else if(misRotacija > -1 && mode == 1)
	{

		Matrix4x4 mt, mx, my;
		Vector3D v;
		my.identity();
		mx.identity();
		v = lookAt - camera;
		v = lookUp.Cross(v);
		v.Normalize();
		if ( misRotacija == 1)
		{
			my.loadRotate(camera, v, (y - pomy)*0.0020f);
		}

		if (misRotacija == 2)
		{
			mx.loadRotate(camera, lookUp, (pomx - x)*0.0020f);
		}
		mt = mx * my;


		lookAt = mt * lookAt;
		Vector4D t = lookUp;
		t.m_w = 0;
		lookUp = mt * t;
		pomx = x;
		pomy = y;
		glutPostRedisplay();
	}

}
void mis(int klick,int state,int x,int y)
{
	switch (klick)
	{
	case GLUT_LEFT_BUTTON: if (state == GLUT_DOWN && e->igra_u_toku == 0) {
		misRotacija = 2;
		pomx = x;
		pomy = y;
		e->Proveri(x, y,0);
	}
						   else
	{
		misRotacija = -1;
		pomy = -1;
		pomx = -1;
	}break;
	case GLUT_RIGHT_BUTTON:if (state == GLUT_DOWN)
	{
		misRotacija = 1;
		pomy = y;
		pomx = x;
	}
						   else
	{
		misRotacija = -1;
		pomy = -1;
		pomx = -1;
	}break;



	default:
		break;
	}

	glutPostRedisplay();
}
void mouseWheel(int button, int dir, int x, int y)
{
	if (dir > 0)
	{
		if (zoom > 10.0)zoom -=10;
	}
	else
	{
		if (zoom < 80.0)zoom += 10;
	}

	glutPostRedisplay();
}




void moveJoystick(double x,double y)
{
	Matrix4x4 mt, mx, my;
	Vector3D v,g;
	my.identity();
	mx.identity();
	v = lookAt - camera;
	v = lookUp.Cross(v);
	v.Normalize();
	v *= x;
	g = lookAt - camera;
	g.Normalize();
	g *= y;
	mx.loadTranslate(v.m_x, v.m_y, v.m_z);
	my.loadTranslate(g.m_x, g.m_y, g.m_z);

	mt = mx * my;

	if ((camera.m_y + v.m_y > 0.2&&camera.m_y + v.m_y<9 && camera.m_x + v.m_x>-9 && camera.m_x + v.m_x<9 && camera.m_z + v.m_z>-9 && camera.m_z + v.m_z < 9)&& (camera.m_y + g.m_y > 0.2&&camera.m_y + g.m_y<9 && camera.m_x + g.m_x>-9 && camera.m_x + g.m_x<9 && camera.m_z + g.m_z>-9 && camera.m_z + g.m_z < 9))
	{
		int test = 0;
		for (int i = 0; i < e->tabla_n; i++)
			for (int j = 0; j < e->tabla_n; j++)
			{
				Vector3D d1((e->tabla[i][j].t1.m_x + e->tabla[i][j].t2.m_x) / 2, 0, (e->tabla[i][j].t1.m_z + e->tabla[i][j].t4.m_z) / 2);

				if (e->tabla[i][j].figura != NULL && e->tabla[i][j].figura->visina >= camera.m_y + v.m_y && (sqrt((camera.m_x + v.m_x - d1.m_x)*(camera.m_x + v.m_x - d1.m_x) + (camera.m_z + v.m_z - d1.m_z)*(camera.m_z + v.m_z - d1.m_z)) <= 0.6)) { test = 1; break; }
			}

		if (test == 0)
		{
			camera = mt * camera;
			lookAt = mt * lookAt;
		}
	}



}

void rotateCameraJoystick(double x, double y)
{
	Matrix4x4 mt, mx, my;
	Vector3D v;
	my.identity();
	mx.identity();
	v = lookAt - camera;
	v = lookUp.Cross(v);
	v.Normalize();

	my.loadRotate(camera, v, y*0.15f);



    mx.loadRotate(camera, lookUp, x*0.15f);
	mt = mx * my;


	lookAt = mt * lookAt;
	Vector4D t = lookUp;
	t.m_w = 0;
	lookUp = mt * t;

}

void timer(int)
{
	 joystick_con = glfwJoystickPresent(GLFW_JOYSTICK_1);

	if (joystick_con == 1)
	{
		
		int axesCount;
		const float *axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);
		int buttonCount;
		const unsigned char *buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);
		if (axes[0] != 0.0 || axes[1] != 0.0)moveJoystick(-axes[0], -axes[1]);
		if (axes[2] != 0.0 || axes[3] != 0.0)rotateCameraJoystick(-axes[2],axes[3]);
		if (buttons[4] == GLFW_PRESS)idiDole();
		if (buttons[5] == GLFW_PRESS)idiGore();

		if (buttons[13] == GLFW_PRESS && i_joystick < 7)i_joystick++;
		if (buttons[15] == GLFW_PRESS && i_joystick >0)i_joystick--;
		if (buttons[14] == GLFW_PRESS && j_joystick < 7)j_joystick++;
		if (buttons[16] == GLFW_PRESS && j_joystick > 0)j_joystick--;
		if (buttons[0] == GLFW_PRESS)e->Proveri(i_joystick, j_joystick,1);

		if (buttons[9] == GLFW_PRESS)
		{
			if (e->igra_u_toku == 1)
			{
				e->postaviFigure();
				e->igra_u_toku = 0;
				e->igrac_igra = 1;
				e->beli_sah = 0;
				e->crni_sah = 0;
				e->beli_broj_poteza = 0;
				e->crni_broj_poteza = 0;
				e->partija_broj++;
			}
		}


		if (buttons[7] == GLFW_PRESS)
		{
			if (zoom > 10.0)zoom -= 10;
		}
		if (buttons[6] == GLFW_PRESS)
		{
			if (zoom < 80.0)zoom += 10;
		}
	}
	glutPostRedisplay();
	glutTimerFunc(1000.0 / 60.0, timer, 0);
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE);
   glutInitWindowSize(WINDOW_HEIGHT * aspectRatio, WINDOW_HEIGHT);
   glutInitWindowPosition(50, 50);
   glutCreateWindow(title);
   
   glewInit();
   e=new Engine();

   e->createFrameBufferObject(WINDOW_HEIGHT * aspectRatio,WINDOW_HEIGHT);
 
   e->createTabla();
   e->pozadina.createWorld();
   glfwInit();
   glutTimerFunc(NULL, timer, 100);

   glutMouseFunc(mis);
   glutMotionFunc(Test);
   glutKeyboardFunc(KeyboardKeyPressed);
   glutMouseWheelFunc(mouseWheel);
   glutDisplayFunc(display);
   SetCamera();
   glutReshapeFunc(reshape);
   initGL();
   glutMainLoop();

   return 0;
}
