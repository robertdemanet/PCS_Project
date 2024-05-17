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







/*int main()


{
    string NameFile= "FR3_data.txt";
    vector<Fracture> fractures;
    fractures=readDFN(NameFile);
    vector<Trace> Traces;
    Traces=computeTraces(fractures);
    cout<<Traces[0].id<<endl<<Traces[0].Fracture1ID<<endl<<Traces[0].Fracture2ID<<endl<<Traces[0].firstPoint<<endl<<Traces[0].finalPoint;




  return 0;
}

*/
