#include "VRP_Dantzig_Solver.h"

VRP_Dantzig_Solver::VRP_Dantzig_Solver(Problem problem, std::shared_ptr<RenderData> _renderData, int _wait)
	: Solver{ problem,_renderData } ,
	C(problem.getC()) , 
	D(TraingularMatrix<double>(data.size()))
{

	renderer_wait = _wait;
	if (data.size() > 25) showFullOutput = false;

	std::cout << "calculating number of stages of aggregation." << std::endl;

	//temporary vector for sorting qauntities
	std::vector<unsigned int> sortedQauntities;

	//add qauntities to vector for sorting
	for (auto i : data) {
		sortedQauntities.push_back(i.qauntitiy);
	}

	//sort data in acending order.
	std::sort(sortedQauntities.begin() + 1, sortedQauntities.end());


	int t = 0;
	for (unsigned int qauntitytmp = 0; qauntitytmp <= C; t++) {
		qauntitytmp += sortedQauntities[t];
		//terminate if end of vector reached.
		if (&sortedQauntities[t] == &sortedQauntities.back()) break;
	}
	//minus distribution point and loop additon.
	t -= 2;

	if (t < 2) t = 2;

	N = ceil(log(t));

	std::cout << "Maximum deliveries per route: t = " << t << std::endl;
	std::cout << "Stages of aggreagtion: N = " << N << std::endl;
}

VRP_Dantzig_Solver::~VRP_Dantzig_Solver()
{
}

void VRP_Dantzig_Solver::run()
{
	//start on stage 1.
	int stage = 1;

	//populate global Distance matrix
	for (int x = 0; x < data.size(); x++) {
		for (int y = data.size() - 1; y > x; y--) {
			D(x, y) = eDistance(data[x], data[y]);
		}
	}
	if (showFullOutput) {
		std::cout << "DISTANCE MATRIX" << std::endl;
		D.print();
	}

	std::vector<RouteAggregate> routeAggregates;
	for (auto i : data) {
		routeAggregates.push_back({ {i.id}, i.qauntitiy});
	}

	VRPStage(stage, routeAggregates);	
}

