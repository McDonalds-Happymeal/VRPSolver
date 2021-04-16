#include "Solver.h"

Solver::Solver(Problem problem, std::shared_ptr<RenderData> _renderData) : renderData(_renderData)
{
    data = problem.getPointsData();
}

Solver::~Solver()
{
}

void Solver::TSPSolver()
{
    double totalDistance = 0;
    double shortestDistance;
    std::vector<unsigned int> path = { 0 };
    std::vector<DeliveryPoint>::iterator shortestIt;
    for (auto it = data.begin(); it != data.end()-1; it++) {
        shortestIt = it + 1;
        shortestDistance = eDistance(*it, *shortestIt);
        path.push_back(shortestIt->id);
        renderData->LinesClear();
        renderData->AddLine(path, { 1,1,1,1 }, 5.0f);
        for (auto search = it + 1; search != data.end(); search++) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            if (eDistance(*search, *it) < shortestDistance) {
                shortestDistance = eDistance(*search, *it);
                shortestIt = search;
                path.back() = (*shortestIt).id;
                renderData->LinesClear();
                renderData->AddLine(path, { 1,1,1,1 }, 5.0f);
            }
        }
        std::iter_swap(shortestIt, it + 1);
        totalDistance += shortestDistance;
    }
    
    
    totalDistance += eDistance(*data.begin(),*shortestIt);
    
    std::cout << "Route of length " << totalDistance << " found!" << std::endl;
    path.push_back(0);
    renderData->LinesClear();
    renderData->AddLine(path, { 1,1,1,1 }, 5.0f);
}



//calculates euclidian distance between two points.
inline double Solver::eDistance(DeliveryPoint* a, DeliveryPoint* b)
{
    return sqrt(pow(a->x - b->x, 2) + pow(a->y - b->y, 2));
}

inline double Solver::eDistance(DeliveryPoint a, DeliveryPoint b)
{
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}
