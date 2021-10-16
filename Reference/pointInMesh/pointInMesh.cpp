#include <iostream>
#include <vector>
#include "pbPlots.hpp"
#include "supportLib.hpp"
#include <chrono>
#include <string>
#include <sstream>
#include <math.h>

int dims = 2;
const float halfPi = M_PI/2;
const float twoPi = M_PI*2;
std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
const long long int randSeed = tp.time_since_epoch().count();

class utils{
    public:
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
        
        float cos(float x, int accuracy){
            long I = * ( long * ) &x;
            I &= ~(1UL << 31);//abs(x)
            x = * ( float * ) &I;

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
        std::vector<float> addVectors(std::vector<float> v1,std::vector<float> v2){
            std::vector<float> result;
            for (int i = 0; i < v1.size(); i++){
                result.push_back(v1[i]+v2[i]);
            }
            return result;
        }
        std::vector<float> subtractVectors(std::vector<float> v1,std::vector<float> v2){
            for (int i = 0; i < v2.size(); i++){
                v2[i] = -v2[i];
            }
            return addVectors(v1,v2);
        }
        std::vector<float> multiplyVector(std::vector<float> v1,float val){
            std::vector<float> ret;
            for (int i = 0; i < v1.size(); i++){
                ret.push_back(v1[i]*val);
            }
            return ret;
        }
        std::vector<float> divideVector(std::vector<float> v1,float val){
            return multiplyVector(v1,powf(val,-1));
        }
        float sum(std::vector<float> v){
            while (v.size() > 1){
                v[1] += v[0];
                std::vector<float> newV;
                for (int i = 1; i < v.size(); i++){
                    newV.push_back(v[i]);
                }
                v = newV;
            }
            return v[0];
        }
        std::vector<float> sumRowwise(std::vector<std::vector<float>> m){
            std::vector<float> v;
            for (int i = 0; i < m.size(); i++){
                v.push_back(sum(m[i]));
            }
            return v;
        }
        std::vector<float> vecMatMul(std::vector<std::vector<float>> m, std::vector<float> v){
            std::vector<std::vector<float>> vals2Sum = m;
            for (int i = 0; i < vals2Sum.size(); i++)
                for (int j = 0; j < vals2Sum[0].size(); j++)
                    vals2Sum[i][j] = m[i][j]*v[i];
            return sumRowwise(vals2Sum);
        }
        std::vector<std::vector<float>> getGausJordanInvMatrix(std::vector<std::vector<float>> m){
            std::vector<std::vector<float>> im(m.size());
            std::vector<float> emptyRow(m[0].size());
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
                        std::vector<float> mVecToRem = multiplyVector(m[step],m[row][step]);
                        std::vector<float> imVecToRem = multiplyVector(im[step],m[row][step]);
                        m[row] = subtractVectors(m[row],mVecToRem);
                        im[row] = subtractVectors(im[row],imVecToRem);
                    }
                }
            }
            return im;
        }
        std::vector<std::vector<float>> invMatrix(std::vector<std::vector<float>> m){
            return getGausJordanInvMatrix(m);
        }
};

