#include <iostream>
#include <vector>
#include "pbPlots.hpp"
#include "supportLib.hpp"
#include <chrono>
#include <string>
#include <sstream>

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
};

class rawMesh{
    utils util;
    public:
        std::vector<std::vector<float>> verts;
        std::vector<std::vector<int>> faces;
        rawMesh(std::vector<std::vector<float>> vertexList, std::vector<std::vector<int>> faceList)//imports mesh
        {
            verts = vertexList;
            faces = faceList;
        }
        rawMesh(int dimensions, int vertCount)//generates random mesh
        {
            for (int i = 0; i < vertCount; i++){
                std::vector<float> emptyF;
                std::vector<int> emptyI;
                verts.push_back(emptyF);
                faces.push_back(emptyI);
                for (int j = 0; j < dimensions; j++){
                    verts[i].push_back(util.randF());
                    int index = i+j;
                    if (index >= vertCount)
                        index -= vertCount;
                    faces[i].push_back(index);
                }
            }
        }
};

class meshPointRandomizer{
    public:
        meshPointRandomizer(rawMesh mesh){

        }

};

void plotMesh(rawMesh mesh, double minx, double miny, double maxx, double maxy){
    RGBABitmapImageReference *imageRef = CreateRGBABitmapImageReference();
    std::vector<double> xs;
    std::vector<double> ys;
    for (int i = 0; i < mesh.verts.size(); i++){
        xs.push_back(mesh.verts[i][0]);
        ys.push_back(mesh.verts[i][1]);
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

    for (int i = 0; i < mesh.faces.size(); i++){
        double x1 = MapXCoordinateAutoSettings(mesh.verts[mesh.faces[i][0]][0], imageRef->image, &xs);
        double y1 = MapYCoordinateAutoSettings(mesh.verts[mesh.faces[i][0]][1], imageRef->image, &ys);
        double x2 = MapXCoordinateAutoSettings(mesh.verts[mesh.faces[i][1]][0], imageRef->image, &xs);
        double y2 = MapYCoordinateAutoSettings(mesh.verts[mesh.faces[i][1]][1], imageRef->image, &ys);
        DrawLine(imageRef->image,x1,y1,x2,y2,1.5,CreateRGBAColor(0,0,1,0.3));
    }

    std::vector<double> *pngData = ConvertToPNG(imageRef->image);
    WriteToFile(pngData,"plot.png");
    DeleteImage(imageRef->image);
}

void plotMesh(rawMesh mesh){
    return plotMesh(mesh,0,0,1,1);
}

int main() {
    srand(randSeed);
    const int verticieCount = 5;
    rawMesh m = rawMesh(dims,verticieCount);
    std::cin.ignore();
    plotMesh(m);
    std::cin.ignore();
    return 0;
}

