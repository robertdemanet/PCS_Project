#include "GeometryLibrary.hpp"
#include<algorithm>
#include<cmath>
#include <fstream>
#include <vector>

using namespace std;
using namespace Eigen;

namespace GeometryLibrary{
//****************************************************************************************************************
vector<Trace> computeTraces (vector<struct Fracture>& fractures)
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

            //Vector3d prod_vett1=u1.cross(v1);
            //Vector3d norm_1=prod_vett1/(u1.norm()*v1.norm());

            // Calcolo i vettori che generano la normale per la seconda frattura:

            Vector3d u2 = fractures[j].vertices.col(2) - fractures[j].vertices.col(0);
            Vector3d v2 = fractures[j].vertices.col(1) - fractures[j].vertices.col(0);
            Vector3d norm2 = (u2.cross(v2)).normalized();



            // Calcolo la direzione della retta di intersezione:

            Vector3d tangent = norm1.cross(norm2);

            // Definisco il sistema lineare:

            Matrix3d A;
            A << norm1.transpose(), norm2.transpose(), tangent.transpose();


            double c=tangent.dot(tangent);

          //  double moltipl= (norm2[1]*tangent[2]-norm2[2]*tangent[1])-(norm2[0]*tangent[2]-norm2[2]*tangent[0])+
                            // (norm2[0]*tangent[1]-norm2[1]*tangent[0]);

            if (A.determinant() != 0 && c!=0)// si può sostituire con il calcolo di un prodotto vettoriale,vedere appunti Vicini
            {
                Vector3d b;
                double d1 = fractures[i].vertices.col(0).dot(norm1);
                double d2 = fractures[j].vertices.col(0).dot(norm2);
                b << d1, d2, 0;

                // Risolvo il sistema lineare:



                Vector3d intersection_point = A.fullPivLu().solve(b);//P0, utilizziamo la fattorizzazione PALU perché è la più efficiente, con un costo di (n^3)/3 operazioni
                Vector3d otherPoint= intersection_point+0.1*tangent;
                vector<Vector3d > vertex_Inters1 =TraceVertexes(intersection_point,otherPoint,fractures[i]); //QUI HO A,B
                vector<Vector3d > vertex_Inters2 =TraceVertexes(intersection_point,otherPoint,fractures[j]); // QUI HO C,D
                if(vertex_Inters1.size()!=2 || vertex_Inters2.size()!=2)
                {
                    break;
                }
                double alphaA;
                double alphaB;
                double alphaC;
                double alphaD;
                if(tangent[0] != 0) {
                    alphaA= (vertex_Inters1[0][0]-intersection_point[0])/tangent[0];
                    alphaB= (vertex_Inters1[1][0]-intersection_point[0])/tangent[0];
                    alphaC= (vertex_Inters2[0][0]-intersection_point[0])/tangent[0];
                    alphaD= (vertex_Inters2[1][0]-intersection_point[0])/tangent[0];
                }
                else if (tangent[1]!=0) {
                    alphaA= (vertex_Inters1[0][1]-intersection_point[1])/tangent[1];
                    alphaB= (vertex_Inters1[1][1]-intersection_point[1])/tangent[1];
                    alphaC= (vertex_Inters2[0][1]-intersection_point[1])/tangent[1];
                    alphaD= (vertex_Inters2[1][1]-intersection_point[1])/tangent[1];
                }
                else if (tangent[2]!=0){
                    alphaA= (vertex_Inters1[0][2]-intersection_point[2])/tangent[2];
                    alphaB= (vertex_Inters1[1][2]-intersection_point[2])/tangent[2];
                    alphaC= (vertex_Inters2[0][2]-intersection_point[2])/tangent[2];
                    alphaD= (vertex_Inters2[1][2]-intersection_point[2])/tangent[2];
                }
                double alpha1;
                double alpha2;
                double alpha3;
                double alpha4;

                if(alphaA>alphaB)
                {
                    alpha1=alphaB;
                    alpha2=alphaA;
                }
                else
                {
                    alpha1=alphaA;
                    alpha2=alphaB;
                }


                if(alphaC>alphaD)
                {
                    alpha3=alphaD;
                    alpha4=alphaC;
                }
                else
                {
                    alpha3=alphaC;
                    alpha4=alphaD;
                }

                vector<double> d=compareAlphas(alpha1,alpha2,alpha3,alpha4);
                if(d.size()==0)
                {
                    break;
                }





                /*vector<Vector3d> vertex_Inters;
                vertex_Inters.insert(vertex_Inters.end(),vertex_Inters1.begin(),vertex_Inters1.end());
                vertex_Inters.insert(vertex_Inters.end(),vertex_Inters2.begin(),vertex_Inters2.end());*/

                Trace.vertex_Inters1=vertex_Inters1;
                Trace.vertex_Inters2=vertex_Inters2;
               // sort(vertex_Inters.begin(),vertex_Inters.end(),comparePoints);



                Trace.id=countID;
                Trace.Fracture1ID=fractures[i].id;
                Trace.Fracture2ID=fractures[j].id;
                Trace.firstPoint=intersection_point+d[0]*tangent;
                Trace.finalPoint=intersection_point+d[1]*tangent;
               /* if(vertex_Inters.size()>2){
                    Trace.firstPoint=vertex_Inters[1];
                    Trace.finalPoint=vertex_Inters[2];
                 }
                else
                 {
                    Trace.firstPoint=vertex_Inters[0];
                    Trace.finalPoint=vertex_Inters[1];
                 }*/

                vecTrace.push_back(Trace);
                countID=countID+1;










       }

    }
    }
    return vecTrace;


    // La funzione è giusta e fa quello che deve fare, ma è inutile defininire i vertici appartenenti a prima e seconda traccia
}



