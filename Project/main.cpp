#include <iostream>

#include <Eigen/Geometry>
#include <Eigen/Eigen>
#include <Eigen/Dense>
#include"GeometryLibrary.hpp"

using namespace std;
using namespace Eigen;
using namespace GeometryLibrary;

/*
#include "Utils.hpp"


int main(int argc, char ** argv)
{
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}

*/



int main()


{

    string NameFile= "DFN/FR3_data.txt";
    vector<Fracture> fractures;
    fractures=readDFN(NameFile);


    vector<Trace> Traces;
    Traces=computeTraces(fractures);





    string outputFileName="Traces.txt";
    vector<vector<Support>> FractureTraces = writeResult(outputFileName,Traces);



    PolygonalMesh mesh;
    mesh=createMesh(fractures,mesh,FractureTraces);







  return 0;
}


