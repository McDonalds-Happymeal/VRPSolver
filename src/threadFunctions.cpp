#include "threadFunctions.h"

void renderThread(std::shared_ptr<RenderData>_renderData, Problem& problem, float windowScale, int moniter, float drawScale) {
    std::shared_ptr<RenderData> renderData = _renderData;
    Window window("hello World!", windowScale, moniter);

    //how much points scale from default %
    float pointsRange = 0.5f;
    Color blue = { 1.0f,1.0f,1.0f,1.0f };
    Color red = { 1.0f,1.0f,1.0f,1.0f };

    /*---Create renderer and translate data from problem------*/

    float minPointSize = drawScale - (drawScale * pointsRange);
    //sends all points coords.
    Renderer render = Renderer(problem.getCoords(), renderData, { 0.0f,0.0f,0.0f,1.0f });
    //index 0 is distribution point.
    render.pointsProperties(0, blue, drawScale + (drawScale * pointsRange));

    std::vector<DeliveryPoint> p = problem.getPointsData();

    unsigned int max = p[1].qauntitiy;
    unsigned int min = p[1].qauntitiy;
    for (std::vector<DeliveryPoint>::const_iterator it = p.begin(); it != p.end(); it++) {
        max = (max < it->qauntitiy) ? it->qauntitiy : max;
        min = (min > it->qauntitiy) ? it->qauntitiy : min;
    }

    //finds how mnay point size availible;
    int bins = static_cast<int>(ceil((drawScale + (drawScale * pointsRange)) - (drawScale - (drawScale * pointsRange))));

    int step = (max - min) / bins;
    unsigned int tmpMax = min + step;
    std::vector<unsigned int> tmpBin;
    for (int x = 0; x <= bins; x++) {
        tmpBin.clear();
        for (std::vector<DeliveryPoint>::const_iterator it = p.begin(); it != p.end(); it++) {
            if (it->qauntitiy >= min && it->qauntitiy < tmpMax) tmpBin.push_back(it->id);
        }
        if (!tmpBin.empty()) render.pointsProperties(tmpBin, red, minPointSize + x);
        min += step;
        tmpMax += step;
    }




    std::vector<unsigned int> bois = { 2,5,8,1,2 };
    _renderData->AddLine(bois, { 0.0f, 0.0f, 0.0f, 1.0f }, minPointSize);

    problem.printall();

    window.Run(&render);
}