#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <GL/glut.h>
#include "anaglyph.h"

/* REALIDADE VIRTUAL E AUMENTADA 2021.1
   LAISY CRISTINA
   ANAGLYPH
*/

/* COMANDOS LINUX

gcc anaglyph.c -o anaglyph -lGL -lglut -lm -lGLU -lOpenGL
ps aux | rg Xorg   
./anaglyph   

OBS: Gnome sobre Xorg
*/

/* VARIÁVEIS GLOBAIS */
int debug = 1;
int currentbutton = -1;
float rotatespeed = 0.5;    
double dtheta = 1.0;         /* Incremento do angulo de rotação da câmera */
CAMERA camera;
XYZ origin = {0.0,0.0,0.0};

int main(int argc,char **argv)
{
   glutInit(&argc,argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_ACCUM | GLUT_RGB | GLUT_DEPTH);

   glutCreateWindow("Anaglyph");
   camera.larguratela = 400;
   camera.alturatela = 300;
   glutReshapeWindow(camera.larguratela,camera.alturatela);
      
   glutDisplayFunc(Exibir);
   glutReshapeFunc(Remodelar);
   glutVisibilityFunc(ManipularVisibilidade);
   glutKeyboardFunc(ManipularTeclado);
   glutSpecialFunc(ManipularTecladoEspecial);
   
   CriarAmbiente();
   Home(0);

   glutMainLoop();
   return(0);
}

void CriarAmbiente(void)
{
   int num[2];

   glEnable(GL_DEPTH_TEST);
   glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
   glLineWidth(1.0);
   glPointSize(1.0);
   glClearColor(0.0,0.0,0.0,0.0);
   glClearAccum(0.0,0.0,0.0,0.0);
   glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
   glEnable(GL_COLOR_MATERIAL);
   glPixelStorei(GL_UNPACK_ALIGNMENT,1);
}

void Exibir(void)
{
   int i,j;
   XYZ r;
   double dist,ratio,radians,scale,wd2,ndfl;
   double left,right,top,bottom,near=0.1,far=10000;

   near = camera.tamfocal / 5;

   CROSSPROD(camera.vd,camera.vu,r);
   Normalizar(&r);
   r.x *= camera.olhosep / 3.0;
   r.y *= camera.olhosep / 3.0;
   r.z *= camera.olhosep / 3.0;

   ratio  = camera.larguratela / (double)camera.alturatela;
   radians = DTOR * camera.abertura / 2;
   wd2     = near * tan(radians);
   ndfl    = near / camera.tamfocal;

   glDrawBuffer(GL_BACK);
   glReadBuffer(GL_BACK);

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glViewport(0,0,camera.larguratela,camera.alturatela);

   glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
   glColorMask(GL_FALSE,GL_TRUE,GL_TRUE,GL_TRUE);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   left  = - ratio * wd2 + 0.5 * camera.olhosep * ndfl;
   right =   ratio * wd2 + 0.5 * camera.olhosep * ndfl;
   top    =   wd2;
   bottom = - wd2;
   glFrustum(left,right,bottom,top,near,far);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(camera.vp.x - r.x,camera.vp.y - r.y,camera.vp.z - r.z,
             camera.vp.x - r.x + camera.vd.x,
             camera.vp.y - r.y + camera.vd.y,
             camera.vp.z - r.z + camera.vd.z,
             camera.vu.x,camera.vu.y,camera.vu.z);
   CriarMundo();
   glFlush();
   glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE); 

   glAccum(GL_LOAD,1.0);

   glDrawBuffer(GL_BACK);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   left  = - ratio * wd2 - 0.5 * camera.olhosep * ndfl;
   right =   ratio * wd2 - 0.5 * camera.olhosep * ndfl;
   top    =   wd2;
   bottom = - wd2;
   glFrustum(left,right,bottom,top,near,far);

   glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
   glColorMask(GL_TRUE,GL_FALSE,GL_FALSE,GL_TRUE);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(camera.vp.x + r.x,camera.vp.y + r.y,camera.vp.z + r.z,
             camera.vp.x + r.x + camera.vd.x,
             camera.vp.y + r.y + camera.vd.y,
             camera.vp.z + r.z + camera.vd.z,
             camera.vu.x,camera.vu.y,camera.vu.z);
   CriarMundo();
   glFlush();
   glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE); 

   glAccum(GL_ACCUM,1.0);
   glAccum(GL_RETURN,1.0);
   glutSwapBuffers(); 
}

