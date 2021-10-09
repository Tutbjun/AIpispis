#include <iostream>
#include <vector>
#include "pbPlots.hpp"
#include "supportLib.hpp"

int dims = 2;

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



/* void plotMesh(rawMesh mesh){
    RGBABitmapImageReference *imageRef = CreateRGBABitmapImageReference();
    std::vector<double> x{1,2,3,4,5};
    std::vector<double> y{1,2,3,4,5};

    DrawScatterPlot(imageRef, 600, 400, &x, &y);
    std::vector<double> *pngData = ConvertToPNG(imageRef->image);
    WriteToFile(pngData,"plot.png");
    DeleteImage(imageRef->image);
} */

int main() {
    const int verticieCount = 5;
    rawMesh m = rawMesh(dims,verticieCount);
    //plotMesh(m);
    std::cin.ignore();
    return 0;
}

