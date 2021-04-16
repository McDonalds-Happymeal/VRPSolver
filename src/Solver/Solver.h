#pragma once
#include "Problem.h"
#include <cmath>
#include <algorithm>
#include "../Visualisation/Renderer.h"

class Solver
{
private:
	std::shared_ptr<RenderData> renderData;

	std::vector<DeliveryPoint> data;
public:
	Solver(Problem problem, std::shared_ptr<RenderData> _renderData);
	~Solver();

	void TSPSolver();

	//calculates euclidian distance between two points.
	double eDistance(DeliveryPoint* a, DeliveryPoint* b);
	double eDistance(DeliveryPoint a, DeliveryPoint b);

};

