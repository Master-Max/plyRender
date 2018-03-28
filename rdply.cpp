//g++ rdply.cpp drawFile.cpp -o a.x -lGL -lm -lGLU -lglut
//-lGL -lm -lGLU -lglut
#include "rdply.h"
/*
#include <vector>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
*/
#include <string>
#include <iostream>
#include <sstream>

int vertTotal;
int faceTotal;

int vert_Total(){
  return vertTotal;
}
int face_Total(){
  return faceTotal;
}

glm::vec4 *v;
struct Face *f;

glm::vec4* verts(){
  return v;
}
struct Face* faces(){
  return f;
}

int vert_Count = 0;
int face_Count = 0;

int getData(char *fname)
{
  std::string file(fname);
  std::ifstream infile(file.c_str(), std::ifstream::in);
  std::string line;

  float x,y,z;
  int n,a,b,c;
  int header = 1;
  std::string stA, stB;

  while(std::getline(infile, line))
  {
    std::istringstream iss(line);

    if(header){//Reads the header, stops when it sees "end_headers"
      std::cout << line << "\n";
      if(line.find("element vertex") != std::string::npos){
        if(iss >> stA >> stB >> vertTotal){
          v = new glm::vec4[vertTotal];
          std::cout << "=== Created Vertex Array\n";
        }else{
          printf("Error Creating Vertex Array\n");
          return 0;
        }
      }
      else if(line.find("element face") != std::string::npos){
        if(iss >> stA >> stB >> faceTotal){
          f = new struct Face[faceTotal];
          std::cout << "=== Created Face Array\n";
        }else{
          printf("Error Creating Faces Array\n");
          return 0;
        }
      }
      else if(line.find("end_header") != std::string::npos){
        printf("=== Ending Header\n");
        printf("VTotal : %d\n", vertTotal);
        printf("FTotal : %d\n", faceTotal);
        std::cout<< "================\n";
        printf("VCount : %d\n", vert_Count);
        printf("FCount : %d\n", face_Count);
        header = 0;
      }
    }
    else if(!header){
      if(vert_Count < vertTotal){
        iss >> x >> y >> z;
        v[vert_Count] = glm::vec4(x,y,z,1.0f);
        vert_Count++;
      }
      else if(face_Count < faceTotal){
        iss >> n >> a >> b >> c;
        struct Face tmpF = {a,b,c};
        f[face_Count] = tmpF;
        face_Count++;
      }
    }
  }
  infile.close();
  return 1;
}
