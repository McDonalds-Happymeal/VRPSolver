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


	std::vector<Coords> Aggregates;
	for (int x = 1; x < data.size(); x++) {
		for (int y = data.size() - 1; y > x; y--) {
			if (X(x, y)) Aggregates.push_back({ x,y });
		}
	}

	//void aggregationStage(stage--, Aggregates);


}