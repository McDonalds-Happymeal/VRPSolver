#pragma once
#include "Solver.h"
class VRP_Dantzig_Solver :
    public Solver
{
private:
    //Connection matrix.
    std::vector<std::vector<bool>> X;
    //distance matrix.
    std::vector<std::vector<double>> D;
    //admiisable matrix
    std::vector<std::vector<bool>> A;

    //capacity.
    const unsigned int C;
public:
    VRP_Dantzig_Solver(Problem problem, std::shared_ptr<RenderData> _renderData);
    ~VRP_Dantzig_Solver();

    void Solve();
};

