#pragma once

#include <iostream>
#include "Eigen/Eigen"

using namespace std;
using namespace Eigen;

namespace GeometryLibrary {

// Struct per rappresentare una singola frattura
struct Fracture {
    int id;
    int numVertices;
    MatrixXd vertices;
};

struct Traces {
    // Matrice di coordinate dei punti di inizio e fine delle tracce
    MatrixXd PointsCoordinates;
    // Vettore di coppie di indici delle fratture che definiscono le tracce
    std::vector<std::pair<unsigned int, unsigned int>> FractureIndices;

    Traces() = default;
    Traces(const MatrixXd& PointsCoordinates,
           const vector<pair<unsigned int, unsigned int>>& FractureIndices):
        PointsCoordinates(PointsCoordinates),
        FractureIndices(FractureIndices)
    {}
};

// Funzione per leggere il DFN
vector<Fracture> readDFN(const string& filename);


Vector3d computeCentroid(Fracture fracture);

bool testCircumference(Fracture fracture1,
                       Fracture fracture2);


}

