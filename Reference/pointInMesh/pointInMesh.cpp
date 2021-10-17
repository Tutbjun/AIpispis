#include <iostream>
#include <vector>
#include "pbPlots.hpp"
#include "supportLib.hpp"
#include <chrono>
#include <string>
#include <sstream>
#include <math.h>
#include <random>
#include <numbers>

//TODO:Exceptions 2 fix:
//! terminate called after throwing an instance of 'std::bad_array_new_length'  what():  std::bad_array_new_length
//! error with no exception that happens in arround 1/100000 point findings ¯\_(ツ)_/¯

using namespace std;

int dims = 2;
const float Pi = numbers::pi;
const float halfPi = Pi/2;
const float twoPi = Pi*2;
chrono::system_clock::time_point tp = chrono::system_clock::now();
const long long int randSeed = tp.time_since_epoch().count();
random_device rd;

class utils{
    public:
        void print(string in){
            std::cout << in << ' ';
        }
        void print(float in){
            print(to_string(in));
        }
        void print(vector<float> in){
            print("\n");
            for (float e : in)
                print(e);
        }
        void print(vector<vector<float>> in){
            for (vector<float> v : in)
                print(v);
        }
        int randIFromWeightedList(vector<float> weightedList, float sumOfList){
            float val = randF(sumOfList);
            int i;
            for (i = 0; i < weightedList.size(); i++){
                val -= weightedList[i];
                if (val <= 0)
                    break;
            }
            return i;
        }
        int randI(int min,int max){
            mt19937 rng(rd());
            uniform_int_distribution<int> uni(min,max);
            return (int)uni(rng);
        }
        float randF(float start, float stop){
            return start + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(stop-start)));
        }
        float randF(float stop){
            return static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/stop));
        }
        float randF(){
            return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        }
        float cosTaylor(float x,int n){
            switch(n){
                case 0:
                    return 1;
                case 1:
                    return -pow(x,2)/2;
                case 2:
                    return pow(x,4)/24;
                case 3:
                    return -pow(x,6)/720;
                case 4:
                    return pow(x,8)/40320;
                case 5:
                    return -pow(x,10)/3628800;
                case 6:
                    return pow(x,12)/479001600;
                default:
                    return 0;
            }
        }
        
        float abs(float x){
            long I = * ( long * ) &x;
            I &= ~(1UL << 31);//abs(x)
            x = * ( float * ) &I;
            return x;
        }

        float cos(float x, int accuracy){
            x = abs(x);

            float cycles = x/(halfPi);
            float dummy;
            float cycleDec = modf(cycles,&dummy);
            x = halfPi*cycleDec;

            int cycle = int(cycles)+1;
            if (cycle % 2 == 0)
                x-=halfPi;
            
            float val = 0;
            for (int i = 0; i < accuracy; i++)
                val += cosTaylor(x,i);

            int wholeCycles = (cycle+2) >> 1;
            if (wholeCycles % 2 == 0)
                val = -val;
            return val;
        }
        float cos(float in){
            return cos(in, 6);
        }
        float sin(float in, int accuracy){
            return cos(in-halfPi,accuracy);
        }
        float sin(float in){
            return cos(in-halfPi);
        }
        float tan(float in, int accuracy){
            return sin(in, accuracy)/cos(in, accuracy);
        }
        float tan(float in){
            return sin(in)/cos(in);
        }
        vector<float> addVectors(vector<float> v1,vector<float> v2){
            vector<float> result;
            for (int i = 0; i < v1.size(); i++){
                result.push_back(v1[i]+v2[i]);
            }
            return result;
        }
        vector<float> subtractVectors(float val1,vector<float> v2){
            vector<float> v1;
            for (int i = 0; i < v2.size(); i++)
                v1.push_back(val1);
            return subtractVectors(v1,v2);
        }
        vector<float> subtractVectors(vector<float> v1,vector<float> v2){
            for (int i = 0; i < v2.size(); i++){
                v2[i] = -v2[i];
            }
            return addVectors(v1,v2);
        }
        vector<float> multiplyVector(vector<float> v1,float val){
            vector<float> res;
            for (int i = 0; i < v1.size(); i++){
                res.push_back(v1[i]*val);
            }
            return res;
        }
        vector<vector<float>> multiplyVector(vector<vector<float>> m1,float val){
            vector<vector<float>> res = m1;
            for (int i = 0; i < m1.size(); i++){
                res[i] = multiplyVector(m1[i],val);
            }
            return res;
        }
        vector<float> divideVector(vector<float> v1,float val){
            return multiplyVector(v1,1/val);
        }
        float sum(vector<float> v){
            while (v.size() > 1){
                v[1] += v[0];
                vector<float> newV;
                for (int i = 1; i < v.size(); i++){
                    newV.push_back(v[i]);
                }
                v = newV;
            }
            return v[0];
        }
        float dotProduct(vector<float> v1, vector<float> v2){
            float val = 0;
            for (int i = 0; i < v1.size(); i++)
                val += v1[i]*v2[i];
            return val;
        }
        vector<float> vecMatMul(vector<vector<float>> m, vector<float> v){
            vector<float> result = v;
            for (int i = 0; i < result.size(); i++)
                result[i] = dotProduct(v,m[i]);
            return result;
        }
        vector<vector<float>> vecMatMul(vector<vector<float>> m, vector<vector<float>> vs){
            vector<vector<float>> results = vs;
            for (int i = 0; i < results.size(); i++)
                results[i] = vecMatMul(m,vs[i]);
            return results;
        }
        vector<vector<float>> getGausJordanInvMatrix(vector<vector<float>> m){
            vector<vector<float>> im(m.size());
            vector<float> emptyRow(m[0].size());
            for (int i = 0; i < im.size(); i++){
                im[i] = emptyRow;
                im[i][i] = 1;
            }
            for (int step = 0; step < m[0].size(); step++){
                float val = m[step][step];
                m[step] = divideVector(m[step],val);
                im[step] = divideVector(im[step],val);
                for (int row = 0; row < m.size(); row++){
                    if (row != step){
                        vector<float> mVecToRem = multiplyVector(m[step],m[row][step]);
                        vector<float> imVecToRem = multiplyVector(im[step],m[row][step]);
                        m[row] = subtractVectors(m[row],mVecToRem);
                        im[row] = subtractVectors(im[row],imVecToRem);
                    }
                }
            }
            return im;
        }
        vector<vector<float>> invMatrix(vector<vector<float>> m){
            return getGausJordanInvMatrix(m);
        }
        float vectorLength(vector<float> v){
            return sqrtf(powf(v[0],2)+powf(v[1],2));
        }
        float vectorAngle(vector<float> v1, vector<float> v2){
            return acosf(dotProduct(v1,v2)/(vectorLength(v1)*vectorLength(v2)));
        }
};

