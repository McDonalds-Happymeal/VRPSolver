#include <iostream>
#include <vector>
#include "Visualisation/Renderer.h"
#include "Visualisation/Window.h"
#include "Problem.h"
    
int main(int argc, char** argv) {


    Problem problem(50, 4, 500);
    

    Window window("hello World!", 0.8f, 0);


    float dScale = 10.0f;
    //how much points scale from default %
    float pointsRange = 0.9f;
    Color blue = { 0.0f,0.0f,1.0f,1.0f };
    Color red = { 1.0f,0.0f,0.0f,1.0f };

    /*---Create renderer and translate data from problem------*/

    float minPointSize = dScale - (dScale * pointsRange);
    //sends all points coords.
    Renderer render = Renderer(problem.getCoords());
    //index 0 is distribution point.
    render.pointsProperties(0, blue, dScale+(dScale*pointsRange));

    std::vector<DeliveryPoint> p = problem.getPointsData();

    unsigned int max = p[1].qauntitiy;
    unsigned int min = p[1].qauntitiy;
    for (std::vector<DeliveryPoint>::const_iterator it = p.begin(); it != p.end();it++) {
        max = (max < it->qauntitiy) ? it->qauntitiy : max;
        min = (min > it->qauntitiy) ? it->qauntitiy : min;
    }

    //finds how mnay point size availible;
    int bins = static_cast<int>(ceil((dScale + (dScale * pointsRange)) - (dScale - (dScale * pointsRange))));

    int step = (max - min) / bins;
    int tmpMax = min + step;
    std::vector<unsigned int> tmpBin;
    for (int x = 0; x <= bins; x++) {
        tmpBin.clear();
        std::cout << "bin is : " << min << " to " << tmpMax << std::endl;
        for (std::vector<DeliveryPoint>::const_iterator it = p.begin(); it != p.end(); it++) {
            if (it->qauntitiy >= min && it->qauntitiy < tmpMax) tmpBin.push_back(it->id);
        }
        if(!tmpBin.empty()) render.pointsProperties(tmpBin, red, minPointSize + x);
        min += step;
        tmpMax += step;
    }

    


    std::vector<unsigned int> bois = { 2,5,8,1,2 };
    render.AddLine(bois, {0.0f, 0.0f, 0.0f, 1.0f}, minPointSize);

    problem.printall();

    window.Run(&render);

    return 0;

}