#include "GeometryLibrary.hpp"
#include<algorithm>
#include<cmath>
#include <fstream>
#include <vector>

using namespace std;
using namespace Eigen;

namespace GeometryLibrary{
//****************************************************************************************************************
vector<Trace> computeTraces (vector<struct Fracture>& fractures,
                            vector<Vector3d>& vertex_Inters_f1,
                            vector<Vector3d>& vertex_Inters_f2)
{
    vector<Trace> vecTrace;
    Trace Trace;
    int countID=0;
    for (size_t i = 0; i < fractures.size(); ++i) {
        for (size_t j = i + 1; j < fractures.size(); ++j) {
            if (!testCircumference(fractures[i], fractures[j]))
            {
                break;
            }

            // Calcolo i vettori che generano la normale per la prima frattura:

            Vector3d u1 = fractures[i].vertices.col(2) - fractures[i].vertices.col(0);
            Vector3d v1 = fractures[i].vertices.col(1) - fractures[i].vertices.col(0);
            Vector3d norm1 = (u1.cross(v1)).normalized();

            // Calcolo i vettori che generano la normale per la seconda frattura:

            Vector3d u2 = fractures[j].vertices.col(2) - fractures[j].vertices.col(0);
            Vector3d v2 = fractures[j].vertices.col(1) - fractures[j].vertices.col(0);
            Vector3d norm2 = (u2.cross(v2)).normalized();

            // Calcolo la direzione della retta di intersezione:

            Vector3d tangent = norm1.cross(norm2);

            // Definisco il sistema lineare:

            Matrix3d A;
            A << norm1.transpose(), norm2.transpose(), tangent.transpose();

            if (!(A.determinant() != 0)) // si può sostituire con il calcolo di un prodotto vettoriale,vedere appunti Vicini
            {
                break;
            }


            Vector3d b;
            double d1 = fractures[i].vertices.col(0).dot(norm1);
            double d2 = fractures[j].vertices.col(0).dot(norm2);
            b << d1, d2, 0;

            // Risolvo il sistema lineare:

            Vector3d intersection_point = A.fullPivLu().solve(b);// utilizziamo la fattorizzazione PALU perché è la più efficiente, con un costo di (n^3)/3 operazioni
            Vector3d otherPoint= intersection_point+0.1*tangent;
            vector<Vector3d> vertex_Inters_f1; // questi due vettori mi servono per tenere traccia del fatto che i vertici delle tracce
            vector<Vector3d> vertex_Inters_f2; // appartengono alla prima frattura o alla seconda o a entrambe
            vector<Vector3d > trace_vertexes =TraceVertexes(intersection_point,otherPoint,fractures[i],fractures[j],
                                                             vertex_Inters_f1,
                                                             vertex_Inters_f2);
            Trace.id=countID;
            Trace.Fracture1ID=fractures[i].id;
            Trace.Fracture2ID=fractures[j].id;
            Trace.firstPoint=trace_vertexes[0];
            Trace.finalPoint=trace_vertexes[1];
            vecTrace.push_back(Trace);



        }

    }
    return vecTrace;
}




//****************************************************************************************************************

vector<Vector3d> TraceVertexes(Vector3d& Point1, // nel selezionare i vertici della traccia voglio portarmi dietro anche l'informazione
                               Vector3d& Point2, // sui quattro vertici calcolati mediante l'intersezione per poi usarli per vedere tips
                               Fracture& fracture1,
                               Fracture& fracture2,
                               vector<Vector3d>& vertex_Inters_f1,
                               vector<Vector3d>& vertex_Inters_f2)
{
    vector<Vector3d> vertex_Inters;
    for(int i=0;i<fracture1.numVertices;++i)
    {
        MatrixXd A;
        Vector3d b=fracture1.vertices.col(i )-Point1;
        Vector3d d=fracture1.vertices.col(i+1 % fracture1.numVertices)-fracture1.vertices.col(i );
        A<< (Point2-Point1);
        A<<d;
       // if(! (A.determinant() !=0)) //La matrice A è 3x2 non si può calcolare il determinanate.Serve un altro metodo
        //{
      //      break;
     //   }
        Vector2d solution=A.fullPivLu().solve(b);
        Vector3d intersectionPoint=Point1+solution[0]*(Point2-Point1);//nel fare Point2-Point1 devo normalizzare?
        Vector3d u=fracture1.vertices.col(i+1% fracture1.numVertices)-fracture1.vertices.col(i );
        Vector3d v=Point1-fracture1.vertices.col(i );
        Vector3d z=Point1-fracture1.vertices.col(i+1% fracture1.numVertices);
        if(!(u.dot(v)>0 && -u.dot(z)<0)){ // verifico che il punto trovato sia all'interno del lato della frattura. Come?Vedere appunti Vicini e tablet
            break;
        }
        vertex_Inters.push_back(intersectionPoint);
        vertex_Inters_f1.push_back(intersectionPoint);


    }

    for(int i=0;i<fracture1.numVertices;++i)
    {
        MatrixXd A;
        Vector3d b= Point1-fracture2.vertices.col(i % fracture2.numVertices);
        Vector3d d=fracture2.vertices.col(i % fracture2.numVertices)-fracture2.vertices.col(i+1 % fracture2.numVertices);
        A<< (Point1-Point2);
        A<<d;
        if(! (A.determinant() !=0))
        {
            break;
        }
        Vector3d solution=A.fullPivLu().solve(b);
        Vector3d intersectionPoint=Point1+solution[0]*(Point2-Point1);
        Vector3d u=fracture2.vertices.col(i+1% fracture2.numVertices)-fracture2.vertices.col(i );
        Vector3d v=Point1-fracture2.vertices.col(i );
        Vector3d z=Point1-fracture2.vertices.col(i+1% fracture2.numVertices);
        if(!(u.dot(v)>0 && -u.dot(z)<0)){ // verifico che il punto trovato sia all'interno del lato della frattura. Come?Vedere appunti Vicini e tablet
            break;
        }
        vertex_Inters.push_back(intersectionPoint);
        vertex_Inters_f2.push_back(intersectionPoint);

    }


    sort(vertex_Inters.begin(),vertex_Inters.end(),comparePoints);

    vector<Vector3d> TraceVertexes;
    TraceVertexes.push_back(vertex_Inters[1]);
    TraceVertexes.push_back(vertex_Inters[2]);

    return TraceVertexes;
}

//****************************************************************************************************************
bool comparePoints(const Vector3d& v1,const Vector3d& v2){
    if(v1[0]!=v2[0]) return v1[0]<v2[0];
    if(v1[1]!=v2[1]) return v1[1]<v2[1];
    if(v1[2]!=v2[2]) return v1[2]<v2[2];
}
//****************************************************************************************************************
vector<vector<Support>> writeResult(const string& outputFilePath,
                            vector<Trace>& Traces,
                            vector<Vector3d>& vertex_Inters_f1,
                            vector<Vector3d>& vertex_Inters_f2)
{
    vector<vector<Support>> Return;
    vector<Support> Support;
    struct Support S;
    ofstream file;
    file.open(outputFilePath);

    if (file.fail())
    {
        cerr<< "file open failed"<< endl;

    }
    const string c=";";
    file<<"# Number of Traces"<<endl;
    file<<Traces.size()<<endl;
    file<<"#  TraceId; FractureId1; FractureId2; X1; Y1; Z1; X2; Y2; Z2"<<endl;
    for(size_t i=0;i<Traces.size();++i)
    {
        file<<Traces[i].id<<c<<Traces[i].Fracture1ID<<c<<Traces[i].Fracture2ID<<c;
        for(int k=0;k<2;++k){
            file<<Traces[i].firstPoint[k]<<c;
        }
        for(int h=0;h<1;++h){
            file<<Traces[i].finalPoint[h]<<c;
        }
        file<<Traces[i].finalPoint[2]<<endl;


        S.idT=Traces[i].id;
        S.lenght=pow(Traces[i].finalPoint[0]-Traces[i].firstPoint[0],2)+
                                              pow(Traces[i].finalPoint[1]-Traces[i].firstPoint[1],2)+
                                                pow(Traces[i].finalPoint[2]-Traces[i].firstPoint[2],2);
        //con questa condizione verifico se i vertici della traccia appartengono entrambi alla frattura1
        if( ((Traces[i].firstPoint.isApprox(vertex_Inters_f1[0])) || (Traces[i].firstPoint.isApprox(vertex_Inters_f1[1]))) &&
            ((Traces[i].finalPoint.isApprox(vertex_Inters_f1[0])) || (Traces[i].finalPoint.isApprox(vertex_Inters_f1[1])))  ){

            S.Tips=false;

        }
        else
        {
            S.Tips=true;
        }

        Support.push_back(S);
        Return[Traces[i].Fracture1ID]=Support;

        if( ((Traces[i].firstPoint.isApprox(vertex_Inters_f2[0])) || (Traces[i].firstPoint.isApprox(vertex_Inters_f2[1]))) &&
            ((Traces[i].finalPoint.isApprox(vertex_Inters_f2[0])) || (Traces[i].finalPoint.isApprox(vertex_Inters_f2[1])))  ){

            S.Tips=false;

        }
        else
        {
            S.Tips=true;
        }


        Support.push_back(S);
        Return[Traces[i].Fracture2ID]=Support;





    }
    //ordino i vettori per lunghezza in maniera decrescente e separatamente per passante e non passante
    for(auto& vec:Return)
    {
        vector< struct Support> trueSupports;
        vector<struct Support> falseSupports;
        for(auto& support:vec){
            if(support.Tips)
            {trueSupports.push_back(support); }
            else
            {falseSupports.push_back(support);}

        }

        sort(trueSupports.begin(),trueSupports.end(),compareLenght);
        sort(falseSupports.begin(),falseSupports.end(),compareLenght);
        vec.clear();
        vec.insert(vec.end(),trueSupports.begin(),trueSupports.end());
        vec.insert(vec.end(),falseSupports.begin(),falseSupports.end());

    }

}
//****************************************************************************************************************
bool compareLenght(const Support& Support1,const Support& Support2)
{
    return Support1.lenght>Support2.lenght;
}
//****************************************************************************************************************
bool writeTracesForFracture(const string& outputFilePath,
                            vector<vector<Support>>& FractureTraces)
{
    ofstream file;
    file.open(outputFilePath);

    if (file.fail())
    {
        cerr<< "file open failed"<< endl;
        return false;
    }
    string c=";";
    for(size_t i=0;i<FractureTraces.size();++i)
    {
        file<<"# FractureId; NumTraces"<<endl;
        file<<i<<c<<FractureTraces[i].size()<<endl;
        for(size_t j=0;j<FractureTraces[i].size();++j)
        {
            file<<"# TraceId; Tips; Length"<<endl;
            file<<FractureTraces[i][j].idT<<c<<FractureTraces[i][j].Tips<<c<<FractureTraces[i][j].lenght<<endl;
        }
    }
    return true;
}


//****************************************************************************************************************
bool testCircumference(Fracture& fracture1,
                       Fracture& fracture2)
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
Vector3d computeCentroid(Fracture& fracture)
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



















