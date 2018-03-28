//rdPly.h

//Reads in Ply files and returns 2 arrays:
//
//an array of glm::vec4s
//and an array of faces, a custom variable
#ifndef RDPLY_H
#define RDPLY_H

#include <fstream>//for ifstream
#include <glm/glm.hpp>//for vec4
/*
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
*/
//========================================
//g++ rdply.cpp drawFile.cpp -o a.x -lGL -lm -lGLU -lglut

struct Face{
  int a;
  int b;
  int c;
};

glm::vec4* verts();

struct Face* faces();

int vert_Total();
int face_Total();

//int vert_Total;
//int face_Total;

int getData(char *fname);
//int getData(std::ifstream);

//TODO Make Structs that hold all the data for shit
/*struct faceDate{
  struct Face *f;
  int face_Total;
}*/
#endif
