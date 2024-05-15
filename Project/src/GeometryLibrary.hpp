#pragma once

#include <iostream>
#include <Eigen/Geometry>
#include <Eigen/Eigen>
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

namespace GeometryLibrary {

// Struct per rappresentare una singola frattura
struct Fracture {
    int id;
    int numVertices;
    MatrixXd vertices;
};

// Struct per rappresentare una singola traccia
struct Trace {
    int id;
    int Fracture1ID;
    int Fracture2ID;
    Vector3d firstPoint;
    Vector3d finalPoint;
};

struct Support{
    int NumTraces;
    int idT;
    bool Tips;
    double lenght;
};

// Funzione per leggere il DFN
vector<Fracture> readDFN(const string& filename);


Vector3d computeCentroid(Fracture& fracture);

bool testCircumference(Fracture& fracture1,
                       Fracture& fracture2);

vector<Trace> computeTraces(vector<Fracture>& fractures);

vector<Vector3d> TraceVertexes(Vector3d& Point1,
                               Vector3d& Point2,
                               Fracture& fracture1,
                               Fracture& fracture2);

bool comparePoints(const Vector3d& v1,const Vector3d& v2);
bool compareLenght(const Support& Support1,const Support& Support2);


vector<vector<Support>> writeResult(const string& outputFilePath, // le posizioni del vector più esterno corrispondono all'id della frattura
                            vector<Trace>& Traces,                // per ogni frattura accedo al vector di struct Support e poi alla singola
                                    vector<Vector3d>& vertex_Inters);   // Support

bool writeTracesForFracture(const string& outputFilePath,
                            vector<vector<Support>>& FractureTraces);






}

