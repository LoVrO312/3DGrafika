#include<iostream>
#include<cmath>
#include<fstream>

using namespace std;

void generateCircleVertices(ofstream& file, double height, double radius, int detail){
    double angle = 360.0 / detail;
    double x = radius;
    double y = 0;

    for(int i=0; i<detail; i++){
        x = radius * sin(i * angle * M_PI / 180);
        y = radius * cos(i * angle * M_PI / 180);
        file << "v " << x << " " << y << " " << height << endl;
    }
}

void generateSideNorms(ofstream& file, int detail){
    double angle = 360.0 / detail;
    double nx = 1;
    double ny = 0;

    for(int i=0; i<detail; i++){
        nx = sin(i * angle * M_PI / 180);
        ny = cos(i * angle * M_PI / 180);
        file << "vn " << nx << " " << ny << " 0" << endl;
    }
}

void generateSideFaces(ofstream& file, int firstCircleIndex, int secondCircleIndex, int normIndex){
    for(int i=0; i<secondCircleIndex-2; i++){
        file << "f " << firstCircleIndex + i<< "//" << normIndex + i<< " " << secondCircleIndex + i
             << "//" << normIndex + i << " " << firstCircleIndex + 1 + i << "//" << normIndex + 1 + i << endl;
    }
    file << "f 72//72 144/72 1//1" << endl;
    for(int i=0; i<secondCircleIndex-2; i++){
        file << "f " << secondCircleIndex + i<< "//" << normIndex + i<<  " " << secondCircleIndex + 1 + i << "//" << normIndex + 1 + i << " " << firstCircleIndex + 1 + i
             << "//" << normIndex + 1 + i <<endl;
    }
    file << "f 144//72 73//1 1//1" << endl;
}

void generateCircleFaces(ofstream& file, int detail, int firstVertexIndex, int normIndex){
    for(int i=firstVertexIndex; i<detail + firstVertexIndex - 2; i++){
        file << "f " << firstVertexIndex << "//" << normIndex << " " << i+1 << "//" << normIndex
             << " " <<  i+2 << "//" << normIndex << endl; 
    }
}

int main(){
    ofstream cylinder;
    cylinder.open("valjak.obj");

    int detail = 72;
    
    generateCircleVertices(cylinder, 0, 1, detail);
    generateCircleVertices(cylinder, 2, 1, detail);

    // cylinder side
    generateSideNorms(cylinder, detail);
    generateSideFaces(cylinder, 1, detail+1, 1);

    cylinder << "vn 0 0 -1" << endl;    // normala donjeg kruga
    cylinder << "vn 0 0 -1" << endl;     // normala gornjeg kruga
    generateCircleFaces(cylinder, detail, 1, 73);
    generateCircleFaces(cylinder, detail, detail+1, 74);
    
    

    cylinder.close();
    return 0;
}