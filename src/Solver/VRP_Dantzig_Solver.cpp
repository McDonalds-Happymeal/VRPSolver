#include "VRP_Dantzig_Solver.h"

VRP_Dantzig_Solver::VRP_Dantzig_Solver(Problem problem, std::shared_ptr<RenderData> _renderData)
	: Solver{ problem,_renderData } , C(problem.getC()), D(TraingularMatrix<double>(data.size())), X(TraingularMatrix<int>(data.size()))
{
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

	N = ceil(log(t));

	std::cout << "Maximum deliveries per route: t = " << t << std::endl;
	std::cout << "stages of aggreagtion: N = " << N << std::endl;


	//populate Distance matrix
	for (int x = 0; x < data.size(); x++) {
		for (int y = data.size()-1; y > x; y--) {
			D(x, y) = eDistance(data[x], data[y]);
		}
	}
	std::cout << "DISTANCE MATRIX" << std::endl;
	D.print();

	//Create Routes matrix set 0,i to 1 so all points start connected to the distribution point.
	for (int x = 0; x < data.size(); x++) {
		X(x, 0) = 1;
	}

	std::cout << "ROUTE MATRIX" << std::endl;
	X.print();

	updateVis();
}

VRP_Dantzig_Solver::~VRP_Dantzig_Solver()
{
}

void VRP_Dantzig_Solver::aggregationStage(std::vector<RouteAggregate> Deliveries, int stage)
{
	int maxCombinedCapacity = C / (N - (stage - 1));
	std::cout << "Aggregation stage " << stage << " of " << N << " - max Combined Capacity = " << maxCombinedCapacity << std::endl;

	//Distance Matrix
	TraingularMatrix<double> Distance(Deliveries.size());
	TraingularMatrix<int> Admisable(Deliveries.size());
	for (int x = 0; x < Deliveries.size() - 1; x++) {
		for (int y = Deliveries.size() - 1; y > x; y--) {
			if ((Deliveries[x].qauntitiy + Deliveries[y].qauntitiy) <= maxCombinedCapacity) {
				std::vector<int> tmp = Deliveries[x].containedPoints;
				tmp.insert(tmp.end(), Deliveries[y].containedPoints.begin(), Deliveries[y].containedPoints.end());
				tmp.push_back(0);
				Distance(x, y) = tspNI(tmp);
				Admisable(x, y) = 1;
			}
		}
	}

	Distance.print();
	Admisable.print();

	TraingularMatrix<int> stageX(Deliveries.size());
	//Create Routes matrix set 0,i to 1 so all points start connected to the distribution point.
	for (int x = 0; x < Deliveries.size(); x++) {
		stageX(x, 0) = 1;
	}

	//piValues for each aggregate used to calculate deltas.
	std::vector<double> piValues;
	piValues.push_back(0);//sets distribution point arbitrarily to 0.

	for (int x = 1; x < Deliveries.size(); x++) {
		for (int y = 0; y < Deliveries.size(); y++) {
			if (x != y && stageX(x, y) == 1) {
				std::vector<int> tmp = Deliveries[x].containedPoints;
				tmp.insert(tmp.end(), Deliveries[y].containedPoints.begin(), Deliveries[y].containedPoints.end());
				tmp.push_back(0);
				piValues.push_back(-(piValues[y] - tspNI(tmp)));
				break;
			}
		}
	}

	for (auto i : piValues) {
		std::cout << "piValue =" << i << std::endl;
	}

	std::vector<delta> deltas;
	//calculate deltas for admiisable values of X
	for (int x = 1; x < Deliveries.size(); x++) {
		for (int y = Deliveries.size() - 1; y > x; y--) {
			if (Admisable(x, y)) deltas.push_back({ x,y, (piValues[x] + piValues[y]) - Distance(x, y) });
		}
	}

	std::sort(std::begin(deltas), std::end(deltas), [](const delta& a, const delta& b) {
		return (a.value < b.value);
		});

	std::cout << "DELTAS" << std::endl;
	for (auto i : deltas) {
		std::cout << "(" << i.x << "," << i.y << ") delta =" << i.value << std::endl;
	}

	while (deltas.size() != 0) {
		RapidCorrection(stageX,(deltas.end() - 1)->x, (deltas.end() - 1)->y);
		deltas.pop_back();
		stageX.print();
	}
	stageX.print();

	renderData->LinesClear();
	//find routes from stage
	std::vector<RouteAggregate> nextAggregates;
	for (int x = 0; x < Deliveries.size(); x++) {
		for (int y = Deliveries.size() - 1; y > x; y--) {
			if (stageX(x, y)) {
				std::vector<int> tmp = Deliveries[x].containedPoints;
				tmp.insert(tmp.end(), Deliveries[y].containedPoints.begin(), Deliveries[y].containedPoints.end());
				nextAggregates.push_back({ tmp,Deliveries[x].qauntitiy + Deliveries[y].qauntitiy });
				std::vector<unsigned int> drawData;
				for (auto i : tmp) {
					drawData.push_back(static_cast<unsigned int>(i));
				}
				renderData->AddLine(drawData, { 1.0f,1.0f,1.0f,1.0f }, 2);
			}
		}
	}

	if (stage == N) {
		renderData->LinesClear();

		double totalDistance = 0;
		for (auto routes : nextAggregates) {
			routes.containedPoints.push_back(0);
			double tmpDistance = tspNI(routes.containedPoints);
			totalDistance += tmpDistance;
			std::vector<unsigned int> drawData;
			for (auto i : routes.containedPoints) {
				drawData.push_back(static_cast<unsigned int>(i));
			}
			drawData.push_back(drawData[0]);
			renderData->AddLine(drawData, { 1.0f,1.0f,1.0f,1.0f }, 2);
			std::cout << "ROUTE DISTANCE : " << tmpDistance << std::endl;
			for (auto i : routes.containedPoints) {
				std::cout << i << ", " << std::endl;
			}
			std::cout << std::endl;
		}
	}

}

