#pragma once
#include "Problem.h"
#include <cmath>
#include <algorithm>
#include "../Visualisation/Renderer.h"
#include "TraingularMatrix.h"

class Solver
{
protected:
	std::shared_ptr<RenderData> renderData;
	std::vector<DeliveryPoint> data;
	const Problem& problem;

	//calculates euclidian distance between two points.
	double eDistance(DeliveryPoint* a, DeliveryPoint* b);
	double eDistance(DeliveryPoint& a, DeliveryPoint& b);
	double eDistance(const int& a, const int& b);

	double routeDistance(std::vector<int>& points);
	double tspNI(std::vector<int>& points);

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

inline double Solver::eDistance(DeliveryPoint& a, DeliveryPoint& b)
{
	return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

inline double Solver::eDistance(const int& a, const int& b)
{
	return sqrt(pow(data[a].x - data[b].x, 2) + pow(data[a].y - data[b].y, 2));
}

inline double Solver::routeDistance(std::vector<int>& points)
{
	double totalDistance = eDistance(*points.begin(), *(points.end()-1));
	for (auto p = points.begin(); p != points.end()-1; p++) {
		totalDistance += eDistance(*p, *(p + 1));
	}

	return totalDistance;
}

inline double Solver::tspNI(std::vector<int>& points)
{
	if (points.size() <= 1) return 0.0;
	std::vector<int> route;

	//choose two staring nodes.
	route.push_back(*(points.end() - 1));
	points.pop_back();
	route.push_back(*(points.end() - 1));
	points.pop_back();

	while (points.size() != 0) {
		//find nearest point to any point in route
		auto nearestPoint = points.begin();
		double nearestPointD = DBL_MAX;
		for (auto p = route.begin(); p != route.end(); p++) {
			for (auto n = points.begin(); n != points.end(); n++) {
				if (eDistance(*p, *n) < nearestPointD) {
					nearestPoint = n;
					nearestPointD = eDistance(*p, *n);
				}
			}
		}

		route.insert(route.begin(), *nearestPoint);
		points.erase(nearestPoint);

		int bestPos = 0;
		double bestRoute = routeDistance(route);
		int tmp;
		for (int i = 1; i < route.size()-1; i++) {
			tmp = route[i];
			route[i] = route[i-1];
			route[i-1] = tmp;
			double tmpRouteD = routeDistance(route);
			if (bestRoute > tmpRouteD) {
				bestPos = i;
				bestRoute = routeDistance(route);
			}
		}
		
		route.insert(route.begin() + bestPos, *(route.end()-1));
		route.pop_back();

	}
	points = route;
	
	return routeDistance(points);
}
