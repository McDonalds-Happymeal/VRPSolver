#pragma once
#include "Solver.h"
#include <algorithm>
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

    void aggregationStage(int N);

    void RapidCorrection(int x,int y);

    //capacity.
    const unsigned int C;
public:
    VRP_Dantzig_Solver(Problem problem, std::shared_ptr<RenderData> _renderData);
    ~VRP_Dantzig_Solver();

    
    void run();
};

struct delta {
    int x;
    int y;
    double value;
};