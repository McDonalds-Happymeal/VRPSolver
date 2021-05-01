#include "VRP_Dantzig_Solver.h"

VRP_Dantzig_Solver::VRP_Dantzig_Solver(Problem problem, std::shared_ptr<RenderData> _renderData)
	: Solver{ problem,_renderData } , C(problem.getC())
{

}
