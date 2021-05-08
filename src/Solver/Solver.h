#pragma once
#include "Problem.h"
#include <cmath>
#include <algorithm>
#include "../Visualisation/Renderer.h"


class Solver
{
protected:
	std::shared_ptr<RenderData> renderData;
	std::vector<DeliveryPoint> data;

	//calculates euclidian distance between two points.
	double eDistance(DeliveryPoint* a, DeliveryPoint* b);
	double eDistance(DeliveryPoint a, DeliveryPoint b);

public:
	Solver(Problem problem, std::shared_ptr<RenderData> _renderData);
	~Solver();

	virtual void run();


};

//calculates euclidian distance between two points.
inline double Solver::eDistance(DeliveryPoint* a, DeliveryPoint* b)
{
	return sqrt(pow(a->x - b->x, 2) + pow(a->y - b->y, 2));
}

inline double Solver::eDistance(DeliveryPoint a, DeliveryPoint b)
{
	return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}