void CriarMundo(void)
{
   static double rotateangle = 0.0;

   glPushMatrix();
   glRotatef(rotateangle,0.0,1.0,0.0);

   glColor3f(1.0,0.5,1.2);
   glutSolidTorus(0.258, 0.50, 20, 30); // Criar imagem
   glutWireTorus(0.50, 0.50, 10, 20);

   glPopMatrix();
   rotateangle += rotatespeed;
}

void ManipularTeclado(unsigned char key,int x, int y)
{
   switch (key) {
   case ESC:                            
   case 'Q':
   case 'q': 
      exit(0); 
      break;
   case '[':                           
      RotacionarCamera(0,0,-1);
      break;
   case ']':                          
      RotacionarCamera(0,0,1);
      break;
   case 'i':                           
   case 'I':
      TransladarCamera(0,1);
      break;
   case 'k':                          
   case 'K':
      TransladarCamera(0,-1);
      break;
   case 'j':                           
   case 'J':
      TransladarCamera(-1,0);
      break;
   case 'l':                       
   case 'L':
      TransladarCamera(1,0);
      break;
   }
}

void ManipularTecladoEspecial(int key,int x, int y)
{
   switch (key) {
   case GLUT_KEY_LEFT:
      RotacionarCamera(-1,0,0);
      break;
   case GLUT_KEY_RIGHT:
      RotacionarCamera(1,0,0);
      break;
   case GLUT_KEY_UP:
      RotacionarCamera(0,1,0);
      break;
   case GLUT_KEY_DOWN:
      RotacionarCamera(0,-1,0);
      break;
   }
}

void RotacionarCamera(int ix,int iy,int iz)
{
   XYZ vp,vu,vd;
   XYZ right;
   XYZ newvp,newr;
   double radius,dd,radians;
   double dx,dy,dz;

   vu = camera.vu;
   Normalizar(&vu);
   vp = camera.vp;
   vd = camera.vd;
   Normalizar(&vd);
   CROSSPROD(vd,vu,right);
   Normalizar(&right);
   radians = dtheta * PI / 180.0;

   if (iz != 0) {
      camera.vu.x += iz * right.x * radians;
      camera.vu.y += iz * right.y * radians;
      camera.vu.z += iz * right.z * radians;
      Normalizar(&camera.vu);
      return;
   }

   dx = camera.vp.x - camera.pr.x;
   dy = camera.vp.y - camera.pr.y;
   dz = camera.vp.z - camera.pr.z;
   radius = sqrt(dx*dx + dy*dy + dz*dz);

   dd = radius * radians;
   newvp.x = vp.x + dd * ix * right.x + dd * iy * vu.x - camera.pr.x;
   newvp.y = vp.y + dd * ix * right.y + dd * iy * vu.y - camera.pr.y;
   newvp.z = vp.z + dd * ix * right.z + dd * iy * vu.z - camera.pr.z;
   Normalizar(&newvp);
   camera.vp.x = camera.pr.x + radius * newvp.x;
   camera.vp.y = camera.pr.y + radius * newvp.y;
   camera.vp.z = camera.pr.z + radius * newvp.z;

   newr.x = camera.vp.x + right.x - camera.pr.x;
   newr.y = camera.vp.y + right.y - camera.pr.y;
   newr.z = camera.vp.z + right.z - camera.pr.z;
   Normalizar(&newr);
   newr.x = camera.pr.x + radius * newr.x - camera.vp.x;
   newr.y = camera.pr.y + radius * newr.y - camera.vp.y;
   newr.z = camera.pr.z + radius * newr.z - camera.vp.z;

   camera.vd.x = camera.pr.x - camera.vp.x;
   camera.vd.y = camera.pr.y - camera.vp.y;
   camera.vd.z = camera.pr.z - camera.vp.z;
   Normalizar(&camera.vd);

   CROSSPROD(newr,camera.vd,camera.vu);
   Normalizar(&camera.vu);
}

