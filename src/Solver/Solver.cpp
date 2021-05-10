#include "Solver.h"

Solver::Solver(Problem _problem, std::shared_ptr<RenderData> _renderData) : renderData(_renderData) , problem(_problem)
{
    data = _problem.getPointsData();
}

Solver::~Solver()
{
}

void Solver::run()
{
    std::cout << "program runs!" << std::endl;
}




