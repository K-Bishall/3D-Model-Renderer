#include <iostream>
#include <cstring>
#include <stdio.h>

using std::cout;
using std::endl;

#include "Model.h"
#include "Vector3d.h"
#include "Face.h"

Model::Model()
{}

//-----BEFORE CALLING loadModel CALL setViewCoordinate-----//
/**
 * @brief Function to load obj file for the model to render
 * Constructs vertex table and face table.
 * It also transforms World Coordinate vertex to View Coordinate
 * as soon as the vertex is read.
 * @param Path to obj file
 * @return File success or fail to read
 */
bool Model::loadModel(const char* modelPath){

    vertexCount=0,normalCount=0,faceCount=0;
    Vector3d tempV, tempN;
    Vector3d tempViewVertex;
    Face tempFace;

    FILE * file = fopen(modelPath, "r");
    if( file == NULL ){
        std::cerr<<"File cannot open"<<endl;
        return false;
    }

    while(1){
        char lineHeader[1024];

        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break;

        if (strcmp(lineHeader, "v" ) == 0 ){
            fscanf(file, "%f %f %f\n", &tempV.x, &tempV.y, &tempV.z);

            vertexTable.push_back(tempV);
            vertexCount++;
        }

        //'vn' is normal vector to the vertex and may not be unit vectors
        else if(strcmp(lineHeader, "vn")==0){
            fscanf(file, "%f %f %f\n", &tempN.x, &tempN.y, &tempN.z);
            normalTable.push_back(tempN);
            normalCount++;
        }

        //'f' for faces
        else if(strcmp(lineHeader,"f")==0){
            int v1I,v2I,v3I;
            int n1I, n2I,n3I;
            fscanf(file,"%d//%d %d//%d %d//%d", &v1I,&n1I,&v2I,&n2I,&v3I,&n3I);

            tempFace = Face(v1I-1,v2I-1,v3I-1, n1I-1,n2I-1,n3I-1);
            faceTable.push_back(tempFace);
            faceCount++;
        }

    }
    fclose(file);
    return true;
}


void Model::printData()
{
    int i;
    for(i = 0; i< vertexCount; i++)
    {
        cout<<"v ";
        vertexTable[i].printData();
        cout<<endl;
    }

    for(i = 0; i< normalCount; i++)
    {
        cout<<"vn ";
        normalTable[i].printData();
        cout<<endl;
    }
/*
    for(i = 0; i< faceCount; i++)
    {
        cout<<"f ";
        faceTable[i].vn1.printData();
        cout<<" ";
        faceTable[i].vn2.printData();
        cout<<" ";
        faceTable[i].vn2.printData();

        cout<<endl;
    }
*/
}