//****************************************************************************************************************

vector<Vector3d> TraceVertexes(Vector3d& Point1,
                               Vector3d& Point2,
                               Fracture& fracture1)
{
    const double tol= 1e-10;
    vector<Vector3d> vertex_Inters; // nelle prime due posizioni ho i punti di intersezione che trovo con la prima frattura
                                    // nelle altre due ho quelli che trovo con la frattura 2
    for(int i=0;i<fracture1.numVertices;++i)
    {
        MatrixXd A;
        A.resize(3,2);
        Vector3d b=fracture1.vertices.col(i )-Point1;

        Vector3d d=fracture1.vertices.col((i+1) % fracture1.numVertices)-fracture1.vertices.col(i );

        Vector3d t= Point2-Point1;
        A<< t,d;


        //controllo che le rette non siano parallele
        Vector3d control=t.cross(d);
        if(control[0]!=0 || control[1]!=0 || control[2]!=0)
        {
            Vector2d solution=A.householderQr().solve(b);

            Vector3d intersectionPoint=Point1+solution[0]*(Point2-Point1);
            // basta controllare che beta stia tra 0 e 1
            if( (-solution[1]>=0-tol && -solution[1]<=1+tol))
            {
                vertex_Inters.push_back(intersectionPoint);
            }

        }
    }



    return vertex_Inters;
}
//****************************************************************************************************************




