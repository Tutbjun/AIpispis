#include <iostream>
#include <vector>
#include "pbPlots.hpp"
#include "supportLib.hpp"
#include <chrono>
#include <string>
#include <sstream>
#include <math.h>

int dims = 2;
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
        const float halfPi = M_PI/2;
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
};

class rawMesh{
    utils util;
    public:
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
            float angleSpacing = 2*M_PI/vertCount;
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
        std::vector<std::vector<int>> triangles;
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

};

void plotMesh(std::vector<std::vector<float>> verts, std::vector<std::vector<int>> faces, std::vector<std::vector<int>> triangles, double minx, double miny, double maxx, double maxy){
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

    ScatterPlotSettings *settings = GetDefaultScatterPlotSettings();
	settings->width = 600;
	settings->height = 400;
	settings->autoBoundaries = true;
	settings->autoPadding = true;
	settings->title = toVector(L"");
	settings->xLabel = toVector(L"");
	settings->yLabel = toVector(L"");
	settings->scatterPlotSeries->push_back(serVerts);

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
    WriteToFile(pngData,"plot.png");
    DeleteImage(imageRef->image);
}

void plotMesh(rawMesh mesh){
    std::vector<std::vector<int>> triangles;
    return plotMesh(mesh.verts, mesh.faces, triangles, 0, 0, 1, 1);
}

void plotMesh(meshPointRandomizer mesh){
    return plotMesh(mesh.verts, mesh.faces, mesh.triangles, 0, 0, 1, 1);
}

int main() {
    srand(randSeed);
    const int verticieCount = 5;
    meshPointRandomizer m = rawMesh(dims,verticieCount);
    m.genDivs();
    plotMesh(m);
    return 0;
}