class rawMesh{
    public:
        utils util;
        vector<vector<float>> verts;
        vector<vector<int>> faces;
        rawMesh(){}
        rawMesh(vector<vector<float>> vertexList, vector<vector<int>> faceList)//imports mesh
        {
            verts = vertexList;
            faces = faceList;
        }
        rawMesh(int dimensions, int vertCount)//generates randomized circular mesh
        {
            float angleSpacing = twoPi/vertCount;
            for (int i = 0; i < vertCount; i++){
                vector<float> emptyF;
                vector<int> emptyI;
                verts.push_back(emptyF);
                faces.push_back(emptyI);
                float theta = i * angleSpacing + util.randF(-angleSpacing/2,angleSpacing/2);
                float r = 0.25 + util.randF(-0.2,0.2);
                verts[i].push_back(util.cos(theta)*r);//!not suited for more than 2 dimensions
                verts[i][0] += 0.5;
                verts[i].push_back(util.sin(theta)*r);
                verts[i][1] += 0.5;
                for (int j = 0; j < dimensions; j++){
                    int index = i+j;
                    if (index >= vertCount)
                        index -= vertCount;
                    faces[i].push_back(index);
                }
            }
        }
};

class meshPointRandomizer : public rawMesh {
    private:
        bool transformsGenerated = false;
        bool divsGenerated = false;
    public:
        utils util;
        vector<vector<int>> triangles;
        vector<float> triangleAreas;
        float triangleAreasSum;
        vector<vector<float>> triShifts;
        vector<vector<vector<float>>> rotMatricies;
        vector<vector<float>> fliprotMatrix = {{-1,0},{0,-1}};
        vector<vector<float>> fliprotPoints;
        vector<vector<float>> fliprotLines;
        vector<float> fliprotAs;
        vector<vector<vector<float>>> triangleTransformMatricies;
        
