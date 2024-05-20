#include <iostream>

#include <Eigen/Geometry>
#include <Eigen/Eigen>
#include <Eigen/Dense>
#include"GeometryLibrary.hpp"

using namespace std;
using namespace Eigen;
using namespace GeometryLibrary;


#include "Utils.hpp"


int main(int argc, char ** argv)
{
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}




/*
int main()


{
    string NameFile= "FR3_data.txt";
    vector<Fracture> fractures;
    fractures=readDFN(NameFile);
    vector<Trace> Traces;
    Traces=computeTraces(fractures);
    cout<<Traces[2].id<<endl<<Traces[2].Fracture1ID<<endl<<Traces[2].Fracture2ID<<endl<<Traces[2].firstPoint<<endl<<Traces[2].finalPoint;
    string outputFileName="Traces.txt";
    vector<vector<Support>> FractureTraces = writeResult(outputFileName,Traces);
    string outputName="TracesForFracture.txt";
    writeTracesForFracture(outputName,FractureTraces);

    vector<vector<Support>> Return;
    vector<Support> vec;
    Support S;
    S.idT=0;
    S.NumTraces=0;
    S.lenght=0;
    S.Tips=false;

    Return.push_back(vector<Support>());
    Return[0].push_back(S);



    Fracture fracture1;
    fracture1.numVertices = 4;
    fracture1.id = 0;
    fracture1.vertices.resize(3,4);


    fracture1.vertices << 0, 1, 1, 0,
                 0, 0, 1, 1,
                 0, 0, 0, 0;

    Fracture fracture2;
    fracture2.numVertices = 4;
    fracture2.id = 1;
    fracture2.vertices.resize(3,4);

    fracture2.vertices << 0, 1, 1, 0,
                 0.5, 0.5, 0.5, 0.5,
                -1, -1, 1, 1;
    vector<Fracture> vecFractures;
    vecFractures.push_back(fracture1);
    vecFractures.push_back(fracture2);
    vector<Trace> a;
    a=computeTraces(vecFractures);
   // cout<<a[0].firstPoint<<endl<<a[0].finalPoint;
    cout << a[0].vertex_Inters[0]<<"        "<<a[0].vertex_Inters[1]<<"        "<<a[0].vertex_Inters[2]<<"        "<<a[0].vertex_Inters[3]<<"        ";







  return 0;
}
*/
