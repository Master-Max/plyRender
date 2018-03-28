//g++ rdply.cpp drawFile.cpp -o a.x -lGL -lm -lGLU -lglut
#include "rdply.h"
/*

#include <fstream>
#include <sstream>

#include <vector>
*/
#include <iostream>
#include <string>
#include <GL/glut.h>
//#include <glm/glm.hpp>
#include <glm/ext.hpp>


int dbg;//DEBUG thing

struct Cord3{
  float x,y,z;
};

struct Cord3 Centroid;

glm::vec4 *VERTS;
struct Face *FACES;

//glm::mat4 TraMaster;//TODO Remove This?

glm::vec4 Translate(glm::vec4 v, float Dx, float Dy, float Dz){
  float tmpT[16] = {
    1.0f, 0.0f, 0.0f, Dx,
    0.0f, 1.0f, 0.0f, Dy,
    0.0f, 0.0f, 1.0f, Dz,
    0.0f, 0.0f, 0.0f, 1.0f};
  glm::mat4 traMx = glm::make_mat4(tmpT);
  glm::vec4 result = v * traMx;
  return result;
};

glm::vec4 Scale(glm::vec4 v, float Sx, float Sy, float Sz){
  float tmpS[16] = {
    Sx, 0.0f, 0.0f, 0.0f,
    0.0f, Sy, 0.0f, 0.0f,
    0.0f, 0.0f, Sz, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f};
  glm::mat4 scaMx = glm::make_mat4(tmpS);
  glm::vec4 result = v * scaMx;
  //glm::vec4 result = scaMx * v;
  return result;
};

glm::vec4 Rotate(glm::vec4 v, char Plane, float R){
  glm::mat4 rotMx;
  if(Plane == 'x'){
    float xRot[16] = {
      1.0f, 0.0f, 0.0f, 0.0f,
      0.0f, cos(R), -sin(R), 0.0f,
      0.0f, sin(R), cos(R), 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f};
    rotMx = glm::make_mat4(xRot);
  }
  else if(Plane == 'y'){
    float yRot[16] = {
      cos(R), 0.0f, sin(R), 0.0f,
      0.0f, 1.0f, 0.0f, 0.0f,
      -sin(R), 0.0f, cos(R), 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f};
    rotMx = glm::make_mat4(yRot);
  }
  else if(Plane == 'z'){
    float zRot[16] = {
      cos(R), -sin(R), 0.0f, 0.0f,
      sin(R), cos(R), 0.0f, 0.0f,
      0.0f, 0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f};
    rotMx = glm::make_mat4(zRot);
  }
  glm::vec4 result = v * rotMx;
  return result;
};

void findCentroid(){
  float Ox, Oy, Oz;
  Ox = 0.0f; Oy = 0.0f; Oz = 0.0f;
  for(int i = 0; i < vert_Total(); i++){
    Ox += VERTS[i][0];
    Oy += VERTS[i][1];
    Oz += VERTS[i][2];
  }
  Ox = Ox / vert_Total();
  Oy = Oy / vert_Total();
  Oz = Oz / vert_Total();
  printf("Finding Centroid: (%f,%f,%f)\n",Ox,Oy,Oz);
  Centroid.x = Ox; Centroid.y = Oy; Centroid.z = Oz;
}

void doTranslate(float Dx, float Dy, float Dz, int modCentroid){
  glm::vec4 tmpV;
  if(modCentroid){
    Centroid.x += Dx;//TODO Error Checking? Pre and post Transform Centroids?
    Centroid.y += Dy;
    Centroid.z += Dz;
  }

  for(int i = 0; i<vert_Total();i++){
    tmpV = VERTS[i];
    tmpV = Translate(tmpV, Dx, Dy, Dz);
    VERTS[i] = tmpV;
  }
}

void mvToOrigin(int mvC){
  float tmpX, tmpY, tmpZ;
  tmpX = -1.0f * Centroid.x;
  tmpY = -1.0f * Centroid.y;
  tmpZ = -1.0f * Centroid.z;
  doTranslate(tmpX,tmpY,tmpZ,mvC);//0 means Centroid is safe
}

void mvToCentroid(){
  doTranslate(Centroid.x,Centroid.y,Centroid.z,0);//0 means Centroid is safe
}

void doScale(float amt){
  glm::vec4 tmpV;
  mvToOrigin(0);//0 means it won't change the Centroid
  for(int i = 0; i<vert_Total();i++){
    tmpV = VERTS[i];
    tmpV = Scale(tmpV,amt,amt,amt);
    VERTS[i] = tmpV;
  }
  mvToCentroid();
}

