#pragma once
#include "Solver.h"
#include <algorithm>

struct delta {
    int x;
    int y;
    double value;
};

struct RouteAggregate {
    std::vector<int> containedPoints;
    unsigned int qauntitiy;
};


class VRP_Dantzig_Solver :
    public Solver
{
private:
    //Connection matrix.
    TraingularMatrix<int> X;
    //distance matrix.
    TraingularMatrix<double> D;

    int N;

    void updateVis();
    void updateVis2(TraingularMatrix<int>& stageX, std::vector<RouteAggregate>& Deliveries);

    void aggregationStage(int N);

    void aggregationStage(std::vector<RouteAggregate> Deliveries, int stage);

    void RapidCorrection(int x,int y);
    void RapidCorrection(TraingularMatrix<int>& _X, int x, int y);

    //capacity.
    const unsigned int C;
public:
    VRP_Dantzig_Solver(Problem problem, std::shared_ptr<RenderData> _renderData);
    ~VRP_Dantzig_Solver();

    void run();
};