//****************************************************************************************************************
vector<double> compareAlphas(double& alpha1,double& alpha2,double& alpha3,double& alpha4){

    if(alpha1>alpha4 || alpha3>alpha2)
    {
        vector<double> d={};
        return d ;
    }

    else{
        vector<double> d={alpha1,alpha2,alpha3,alpha4};
        sort(d.begin(),d.end());
        vector<double> d_={d[1],d[2]};
        return d_;
    }




}
//****************************************************************************************************************
vector<vector<Support>> writeResult(const string& outputFilePath,
                                    vector<Trace>& Traces)
{
    vector<vector<Support>> Return;
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
        for(int k=0;k<3;++k){
            file<<Traces[i].firstPoint[k]<<c;
        }
        for(int h=0;h<2;++h){
            file<<Traces[i].finalPoint[h]<<c;
        }
        file<<Traces[i].finalPoint[2]<<endl;


        S.idT=Traces[i].id;
        S.lenght=pow(Traces[i].finalPoint[0]-Traces[i].firstPoint[0],2)+
                                              pow(Traces[i].finalPoint[1]-Traces[i].firstPoint[1],2)+
                                                pow(Traces[i].finalPoint[2]-Traces[i].firstPoint[2],2);
        //con questa condizione verifico se i vertici della traccia appartengono entrambi alla frattura1
        //Qui il controllo sarebbe da fare con la tolleranza
        if(Traces[i].vertex_Inters1.size()==2){
         if( ((Traces[i].firstPoint.isApprox(Traces[i].vertex_Inters1[0])) || (Traces[i].firstPoint.isApprox(Traces[i].vertex_Inters1[1]))) &&
            ((Traces[i].finalPoint.isApprox(Traces[i].vertex_Inters1[0])) || (Traces[i].finalPoint.isApprox(Traces[i].vertex_Inters1[1])))  ){

            S.Tips=false;

            }
           else
            {
            S.Tips=true;
            }
        }
        else {S.Tips=true;}








        Return.push_back(vector<Support>());
        Return[Traces[i].Fracture1ID].push_back(S);

            //Qui il controllo sarebbe da fare con la tolleranza
        if(Traces[i].vertex_Inters2.size()==2){
            if( ((Traces[i].firstPoint.isApprox(Traces[i].vertex_Inters2[0])) || (Traces[i].firstPoint.isApprox(Traces[i].vertex_Inters2[1]))) &&
                ((Traces[i].finalPoint.isApprox(Traces[i].vertex_Inters2[0])) || (Traces[i].finalPoint.isApprox(Traces[i].vertex_Inters2[1])))  ){

                S.Tips=false;

            }
            else
            {
                S.Tips=true;
            }
        }
        else {S.Tips=true;}



        Return.push_back(vector<Support>());
        Return[Traces[i].Fracture2ID].push_back(S);






    }
    file.close();
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

    return Return;

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

    // Definisco i centroidi per ciascuna frattura:

    Vector3d centroid1 = computeCentroid(fracture1);
    Vector3d centroid2 = computeCentroid(fracture2);

    // Inizializzo i raggi delle due circonferenze a 0:

    double radius1 = 0.00;
    double radius2 = 0.00;

    // Itero sui vertici della prima frattura, aggiornando ad ogni iterata
    // il valore massimo assunto dalla distanza del centroide da un vertice

    for (int v = 0; v < fracture1.numVertices; ++v)
    {
        Vector3d vertex1(fracture1.vertices(0,v), fracture1.vertices(1,v), fracture1.vertices(2,v));
        Vector3d diff1 = centroid1 - vertex1;
        double distance1 = diff1.norm();
        radius1 = std::max(radius1, distance1);
    }

    // Itero sui vertici della prima frattura, aggiornando ad ogni iterata
    // il valore massimo assunto dalla distanza del centroide da un vertice

    for (int v = 0; v < fracture2.numVertices; ++v)
    {
        Vector3d vertex2(fracture2.vertices(0,v), fracture2.vertices(1,v), fracture2.vertices(2,v));
        Vector3d diff2 = centroid2 - vertex2;
        double distance2 = diff2.norm();
        radius2 = std::max(radius2, distance2);
    }

    // Calcolo la distanza nella spazio 3D dei due centroidi:

    Vector3d diff_centroids = centroid1 - centroid2;
    double distance_centroids = diff_centroids.norm();

    // E verifico se essa è maggiore della somma dei due raggi:

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

    // Definisco il vettore (punto 3D) centroide:

    Vector3d centroid;

    // Inizializzo le somme delle coordinate x/y/z a zero:

    double sum_x = 0;
    double sum_y = 0;
    double sum_z = 0;

    // Itero sul numero di vertici sommando ogni volta su ciascuna coordinata:

    int n_vert = fracture.numVertices;
    for (int j = 0; j < n_vert; ++j)
    {
        sum_x = sum_x + fracture.vertices(0,j);
        sum_y = sum_y + fracture.vertices(1,j);
        sum_z = sum_z + fracture.vertices(2,j);
    }

    // Calcolo i valori medi assunti da ciascuna coordinata e li inserisco nel vettore centroide:

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
        fracture.vertices.resize(3,fracture.numVertices);



       getline(file,line); // header of vertices
       for (int j = 0; j < 3; ++j) {
            getline(file,line); // line of x/y/z coordinates
            char d;
            istringstream convertCOORD(line);
            for (int k = 0; k < fracture.numVertices; ++k) {
                if(k != fracture.numVertices - 1){
                    double vertice;
                    convertCOORD >>vertice;
                    fracture.vertices(j,k)=vertice;
                    convertCOORD >> d;
                }
                else{
                    double vertice;
                    convertCOORD >>vertice;
                    fracture.vertices(j,k)=vertice;
                }
            }

        }
        fractures.push_back(fracture);

    }

    file.close();
    return fractures;
}
//****************************************************************************************************************
}



