void VRP_Dantzig_Solver::VRPStage(int stage, std::vector<RouteAggregate>& A)
{
	//calulate stage maximum capacity
	int maxCombinedCapacity = C / (N - (stage - 1));
	std::cout << "Aggregation stage " << stage << " of " << N << " - Max Combined Capacity = " << maxCombinedCapacity << std::endl;

	//Create Distance Matrix.
	TraingularMatrix<double> Distance(A.size());
	for (int x = 0; x < Distance.m_depth; x++) {
		for (int y = Distance.m_depth - 1; y > x; y--) {
			//will only calulate if admissable to improve preformance.
			if ((A[x].qauntitiy + A[y].qauntitiy) <= maxCombinedCapacity) {
				//temporary vector combines points from x and y.
				std::vector<unsigned int> tmp = A[x].containedPoints;
				tmp.insert(tmp.end(), A[y].containedPoints.begin(), A[y].containedPoints.end());
				//calculate shortest distance to meet points using tsp algorithm.
				Distance(x, y) = tspNI(tmp);
			}
		}
	}
	if (showFullOutput) {
		std::cout << "STAGE " << stage << " DISTANCE MATRIX" << std::endl;
		Distance.print();
		std::cout << std::endl;
	}

	TraingularMatrix<int> X(A.size());
	//Create Routes matrix set 0,i to 1 so all points start connected to the distribution point.
	for (int x = 0; x < A.size(); x++) {
		X(x, 0) = 1;
	}

	//update visualiser with created X matrix.
	updateVis2(X, A);

	if (showFullOutput) {
		std::cout << "STAGE " << stage << " CONNECTION MATRIX" << std::endl;
		X.print();
		std::cout << std::endl;
	}

	//piValues for each aggregate used to calculate deltas.
	std::vector<double> piValues;
	piValues.push_back(0);//sets distribution point arbitrarily to 0.

	//Calculate piValue for each point.
	for (int x = 1; x < A.size(); x++) {
		for (int y = 0; y < A.size(); y++) {
			if (x != y && X(x, y) == 1) {
				std::vector<unsigned int> tmp = A[x].containedPoints;
				tmp.insert(tmp.end(), A[y].containedPoints.begin(), A[y].containedPoints.end());
				tmp.push_back(0);
				piValues.push_back(-(piValues[y] - tspNI(tmp)));
				break;
			}
		}
	}

	std::vector<delta> deltas;
	//calculate deltas for admiisable values of X
	for (int x = 1; x < A.size(); x++) {
		for (int y = A.size() - 1; y > x; y--) {
			//calcualtes deltas for Distances that are calculated.
			if (Distance(x, y)>0) deltas.push_back({ x,y, (piValues[x] + piValues[y]) - Distance(x, y) });
		}
	}

	//sorts deltas form highest to lowest.
	std::sort(std::begin(deltas), std::end(deltas), [](const delta& a, const delta& b) {
		return (a.value < b.value);
		});

	if (showFullOutput) {
		std::cout << "STAGE " << stage << " DELTAS" << std::endl;
		for (auto i : deltas) {
			std::cout << "(" << i.x << "," << i.y << ") delta =" << i.value << std::endl;
		}
	}

	//apply corrections using deltas
	while (deltas.size() != 0) {
		//exit if correction would not improve solution.
		if ((deltas.end() - 1)->value < 0) break;
		RapidCorrection(X, (deltas.end() - 1)->x, (deltas.end() - 1)->y);
		deltas.pop_back();
		updateVis2(X, A);
	}

	
	if (showFullOutput) {
		std::cout << "UPDATED STAGE " << stage << " CONNECTION MATRIX" << std::endl;
		X.print();
		std::cout << std::endl;
	}

	std::vector<RouteAggregate> newAggregates;
	//collect points that are still swinned with Distribution.
	for (int y = X.m_depth - 1; y > 0; y--) {
		if (X(0,y)) newAggregates.push_back({ A[y].containedPoints, A[y].qauntitiy });
	}
	for (int x = 1; x < X.m_depth; x++) {
		for (int y = X.m_depth - 1; y > x; y--) {
			if (X(x, y)) {
				std::vector<unsigned int> tmp = A[x].containedPoints;
				tmp.insert(tmp.end(), A[y].containedPoints.begin(), A[y].containedPoints.end());
				newAggregates.push_back({ tmp,A[x].qauntitiy + A[y].qauntitiy });
			}
		}
	}

	//if another stage of aggregation required call recursive, or complete finalisation.
	if (stage != N) {
		//required to stop points connecting with themselves.
		newAggregates.insert(newAggregates.begin(), { {0},0 });
		//launch next stage of VRP.
		VRPStage(stage + 1, newAggregates);
	}
	else {
		for (int i = 0; i < newAggregates.size(); i++) {
			//add distribution point and find optimal route from points using tsp.
			newAggregates[i].containedPoints.push_back(0);
			tspNI(newAggregates[i].containedPoints);

			//move distribution to front of all routes, for presentation.
			while (newAggregates[i].containedPoints[0] != 0) {
				newAggregates[i].containedPoints.insert(newAggregates[i].containedPoints.begin(), *(newAggregates[i].containedPoints.end() - 1));
				newAggregates[i].containedPoints.pop_back();
			}
			newAggregates[i].containedPoints.push_back(0);
		}
		updateVis2(newAggregates);
		finalStage(newAggregates);
	}
}

void VRP_Dantzig_Solver::finalStage(std::vector<RouteAggregate>& A)
{
	std::cout << "VRP COMPLETE!" << std::endl;
	std::cout << "solution contains " << A.size() << " routes." << std::endl;
	double totalCost = 0;
	int routeID = 0;
	for (auto i : A) {
		std::cout << "ROUTE " << routeID++ << " : ";
		for (auto z : i.containedPoints) {
			std::cout << z << ", ";
		}
		std::cout << std::endl;
		totalCost += routeDistance(i.containedPoints);
	}
	std::cout << std::endl;
	std::cout << "TOTAL COST OF ROUTES :" << totalCost << std::endl;
}

void VRP_Dantzig_Solver::updateVis2(TraingularMatrix<int>& X, std::vector<RouteAggregate>& A)
{
	renderData->LinesClear();
	for (int x = 0; x < X.m_depth; x++) {
		for (int y = X.m_depth - 1; y > x; y--) {
			if (X(x, y) == 1) {
				std::vector<unsigned int> tmp = A[x].containedPoints;
				tmp.insert(tmp.end(), A[y].containedPoints.begin(), A[y].containedPoints.end());
				renderData->AddLine(tmp, { 1.f,1.f,1.f,1.f }, 2);
			}
		}
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(renderer_wait));
}

void VRP_Dantzig_Solver::updateVis2(std::vector<RouteAggregate>& A)
{
	renderData->LinesClear();
	for (auto i : A) {
		renderData->AddLine(i.containedPoints, { 1.f,1.f,1.f,1.f }, 2);
	}
}


void VRP_Dantzig_Solver::RapidCorrection(TraingularMatrix<int>& _X, int x, int y)
{
	if (_X(0, x) == 1 && _X(0, y) == 1) {
		_X(0, x) = 0;
		_X(0, y) = 0;
		_X(x, y) = 1;
	}
}