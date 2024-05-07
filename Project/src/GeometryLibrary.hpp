#pragma once

#include <iostream>
#include "Eigen/Eigen"

using namespace std;
using namespace Eigen;

namespace GeometryLibrary {

struct Fractures {
    vector<vector<vector<double>>> listVertices;

    Fractures() = default;
    Fractures(const vector<vector<vector<double>>>& listVertices) :
        listVertices(listVertices)
    {}
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


void importFracturesList(const string& filepath,
                        Fractures& fractures);


Vector3d computeCentroid(vector<vector<double>> singularListVertices);

bool testCircumference(vector<vector<double>> fracture1,
                       vector<vector<double>> fracture2);


}

