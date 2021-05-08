#pragma once
#include "Solver.h"
class TspNN :
    public Solver
{
private:

public:
    TspNN(Problem problem, std::shared_ptr<RenderData> _renderData);
    ~TspNN();

    void run();
};