void TransladarCamera(int ix,int iy)
{
   XYZ vp,vu,vd;
   XYZ right;
   XYZ newvp,newr;
   double radians,delta;

   vu = camera.vu;
   Normalizar(&vu);
   vp = camera.vp;
   vd = camera.vd;
   Normalizar(&vd);
   CROSSPROD(vd,vu,right);
   Normalizar(&right);
   radians = dtheta * PI / 180.0;
   delta = dtheta * camera.tamfocal / 90.0;

   camera.vp.x += iy * vu.x * delta;
   camera.vp.y += iy * vu.y * delta;
   camera.vp.z += iy * vu.z * delta;
   camera.pr.x += iy * vu.x * delta;
   camera.pr.y += iy * vu.y * delta;
   camera.pr.z += iy * vu.z * delta;

   camera.vp.x += ix * right.x * delta;
   camera.vp.y += ix * right.y * delta;
   camera.vp.z += ix * right.z * delta;
   camera.pr.x += ix * right.x * delta;
   camera.pr.y += ix * right.y * delta;
   camera.pr.z += ix * right.z * delta;
}

void ManipularVisibilidade(int visible)
{
   if (visible == GLUT_VISIBLE)
      Temporizador(0);
   else
      ;
}

void Temporizador(int value)
{
   glutPostRedisplay();
   glutTimerFunc(30,Temporizador,0);
}

void Remodelar(int w,int h)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glViewport(0,0,(GLsizei)w,(GLsizei)h);
   camera.larguratela = w;
   camera.alturatela = h;
}

void Home(int mode)
{
   camera.abertura = 60;
   camera.pr = origin;

   camera.vd.x = 1;
   camera.vd.y = 0;
   camera.vd.z = 0;

   camera.vu.x = 0;
   camera.vu.y = 1;
   camera.vu.z = 0;

   camera.vp.x = -10;
   camera.vp.y = 0;
   camera.vp.z = 0;

   switch (mode) {
   case 0:
   case 2:
   case 4:
      camera.tamfocal = 10;
      break;
   case 1:
      camera.tamfocal = 5;
      break;
   case 3:
      camera.tamfocal = 15;
      break;
   }
   camera.olhosep = camera.tamfocal / 30.0;
}

void Normalizar(XYZ *p)
{
   double length;

   length = sqrt(p->x * p->x + p->y * p->y + p->z * p->z);
   if (length != 0) {
      p->x /= length;
      p->y /= length;
      p->z /= length;
   } else {
      p->x = 0;
      p->y = 0;
      p->z = 0;
   }
}

XYZ CalcNormal(XYZ p,XYZ p1,XYZ p2)
{
   XYZ n,pa,pb;

   pa.x = p1.x - p.x;
   pa.y = p1.y - p.y;
   pa.z = p1.z - p.z;
   pb.x = p2.x - p.x;
   pb.y = p2.y - p.y;
   pb.z = p2.z - p.z;
   Normalizar(&pa);
   Normalizar(&pb);

   n.x = pa.y * pb.z - pa.z * pb.y;
   n.y = pa.z * pb.x - pa.x * pb.z;
   n.z = pa.x * pb.y - pa.y * pb.x;
   Normalizar(&n);

   return(n);
}