        meshPointRandomizer(rawMesh mesh){
            verts = mesh.verts;
            faces = mesh.faces;
        }
        void genDivs(){
            vector<float> zero;
            zero.push_back(0.5);zero.push_back(0.5);
            verts.push_back(zero);
            for (int i = 0; i < faces.size(); i++){
                vector<int> emptyI;
                triangles.push_back(emptyI);
                triangles[i].push_back(faces[i][0]);
                triangles[i].push_back(faces[i][1]);
                triangles[i].push_back(verts.size()-1);
            }
            divsGenerated = true;
        }
        void genTransforms(){
            for (int i = 0; i < triangles.size(); i++){
                //shift transform
                triShifts.push_back(verts[triangles[i][0]]);
                //rotation transform
                vector<vector<float>> lineVecs;
                lineVecs.push_back(util.subtractVectors(verts[triangles[i][1]],verts[triangles[i][0]]));
                lineVecs.push_back(util.subtractVectors(verts[triangles[i][2]],verts[triangles[i][0]]));
                vector<float> tempAngles;
                for (int i = 0; i < 2; i++){
                    tempAngles.push_back(atan(lineVecs[i][1]/lineVecs[i][0]));
                    while (tempAngles[i] > twoPi)
                        tempAngles[i] -= twoPi;
                    while (tempAngles[i] < 0)
                        tempAngles[i] += twoPi;
                }
                float angle;
                float deltaAngle = tempAngles[1]-tempAngles[0];
                vector<float> foldingLine;
                angle = tempAngles[1];
                foldingLine = lineVecs[0];
                angle -= halfPi;
                float cosA = cos(angle);
                float sinA = sin(angle);
                vector<vector<float>> invRotMatrix = {{cosA,sinA},{-sinA,cosA}};
                if (util.vecMatMul(invRotMatrix,foldingLine)[0] < 0)//should be decidable by decission tree instead of this crap
                    invRotMatrix = util.multiplyVector(invRotMatrix,-1);
                vector<vector<float>> rotMatrix = util.invMatrix(invRotMatrix);
                rotMatricies.push_back(rotMatrix);
                //folding transform line definition
                foldingLine = util.vecMatMul(invRotMatrix,foldingLine);
                fliprotLines.push_back(foldingLine);
                fliprotPoints.push_back(util.divideVector(foldingLine,2));
                fliprotAs.push_back(fliprotLines[i][1]/fliprotLines[i][0]);
                //square 2 triangle transform
                vector<vector<float>> refLinePoints;
                refLinePoints.push_back(util.vecMatMul(invRotMatrix,lineVecs[0]));
                refLinePoints.push_back(util.vecMatMul(invRotMatrix,lineVecs[1]));
                if (refLinePoints[0][0]>refLinePoints[1][0])
                    swap(refLinePoints[0],refLinePoints[1]);
                float scalerY = refLinePoints[0][1];
                float scalerX = refLinePoints[1][0];
                float shifter = scalerY-refLinePoints[1][1];
                vector<vector<float>> triangleMatrix = {{scalerX,0},{-shifter,scalerY}};
                triangleTransformMatricies.push_back(triangleMatrix);
                //find triangle areas
                float a = util.vectorLength(lineVecs[0]);
                float b = util.vectorLength(lineVecs[1]);
                float triAngle = util.vectorAngle(lineVecs[0],lineVecs[1]);
                float A = 0.5f*a*b*util.sin(triAngle);
                triangleAreas.push_back(A);
            }
            triangleAreasSum = util.sum(triangleAreas);
            transformsGenerated = true;
        }
        vector<float> performTransforms(vector<float> inPoint, int transformSet){
            vector<float> point = util.vecMatMul(triangleTransformMatricies[transformSet],inPoint);//transform to triangle
            if (point[0]*fliprotAs[transformSet] > point[1]){//flip if outside triangle
                point = util.subtractVectors(point,fliprotPoints[transformSet]);
                point = util.vecMatMul(fliprotMatrix,point);
                point = util.addVectors(point,fliprotPoints[transformSet]);
            }
            point = util.vecMatMul(rotMatricies[transformSet],point);//rotate to desired spot
            point = util.addVectors(point,triShifts[transformSet]);
            return point;
        }
        vector<float> pickPoint(){
            if (divsGenerated == false)
                genDivs();
            if (transformsGenerated == false)
                genTransforms();

            vector<float> randCartPoint;
            randCartPoint.push_back(util.randF());randCartPoint.push_back(util.randF());

            int pickedTriangle = util.randIFromWeightedList(triangleAreas,triangleAreasSum);
            vector<float> point = performTransforms(randCartPoint,pickedTriangle);
            return point;
        }
};