class rawMesh{
    public:
        utils util;
        std::vector<std::vector<float>> verts;
        std::vector<std::vector<int>> faces;
        rawMesh(){}
        rawMesh(std::vector<std::vector<float>> vertexList, std::vector<std::vector<int>> faceList)//imports mesh
        {
            verts = vertexList;
            faces = faceList;
        }
        rawMesh(int dimensions, int vertCount)//generates randomized circular mesh
        {
            float angleSpacing = twoPi/vertCount;
            std::cout << angleSpacing;
            std::cout << "\n";
            std::cout << vertCount;
            std::cin.ignore();
            for (int i = 0; i < vertCount; i++){
                std::vector<float> emptyF;
                std::vector<int> emptyI;
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
    public:
        utils util;
        std::vector<std::vector<int>> triangles;
        std::vector<std::vector<float>> triShifts;
        std::vector<std::vector<std::vector<float>>> rotMatricies;
        std::vector<std::vector<float>> foldingLines;
        
        meshPointRandomizer(rawMesh mesh){
            verts = mesh.verts;
            faces = mesh.faces;
        }
        void genDivs(){
            std::vector<float> zero;
            zero.push_back(0.5);zero.push_back(0.5);
            verts.push_back(zero);
            for (int i = 0; i < faces.size(); i++){
                std::vector<int> emptyI;
                triangles.push_back(emptyI);
                triangles[i].push_back(faces[i][0]);
                triangles[i].push_back(faces[i][1]);
                triangles[i].push_back(verts.size()-1);
            }
        }
        void genTransforms(){
            for (int i = 0; i < triangles.size(); i++){
                //shift transform
                triShifts.push_back(verts[triangles[i][0]]);
                //rotation transform
                std::vector<std::vector<float>> lineVecs;
                lineVecs.push_back(util.subtractVectors(verts[triangles[i][1]],verts[triangles[i][0]]));
                lineVecs.push_back(util.subtractVectors(verts[triangles[i][2]],verts[triangles[i][0]]));
                std::vector<float> tempAngles;
                for (int i = 0; i < 2; i++){
                    tempAngles.push_back(std::atan(lineVecs[i][1]/lineVecs[i][0]));
                    while (tempAngles[i] > twoPi)
                        tempAngles[i] -= twoPi;
                    while (tempAngles[i] < 0)
                        tempAngles[i] += twoPi;
                }
                float angle;
                if (tempAngles[1] > tempAngles[0]){
                    angle = tempAngles[1];
                    foldingLines.push_back(lineVecs[0]);//folding transform following line
                }
                else{
                    angle = tempAngles[0];
                    foldingLines.push_back(lineVecs[1]);//folding transform following line
                }
                std::vector<std::vector<float>> matrix = {{0,0},{0,0}};
                angle -= halfPi;
                float cosA = cos(angle);
                float sinA = sin(angle);
                matrix[0][0] = cosA;
                matrix[1][1] = cosA;
                matrix[0][1] = sinA;
                matrix[1][0] = -sinA;
                rotMatricies.push_back(matrix);
                //square 2 triangle transform
                std::vector<std::vector<float>> refLinePoints;
                std::vector<std::vector<float>> invMatrix = util.invMatrix(matrix);
                refLinePoints.push_back(util.vecMatMul(invMatrix,verts[triangles[i][2]]));
                refLinePoints.push_back(util.vecMatMul(invMatrix,verts[triangles[i][1]]));
                std::vector<float> refLine = util.subtractVectors(refLinePoints[1],refLinePoints[0]);;
                if (refLine[0] < 0){
                    std::vector<float> zero;zero.push_back(0);zero.push_back(0);
                    refLine = util.subtractVectors(zero,refLine);
                }//TODO: finish up

            }
        }
        std::vector<float> pickPoint(){
            std::vector<float> point;
            point.push_back(0.5);point.push_back(0.5);
            return point;
        }
};

void plotMesh(std::vector<std::vector<float>> verts, std::vector<std::vector<int>> faces, std::vector<std::vector<int>> triangles, double minx, double miny, double maxx, double maxy, std::vector<float> point, std::string file){
    RGBABitmapImageReference *imageRef = CreateRGBABitmapImageReference();
    std::vector<double> xs;
    std::vector<double> ys;
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
    std::vector<std::vector<double>> pxs;
    std::vector<std::vector<double>> pys;
    std::vector<double> x = {point[0]};
    std::vector<double> y = {point[1]};
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
            if (jp > 3)
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

    std::vector<double> *pngData = ConvertToPNG(imageRef->image);
    WriteToFile(pngData,file);
    DeleteImage(imageRef->image);
}

void plotMesh(rawMesh mesh, std::vector<float> point, std::string file){
    std::vector<std::vector<int>> triangles;
    return plotMesh(mesh.verts, mesh.faces, triangles, 0, 0, 1, 1, point, file);
}

void plotMesh(meshPointRandomizer mesh, std::vector<float> point, std::string file){
    return plotMesh(mesh.verts, mesh.faces, mesh.triangles, 0, 0, 1, 1, point, file);
}

int main() {
    std::cin.ignore();
    srand(randSeed);
    const int verticieCount = 5;
    meshPointRandomizer m = rawMesh(dims,verticieCount);
    m.genDivs();
    std::vector<float> point = m.pickPoint();
    plotMesh(m, point, "plot1.png");
    return 0;
}