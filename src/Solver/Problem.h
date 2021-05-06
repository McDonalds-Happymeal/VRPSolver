#pragma once

#include <vector>
#include <string>
#include <stdlib.h>
#include <time.h> 
#include <iostream>
#include <fstream>
#include <sstream>

#include "DeliveryPoint.h"
#include "DeliveryVehicles.h"

class Problem
{
private:
	//stores all delivery points and distribution at index 0;
	std::vector<DeliveryPoint> deliveries;
	std::vector<DeliveryVehicles> vehicles;
public:
	Problem(unsigned int nDeliveries, unsigned int vehCapacity);
	~Problem();

	void saveProblem(std::string file);
	void loadProblem(std::string file);

	std::vector<double> getCoords();
	std::vector<DeliveryPoint> getPointsData();
	unsigned int distributionIndex();

	unsigned int getC();
	
	void printall();
};

