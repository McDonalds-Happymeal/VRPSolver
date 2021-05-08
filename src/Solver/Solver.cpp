#include "Solver.h"

Solver::Solver(Problem problem, std::shared_ptr<RenderData> _renderData) : renderData(_renderData)
{
    data = problem.getPointsData();
}

Solver::~Solver()
{
}

void Solver::run()
{
    std::cout << "program runs!" << std::endl;
}




