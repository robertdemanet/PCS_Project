#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>

namespace PolygonalLibrary {
bool ImportMesh(const string& filepath,
                PolygonalMesh& mesh)
{
    if(!ImportCell0Ds(filepath + "/Cell0Ds.csv",
                       mesh))
    {
        return false;
    }
    else
    {
        cout << "Cell0D marker:" << endl;
        for(auto it = mesh.Cell0DMarkers.begin(); it != mesh.Cell0DMarkers.end(); it++)
        {
            cout << "key:\t" << it -> first << "\t values:";
            for(const unsigned int id : it -> second)
                cout << "\t" << id;

            cout << endl;
        }
    }

    if(!ImportCell1Ds(filepath + "/Cell1Ds.csv",
                       mesh))
    {
        return false;
    }
    else
    {
        cout << "Cell1D marker:" << endl;
        for(auto it = mesh.Cell1DMarkers.begin(); it != mesh.Cell1DMarkers.end(); it++)
        {
            cout << "key:\t" << it -> first << "\t values:";
            for(const unsigned int id : it -> second)
                cout << "\t" << id;

            cout << endl;
        }
    }

    if(!ImportCell2Ds(filepath + "/Cell2Ds.csv",
                       mesh))
    {
        return false;
    }
    else
    {
        cout << "Cell2D marker:" << endl;
        for(auto it = mesh.Cell2DMarkers.begin(); it != mesh.Cell2DMarkers.end(); it++)
        {
            cout << "key:\t" << it -> first << "\t values:";
            for(const unsigned int id : it -> second)
                cout << "\t" << id;

            cout << endl;
        }

        for(unsigned int c = 0; c < mesh.NumberCell2D; c++)
        {
            vector<unsigned int> edges = mesh.Cell2DEdges[c];

            std::vector<double> vecX;
            std::vector<double> vecY;

            unsigned int n=edges.size();
            vecX.reserve(2*n);
            vecY.reserve(2*n);



            for(unsigned int e = 0; e < n; e++)
            {
                const unsigned int origin = mesh.Cell1DVertices[edges[e]][0];
                const unsigned int end = mesh.Cell1DVertices[edges[e]][1];

                const double Xorigin=mesh.Cell0DCoordinates[origin][0];
                const double Yorigin=mesh.Cell0DCoordinates[origin][1];
                const double Xend=mesh.Cell0DCoordinates[end][0];
                const double Yend=mesh.Cell0DCoordinates[end][1];

                vecX.push_back(Xorigin);
                vecX.push_back(Xend);
                vecY.push_back(Yorigin);
                vecY.push_back(Yend);



                double machine_epsilon = std::numeric_limits<double>::epsilon();
                if(!fabs(origin-end) > machine_epsilon)
                {
                    cout << "The edge number " << edges[e] << " has lenght zero" << endl;
                }

                auto findOrigin = find(mesh.Cell2DVertices[c].begin(), mesh.Cell2DVertices[c].end(), origin);
                if (findOrigin == mesh.Cell2DVertices[c].end())
                {
                    cerr << "Wrong mesh" << endl;
                    return 2;
                }

                auto findEnd = find(mesh.Cell2DVertices[c].begin(), mesh.Cell2DVertices[c].end(), end);
                if(findEnd == mesh.Cell2DVertices[c].end())
                {
                    cerr << "Wrong mesh" << endl;
                    return 3;
                }

                unsigned int z=vecX.size();
                double Area=0.0;
                for(unsigned int i=0;i<z;i++)
                {
                    Area= Area+ 0.5*fabs(vecX[i]*vecY[i+1]-vecX[i+1]*vecY[i]);

                }
                if (Area<machine_epsilon)
                    cout<<"la cella"<<" "<<c<<" "<<" tra le celle 2D ha area nulla"<<endl;
            }


        }

    }

    return true;
}


// *****************************************************************************

bool ImportCell0Ds(const string &filename,
                   PolygonalMesh& mesh)
{

    ifstream file;
    file.open(filename);

    if(file.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(file,line))
        listLines.push_back(line);

    file.close();

    listLines.pop_front();

    mesh.NumberCell0D = listLines.size();

    if (mesh.NumberCell0D == 0)
    {
        cerr << "There is no cell 0D" << endl;
        return false;
    }

    mesh.Cell0DId.reserve(mesh.NumberCell0D);
    mesh.Cell0DCoordinates.reserve(mesh.NumberCell0D);

    for (const string& line : listLines)
    {
        istringstream converter(line);

        char c;
        unsigned int id;
        unsigned int marker;
        Vector2d coord;

        converter >> id >> c >> marker >> c >> coord(0) >> c >> coord(1);

        mesh.Cell0DId.push_back(id);
        mesh.Cell0DCoordinates.push_back(coord);

        if (marker != 0)
        {
            auto ret = mesh.Cell0DMarkers.insert({marker, {id}});
            if(!ret.second)
                (ret.first)->second.push_back(id);
        }

    }

    file.close();

    return true;

}


// *****************************************************************************


bool ImportCell1Ds(const string &filename,
                   PolygonalMesh& mesh)
{

    ifstream file;
    file.open(filename);

    if(file.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(file,line))
        listLines.push_back(line);

    listLines.pop_front();

    mesh.NumberCell1D = listLines.size();

    if (mesh.NumberCell1D == 0)
    {
        cerr << "There is no cell 1D" << endl;
        return false;
    }

    mesh.Cell0DId.reserve(mesh.NumberCell1D);
    mesh.Cell1DVertices.reserve(mesh.NumberCell1D);



    for (const string& line : listLines)
    {
        istringstream converter(line);

        char c;
        unsigned int id;
        unsigned int marker;
        Vector2i vertices;

        converter >> id >> c >> marker >> c >> vertices(0) >> c >> vertices(1);

        mesh.Cell1DId.push_back(id);
        mesh.Cell1DVertices.push_back(vertices);

        if (marker != 0)
        {
            auto ret = mesh.Cell1DMarkers.insert({marker, {id}});
            if(!ret.second)
                (ret.first)->second.push_back(id);
        }
    }

    file.close();

    return true;
}


// *****************************************************************************


bool ImportCell2Ds(const string &filename,
                   PolygonalMesh &mesh)
{

    ifstream file;
    file.open(filename);

    if(file.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(file,line))
        listLines.push_back(line);

    listLines.pop_front();

    mesh.NumberCell2D = listLines.size();

    if (mesh.NumberCell2D == 0)
    {
        cerr << "There is no cell 2D" << endl;
        return false;
    }

    mesh.Cell2DId.reserve(mesh.NumberCell2D);
    mesh.Cell2DVertices.reserve(mesh.NumberCell2D);
    mesh.Cell2DEdges.reserve(mesh.NumberCell2D);

    for (const string& line : listLines)
    {
        istringstream converter(line);

        char c;
        unsigned int id;
        vector<unsigned int> vertices;
        vector<unsigned int> edges;
        unsigned int marker;
        unsigned int numVertices;
        unsigned int numEdges;

        converter >> id >> c >> marker >> c >> numVertices;
        for(unsigned int i = 0; i < numVertices; i++)
            converter >> vertices[i] >> c;
        for(unsigned int i = 0; i < numEdges-1; i++)
            converter >> edges[i] >> c;
        converter >> edges[numEdges-1];

        mesh.Cell2DId.push_back(id);
        mesh.Cell2DVertices.push_back(vertices);
        mesh.Cell2DEdges.push_back(edges);

        if (marker != 0)
        {
            auto ret = mesh.Cell2DMarkers.insert({marker, {id}});
            if(!ret.second)
                (ret.first)->second.push_back(id);
        }

    }
    file.close();
    return true;

}















}