void plotMesh(vector<vector<float>> verts, vector<vector<int>> faces, vector<vector<int>> triangles, double minx, double miny, double maxx, double maxy, vector<vector<float>> points, string file){
    RGBABitmapImageReference *imageRef = CreateRGBABitmapImageReference();
    vector<double> xs;
    vector<double> ys;
    for (int i = 0; i < verts.size(); i++){
        xs.push_back(verts[i][0]);
        ys.push_back(verts[i][1]);
    }
    xs.push_back(minx);
    ys.push_back(miny);
    xs.push_back(maxx);
    ys.push_back(maxy);

    ScatterPlotSeries *serVerts = GetDefaultScatterPlotSeriesSettings();
	serVerts->xs = &xs;
	serVerts->ys = &ys;
	serVerts->linearInterpolation = false;
	serVerts->pointType = toVector(L"dots");
	serVerts->color = CreateRGBColor(1, 0, 0);

    ScatterPlotSeries *serPoint = GetDefaultScatterPlotSeriesSettings();
    vector<vector<double>> pxs;
    vector<vector<double>> pys;
    vector<double> x;
    vector<double> y;
    for (int i = 0; i < points.size(); i++){
        x.push_back(points[i][0]);
        y.push_back(points[i][1]);
    }
    pxs.push_back(x);
    pys.push_back(y);
	serPoint->xs = &x;
	serPoint->ys = &y;
	serPoint->linearInterpolation = false;
	serPoint->pointType = toVector(L"crosses");
	serPoint->color = CreateRGBColor(0, 0, 0);

    ScatterPlotSettings *settings = GetDefaultScatterPlotSettings();
	settings->width = 600;
	settings->height = 400;
	settings->autoBoundaries = true;
	settings->autoPadding = true;
	settings->title = toVector(L"");
	settings->xLabel = toVector(L"");
	settings->yLabel = toVector(L"");
	settings->scatterPlotSeries->push_back(serVerts);
    settings->scatterPlotSeries->push_back(serPoint);

    DrawScatterPlotFromSettings(imageRef,settings);

    for (int i = 0; i < triangles.size(); i++){
        for (int j = 0; j < 3; j++){
            int jp = j + 1;
            if (jp >= 3)
                jp -= 3;
            double x1 = MapXCoordinateAutoSettings(verts[triangles[i][j]][0], imageRef->image, &xs);
            double y1 = MapYCoordinateAutoSettings(verts[triangles[i][j]][1], imageRef->image, &ys);
            double x2 = MapXCoordinateAutoSettings(verts[triangles[i][jp]][0], imageRef->image, &xs);
            double y2 = MapYCoordinateAutoSettings(verts[triangles[i][jp]][1], imageRef->image, &ys);
            DrawLine(imageRef->image,x1,y1,x2,y2,1,CreateRGBColor(0,1,0));
        }
    }

    for (int i = 0; i < faces.size(); i++){
        double x1 = MapXCoordinateAutoSettings(verts[faces[i][0]][0], imageRef->image, &xs);
        double y1 = MapYCoordinateAutoSettings(verts[faces[i][0]][1], imageRef->image, &ys);
        double x2 = MapXCoordinateAutoSettings(verts[faces[i][1]][0], imageRef->image, &xs);
        double y2 = MapYCoordinateAutoSettings(verts[faces[i][1]][1], imageRef->image, &ys);
        DrawLine(imageRef->image,x1,y1,x2,y2,1.5,CreateRGBAColor(1,0,0,0.3));
    }

    vector<double> *pngData = ConvertToPNG(imageRef->image);
    WriteToFile(pngData,file);
    DeleteImage(imageRef->image);
}

void plotMesh(rawMesh mesh, vector<vector<float>> points, string file){
    vector<vector<int>> triangles;
    return plotMesh(mesh.verts, mesh.faces, triangles, 0, 0, 1, 1, points, file);
}

void plotMesh(rawMesh mesh, vector<float> point, string file){
    vector<vector<float>> points;
    points.push_back(point);
    return plotMesh(mesh, points, file);
}
void plotMesh(meshPointRandomizer mesh, vector<float> point, string file){
    vector<vector<float>> points;
    points.push_back(point);
    return plotMesh(mesh, points, file);
}
void plotMesh(meshPointRandomizer mesh, vector<vector<float>> points, string file){
    return plotMesh(mesh.verts, mesh.faces, mesh.triangles, 0, 0, 1, 1, points, file);
}
void plotMesh(meshPointRandomizer mesh, string file){
    vector<vector<float>> points = {{0,0}};
    return plotMesh(mesh.verts, mesh.faces, mesh.triangles, 0, 0, 1, 1, points, file);
}

int main() {
    srand(randSeed);
    const int verticieCount = 5;
    meshPointRandomizer m = rawMesh(dims,verticieCount);
    plotMesh(m, "plot1.png");
    vector<vector<float>> points;
    for (int i = 0; i < 1000; i++)
        points.push_back(m.pickPoint());
    plotMesh(m, points, "plot1.png");
    return 0;
}