void VRP_Dantzig_Solver::run()
{


	aggregationStage(1);

	X.print();
}

void VRP_Dantzig_Solver::updateVis()
{
	renderData->LinesClear();
	std::vector<unsigned int> linesToDraw;
	for (int x = 0; x < data.size(); x++) {
		for (int y = data.size()-1; y > x; y--) {
			if (X(x, y) == 1) {
				linesToDraw.clear();
				linesToDraw.push_back(x);
				linesToDraw.push_back(y);
				renderData->AddLine(linesToDraw, { 1.0f,1.0f,1.0f,1.0f }, 2);
			}
		}
	}
}



void VRP_Dantzig_Solver::aggregationStage(int stage)
{
	int maxCombinedCapacity = C / (N - (stage - 1));
	std::cout << "Aggregation stage " << stage << " of " << N << " - max Combined Capacity = " << maxCombinedCapacity << std::endl;

	//admisable matrix
	TraingularMatrix<int> A(data.size());

	for (int x = 0; x < data.size() - 1; x++) {
		for (int y = data.size() - 1; y > x; y--) {
			if (maxCombinedCapacity > (data[x].qauntitiy + data[y].qauntitiy)) A(x, y) = 1;
		}
	}
	A.print();

	//piValues for each delivery point used to calculate deltas.
	std::vector<double> piValues;
	piValues.push_back(0);//sets distribution point arbitrarily to 0.

	for (int x = 1; x < data.size(); x++) {
		for (int y = 0; y < data.size(); y++) {
			if (x != y && X(x, y) == 1) {
				piValues.push_back(-(piValues[y] - D(x, y)));
				break;
			}
		}
	}

	std::vector<delta> deltas;
	//calculate deltas for admiisable values of X
	for (int x = 1; x < data.size(); x++) {
		for (int y = data.size() - 1; y > x; y--) {
			if (A(x, y)) deltas.push_back({ x,y, piValues[x] + piValues[y] - D(x, y) });
		}
	}

	std::sort(std::begin(deltas), std::end(deltas), [](const delta& a, const delta& b) {
		return (a.value < b.value);
		});

	std::cout << "DELTAS" << std::endl;
	for (auto i : deltas) {
		std::cout << "(" << i.x << "," << i.y << ") delta =" << i.value << std::endl;
	}

	while (deltas.size() != 0) {
		RapidCorrection((deltas.end() - 1)->x, (deltas.end() - 1)->y);
		deltas.pop_back();
		updateVis();
	}


	std::vector<RouteAggregate> Aggregates;
	Aggregates.push_back({ { 0 },0 });
	for (int x = 0; x < data.size(); x++) {
		for (int y = data.size() - 1; y > x; y--) {
			if (X(x, y)) {
				if(x==0) Aggregates.push_back({ {y},data[x].qauntitiy + data[y].qauntitiy });
				else if (y==0) Aggregates.push_back({ {x},data[x].qauntitiy + data[y].qauntitiy });
				else Aggregates.push_back({ {x,y},data[x].qauntitiy + data[y].qauntitiy });
			}
		}
	}

	aggregationStage(Aggregates ,stage+1);
}


void VRP_Dantzig_Solver::RapidCorrection(int x, int y)
{
	if (X(0, x) == 1 && X(0, y) == 1) {
		X(0, x) = 0;
		X(0, y) = 0;
		X(x, y) = 1;
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
