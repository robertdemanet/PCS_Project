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

// Funzione per leggere il DFN
vector<Fracture> readDFN(const string& filename);


Vector3d computeCentroid(Fracture fracture);

bool testCircumference(Fracture fracture1,
                       Fracture fracture2);

vector<Trace> computeTraces(const string& filename,
                            vector<Fracture>& fractures);


}

