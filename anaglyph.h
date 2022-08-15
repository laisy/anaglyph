#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <GL/glut.h>

typedef struct {
   double x,y,z;
} XYZ;


#define DTOR            0.0174532925
#define PI              3.141592653589793238462643
#define ESC 27
#define CROSSPROD(p1,p2,p3) \
   p3.x = p1.y*p2.z - p1.z*p2.y; \
   p3.y = p1.z*p2.x - p1.x*p2.z; \
   p3.z = p1.x*p2.y - p1.y*p2.x

typedef struct {
   XYZ vp;              /* Ver posição             */
   XYZ vd;              /* Ver vetor de direção    */
   XYZ vu;              /* Ver direção para cima   */
   XYZ pr;              /* Ponto para girar sobre  */
   double tamfocal;     /* Distancia focal         */
   double abertura;     /* Abertura da camera      */
   double olhosep;      /* Separação dos olhos     */
   int alturatela,larguratela;
} CAMERA;

void Exibir(void);
void CriarAmbiente(void);
void CriarMundo(void);
void ManipularTeclado(unsigned char key,int x, int y);
void ManipularTecladoEspecial(int key,int x, int y);
void ManipularVisibilidade(int vis);
void Remodelar(int,int);
void Temporizador(int);
void RotacionarCamera(int,int,int);
void TransladarCamera(int,int);
void Home();
void Normalizar(XYZ *);
XYZ  CalcNormal(XYZ,XYZ,XYZ);
