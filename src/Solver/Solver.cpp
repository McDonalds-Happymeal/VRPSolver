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
        shortestDistance = eDistance(*it, *(it + 1));
        shortestIt = it + 1;
        path.push_back((*(it+1)).id);
        for (auto search = it + 2; search != data.end(); search++) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
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

    std::cout << "Route of length " << totalDistance << " found!" << std::endl;
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
