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
    vector<Vector3d> vertex_Inters1;
    vector<Vector3d> vertex_Inters2;


   /* bool operator==(const Trace& other) const {
        return id == other.id &&
               Fracture1ID == other.Fracture1ID &&
               Fracture2ID == other.Fracture2ID &&
               firstPoint == other.firstPoint &&
               finalPoint == other.finalPoint;
    }*/
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
                               Fracture& fracture1);

vector<double> compareAlphas(double& alpha1,double& alpha2,double& alpha3,double& alpha4);
bool compareLenght(const Support& Support1,const Support& Support2);


vector<vector<Support>> writeResult(const string& outputFilePath, // le posizioni del vector più esterno corrispondono all'id della frattura
                                    vector<Trace>& Traces);              // per ogni frattura accedo al vector di struct Support e poi alla singola
                                                                  // Support

bool writeTracesForFracture(const string& outputFilePath,
                            vector<vector<Support>>& FractureTraces);


// Seconda parte //////////

// CELLE 2D
struct Cell2D{
    int numIDvertices;
    int numIDedges;
    vector<unsigned int> IDs_vertices;
    vector<unsigned int> IDs_edges;
    bool status;
} ;





struct PolygonalMesh{
    //CELLE 0D
    vector<unsigned int> Cell0Id={};
    vector<Vector3d> Cell0dCoordinates={};

    //CELLE 1D
    map<unsigned int,array< int,2>> Cell1;//sia id del lato sia ids dei vertici

    // CELLE 2D
    vector<Celle2D> vecCell2D;

};

PolygonalMesh createMesh(vector<Fracture>& fractures,PolygonalMesh& mesh);


}

