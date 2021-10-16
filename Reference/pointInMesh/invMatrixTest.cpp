#include <vector>

std::vector<float> vecDiv(std::vector<float> v1,float val){
    std::vector<float> ret;
    for (int i = 0; i < v1.size(); i++){
        ret.push_back(v1[i]/val);
    }
    return ret;
}
std::vector<float> vecMinus(std::vector<float> v1,std::vector<float> v2){
    std::vector<float> ret;
    for (int i = 0; i < v1.size(); i++){
        ret.push_back(v1[i]-v2[i]);
    }
    return ret;
}
std::vector<float> vecMult(std::vector<float> v1,float val){
    std::vector<float> ret;
    for (int i = 0; i < v1.size(); i++){
        ret.push_back(v1[i]*val);
    }
    return ret;
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
        m[step] = vecDiv(m[step],val);
        im[step] = vecDiv(im[step],val);
        for (int row = 0; row < m.size(); row++){
            if (row != step){
                std::vector<float> mVecToRem = vecMult(m[step],m[row][step]);
                std::vector<float> imVecToRem = vecMult(im[step],m[row][step]);
                m[row] = vecMinus(m[row],mVecToRem);
                im[row] = vecMinus(im[row],imVecToRem);
            }
        }
    }
    return im;
}

int main() {
    std::vector<std::vector<float>> m = {{1,3,-2},{0,4,1},{-1,-2,2}};
    std::vector<std::vector<float>> inv = getGausJordanInvMatrix(m);
    return 0;
}