void doRotate(char p, float rad){
  glm::vec4 tmpV;
  mvToOrigin(0);//0 means it won't change the Centroid
  for(int i = 0; i<vert_Total();i++){
    tmpV = VERTS[i];
    tmpV = Rotate(tmpV,p,rad);
    VERTS[i] = tmpV;
  }
  mvToCentroid();
}

float getInitScale(){
  float Biggie = 0.0f;
  float Smalls = 0.0f;

  for(int i=0;i<vert_Total();i++){
    for(int j=0;j<3;j++){
      if(VERTS[i][j] > Biggie)
        Biggie = VERTS[i][0];
      if(VERTS[i][j] < Smalls)
        Smalls = VERTS[i][0];
    }
  }
  Smalls = Smalls * -1.0f;
  if(Smalls > Biggie)
    return Smalls;
  else
    return Biggie;
}

void keyF(unsigned char key, int x, int y){
  printf("keypress: %d\n", key);

  switch(key){
    //TRANSLATE
    case 'w'://forward
      printf("Translate Forward\n");
      doTranslate(0.0f,0.1f,0.0f,1);
      break;
    case 's'://back
      printf("Translate Back\n");
      doTranslate(0.0f,-0.1f,0.0f,1);
      break;
    case 'a'://left
      printf("Translate Left\n");
      doTranslate(-0.1f,0.0f,0.0f,1);
      break;
    case 'd'://right
      printf("Translate Right\n");
      doTranslate(0.1f,0.0f,0.0f,1);
      break;
    case 'u'://up
      printf("Translate Up\n");
      doTranslate(0.0f,0.0f,0.1f,1);
      break;
    case 'j'://down
      printf("Translate Down\n");
      doTranslate(0.0f,0.0f,-0.1f,1);
      break;
    //ROTATE
    case '8'://rot x
      printf("Rotating by X\n");
      doRotate('x', 0.1f);
      break;
    case '6'://rot y
      printf("Rotating by Y\n");
      doRotate('y', 0.1f);
      break;
    case '4'://rot z
      printf("Rotating by Y\n");
      doRotate('z', 0.1f);
      break;
    //SCALE
    case '+'://increase
      printf("Scale Increase\n");
      doScale(1.01f);
      break;
    case '-'://decrease
      printf("Scale Decrease\n");
      doScale(0.9f);
      break;

    default:
      printf("Invalid Command\n");
  }
}

void render(){
  glClearColor(0.0, 0.0, 0.0, 0.0);//Black
  glClear(GL_COLOR_BUFFER_BIT);
  glBegin(GL_TRIANGLES);
  for(int i = 0; i < face_Total(); i++){
    float grn = (float) i/ (float)face_Total();//TODO
    glColor3f(1.0f,grn,0.0);
    glVertex3f(VERTS[FACES[i].a][0],VERTS[FACES[i].a][1],VERTS[FACES[i].a][2]);
    glVertex3f(VERTS[FACES[i].b][0],VERTS[FACES[i].b][1],VERTS[FACES[i].b][2]);
    glVertex3f(VERTS[FACES[i].c][0],VERTS[FACES[i].c][1],VERTS[FACES[i].c][2]);
  }
  glEnd();
  glutSwapBuffers();
}

void mloop(int v){
    render();
    glutTimerFunc(1000/60, mloop, v);
}

void printInf(std::string s){
  //std::string SS(s);
  if(dbg){
    //std::cout << SS;
    std::cout << s;
  }
}

int main(int argc, char **argv, char **envp){
  dbg = 1;//Turn Debugging on and off, ie the print statements
  printInf("Debugging is ON\n");

  if(argc != 2){
    printf("Usage: draw.x [file.ply]\n");
    return 1;
  }

  //std::string file(argv[1]);
  //std::ifstream infile(file.c_str(), std::ifstream::in);

  if(!getData(argv[1])){
    printf("Error loading file");
    return 0;
  }
  /*
  if(!getData(infile)){
    printf("Error loading file");
    return 0;
  }*/

  //infile.close();
  VERTS = verts();
  FACES = faces();

  findCentroid();
  mvToOrigin(1);//1 means it will change the Centroid

  float BIG = getInitScale();
  if(BIG > 1.0f){
    for(int i=0;i<vert_Total();i++){
      VERTS[i][0] = (VERTS[i][0]) / (BIG * 2.0f);
      VERTS[i][1] = (VERTS[i][1]) / (BIG * 2.0f);
      VERTS[i][2] = (VERTS[i][2]) / (BIG * 2.0f);
    }
  }

  glutInit(&argc, argv);
  glutInitWindowSize(750,750);
  glutCreateWindow("drawFile.cpp");
  glutKeyboardFunc(keyF);
  glutDisplayFunc(render);
  glutTimerFunc(1000/60, mloop, 0);
  glutMainLoop();
}
