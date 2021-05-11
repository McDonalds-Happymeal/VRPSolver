#pragma once
#include "Solver.h"
#include <algorithm>

struct delta {
    int x;
    int y;
    double value;
};

struct RouteAggregate {
    std::vector<unsigned int> containedPoints;
    unsigned int qauntitiy;
};


class VRP_Dantzig_Solver :
    public Solver
{
private:
    //suppresses console output for larger problem to increase preformance.
    bool showFullOutput = true;

    //global distance matrix.
    TraingularMatrix<double> D;
    int N;
    void updateVis2(TraingularMatrix<int>& X, std::vector<RouteAggregate>& A);
    void updateVis2(std::vector<RouteAggregate>& A);


    void RapidCorrection(TraingularMatrix<int>& _X, int x, int y);
    void VRPStage(int stage, std::vector<RouteAggregate>& A);
    void finalStage(std::vector<RouteAggregate>& A);

    //capacity.
    const unsigned int C;
public:
    VRP_Dantzig_Solver(Problem problem, std::shared_ptr<RenderData> _renderData);
    ~VRP_Dantzig_Solver();

    void run();
};

