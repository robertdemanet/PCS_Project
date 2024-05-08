#include "GeometryLibrary.hpp"

#include <Eigen/Eigen>
#include <Eigen/Geometry>
#include <Eigen/Dense>
#include <fstream>
#include <vector>

using namespace std;
using namespace Eigen;

namespace GeometryLibrary{
//****************************************************************************************************************
vector<Trace> computeTraces(const string& filename,
                            vector<Fracture>& fractures)
{
    for (int i = 0; i < fractures.size(); ++i) {
        for (int j = i + 1; j < fractures.size(); ++j) {
            if (testCircumference(fractures[i], fractures[j]))
            {
                Vector3d u1 = fractures[i].vertices.col(2) - fractures[i].vertices.col(0);
                Vector3d v1 = fractures[i].vertices.col(1) - fractures[i].vertices.col(0);
                Vector3d norm1 = (u1.cross(v1)).normalized();
                Vector3d u2 = fractures[j].vertices.col(2) - fractures[j].vertices.col(0);
                Vector3d v2 = fractures[j].vertices.col(1) - fractures[j].vertices.col(0);
                Vector3d norm2 = (u2.cross(v2)).normalized();
                // Vector3d norm1 = ((fractures[i].vertices.col(2) - fractures[i].vertices.col(0)).cross(fractures[i].vertices.col(1) - fractures[i].vertices.col(0))).normalized();
                // Vector3d norm2 = ((fractures[j].vertices.col(2) - fractures[j].vertices.col(0)).cross(fractures[j].vertices.col(1) - fractures[j].vertices.col(0))).normalized();
                Vector3d tangent = norm1.cross(norm2);
                Matrix3d A;
                A << norm1, norm2, tangent;
                Vector3d b;
            }

        }

    }
}














//****************************************************************************************************************
bool testCircumference(Fracture fracture1,
                       Fracture fracture2)
{
    Vector3d centroid1 = computeCentroid(fracture1);
    Vector3d centroid2 = computeCentroid(fracture2);
    double radius1 = 0.00;
    double radius2 = 0.00;

    for (int v = 0; v < fracture1.numVertices; ++v)
    {
        Vector3d vertex1(fracture1.vertices(0,v), fracture1.vertices(1,v), fracture1.vertices(2,v));
        Vector3d diff1 = centroid1 - vertex1;
        double distance1 = diff1.norm();
        radius1 = std::max(radius1, distance1);
    }


    for (int v = 0; v < fracture2.numVertices; ++v)
    {
        Vector3d vertex2(fracture2.vertices(0,v), fracture2.vertices(1,v), fracture2.vertices(2,v));
        Vector3d diff2 = centroid2 - vertex2;
        double distance2 = diff2.norm();
        radius2 = std::max(radius2, distance2);
    }

    Vector3d diff_centroids = centroid1 - centroid2;
    double distance_centroids = diff_centroids.norm();

    if(distance_centroids > (radius1 + radius2))
    {
        return false;
    }
    else
    {
        return true;
    }
}

//****************************************************************************************************************
Vector3d computeCentroid(Fracture fracture)
{
    Vector3d centroid;
    double sum_x = 0;
    double sum_y = 0;
    double sum_z = 0;
    int n_vert = fracture.numVertices;
    for (int j = 0; j < n_vert; ++j)
    {
        sum_x = sum_x + fracture.vertices(0,j);
        sum_y = sum_y + fracture.vertices(1,j);
        sum_z = sum_z + fracture.vertices(2,j);
    }

    double median_x = sum_x/n_vert;
    double median_y = sum_y/n_vert;
    double median_z = sum_z/n_vert;

    centroid << median_x, median_y, median_z;
    return centroid;
}

//****************************************************************************************************************
vector<Fracture> readDFN(const string &filename) {
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: it is impossible to open the file" << endl;
        exit(1);
    }

    string line;
    getline(file,line); // header of numFractures

    getline(file,line); // line of numFractures
    int numFractures;
    istringstream convertNF(line);
    convertNF >> numFractures;

    vector<Fracture> fractures;
    for (int i = 0; i < numFractures; ++i) {
        Fracture fracture;

        getline(file,line); // header of fractureID and numVertices
        getline(file,line); // line of fractureID and numVertices
        char b;
        istringstream convertIDandVERT(line);
        convertIDandVERT >> fracture.id >> b >> fracture.numVertices;


        getline(file,line); // header of vertices
        for (int j = 0; j < 3; ++j) {
            getline(file,line); // line of x/y/z coordinates
            char d;
            istringstream convertCOORD(line);
            for (int k = 0; k < fracture.numVertices; ++k) {
                if(k != fracture.numVertices - 1){
                    convertCOORD >> fracture.vertices(j,k) >> d;
                }
                else{
                    convertCOORD >> fracture.vertices(j,k);
                }
            }

        fractures.push_back(fracture);
        }
    }

    file.close();
    return fractures;
}
//****************************************************************************************************************
}



















