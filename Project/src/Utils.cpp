#include "GeometryLibrary.hpp"

#include <fstream>
#include <vector>

namespace GeometryLibrary{
//****************************************************************************************************************
bool testCircumference(vector<vector<double>> fracture1,
                       vector<vector<double>> fracture2)
{
    Vector3d centroid1 = computeCentroid(fracture1);
    Vector3d centroid2 = computeCentroid(fracture2);
    unsigned int numVertices1 = fracture1.size();
    unsigned int numVertices2 = fracture2.size();
    double radius1 = 0.00;
    double radius2 = 0.00;
    vector<double> distances1;
    vector<double> distances2;

    for (unsigned int v = 0; v < numVertices1; ++v)
    {
        Eigen::Vector3d vertex1(fracture1[v][0], fracture1[v][1], fracture1[v][2]);
        Vector3d diff1 = centroid1 - vertex1;
        double distance1 = diff1.norm();
        distances1.push_back(distance1);
        radius1 = std::max(radius1, distance1);
    }


    for (unsigned int v = 0; v < numVertices2; ++v)
    {
        Eigen::Vector3d vertex2(fracture2[v][0], fracture2[v][1], fracture2[v][2]);
        Vector3d diff2 = centroid2 - vertex2;
        double distance2 = diff2.norm();
        distances2.push_back(distance2);
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
Vector3d computeCentroid(vector<vector<double>> singularListVertices)
{
    unsigned int numVertices = singularListVertices.size();
    Vector3d centroid;
    double sum_x = 0;
    double sum_y = 0;
    double sum_z = 0;
    for (unsigned int j = 0; j < numVertices; ++j)
    {
        for (unsigned int d = 0; d < 3; ++d)
        {
            if(d = 0)
            {
                sum_x = sum_x + singularListVertices[j][d];
            }

            if(d = 1)
            {
                sum_y = sum_y + singularListVertices[j][d];
            }

            if(d = 2)
            {
                sum_z = sum_z + singularListVertices[j][d];
            }
        }
    }

    double median_x = sum_x/numVertices;
    double median_y = sum_y/numVertices;
    double median_z = sum_z/numVertices;

    centroid << median_x, median_y, median_z;
    return centroid;
}

//****************************************************************************************************************
void importFracturesList(const string& filepath,
                         Fractures &fractures)
{
    ifstream file(filepath);

    if(file.fail())
        throw runtime_error("cannot open file");

    string line;
    getline(file, line); // header

    getline(file, line);
    unsigned int numFractures;
    istringstream convertNF(line);
    convertNF >> numFractures;

    fractures.listVertices.resize(numFractures);
    for (unsigned int c = 0; c < numFractures; ++c)
    {
        getline(file, line); // header

        unsigned int id;
        unsigned int numVertices;
        char b;
        getline(file,line);

        istringstream convertNV(line);
        convertNV >> id >> b >> numVertices;

        fractures.listVertices[c].resize(numVertices);

        getline(file, line); // header
        VectorXd x_coordinates;
        x_coordinates.resize(numVertices);
        VectorXd y_coordinates;
        y_coordinates.resize(numVertices);
        VectorXd z_coordinates;
        z_coordinates.resize(numVertices);

        getline(file,line);
        istringstream convertX(line);
        char p;
        for (int i = 0; i < numVertices; ++i)
        {
            if(i != numVertices - 1)
            {
                convertX >> x_coordinates[i] >> p;
            }
            else
            {
                convertX >> x_coordinates[i];
            }
        }

        getline(file,line);
        istringstream convertY(line);
        char s;
        for (int i = 0; i < numVertices; ++i)
        {
            if(i != numVertices - 1)
            {
                convertY >> y_coordinates[i] >> s;
            }
            else
            {
                convertY >> y_coordinates[i];
            }
        }

        getline(file,line);
        istringstream convertZ(line);
        char a;
        for (int i = 0; i < numVertices; ++i)
        {
            if(i != numVertices - 1)
            {
                convertZ >> z_coordinates[i] >> a;
            }
            else
            {
                convertZ >> z_coordinates[i];
            }
        }

        fractures.listVertices[c].resize(numVertices);
        for (int v = 0; v < numVertices; ++v)
        {
            fractures.listVertices[c][v].resize(3);
            fractures.listVertices[c][v][0] = x_coordinates[v];
            fractures.listVertices[c][v][1] = y_coordinates[v];
            fractures.listVertices[c][v][2] = z_coordinates[v];
        }

    }
    file.close();
}
//****************************************************************************************************************
}



















