#include "Problem.h"

Problem::Problem(unsigned int nDeliveries, unsigned int nVehicles, unsigned int vehCapacity)
{
	//seed random number gen.
	srand(time(NULL));

	distribution = { 0,0,0,0 };

	//index +1 as id=0 taken by distribution.
	for(unsigned int x = 1; x<=nDeliveries;x++){
		double tmpx = static_cast<double>(rand())-(RAND_MAX/2);
		double tmpy = static_cast<double>(rand()) - (RAND_MAX / 2);
		unsigned int tmpq = rand() % vehCapacity;
		deliveries.push_back({x, tmpx,tmpy,tmpq });
	}

	for (unsigned int x = 0; x < nVehicles; x++) {
		vehicles.push_back({x, vehCapacity });
	}
}

Problem::~Problem()
{
}

std::vector<double> Problem::getCoords()
{
	std::vector<double> tmp;

	//add distribution coords first so coorsponds to index 0;
	tmp.push_back(distribution.x);
	tmp.push_back(distribution.y);

	for (auto &p : deliveries)
	{
		tmp.push_back(p.x);
		tmp.push_back(p.y);
	}
	return tmp;
}

std::vector<DeliveryPoint> Problem::getPointsData()
{
	return deliveries;
}

//returns index of distribution
std::vector<unsigned int> Problem::distributionIndex()
{
	std::vector<unsigned int> tmp = { distribution.id };
	return tmp;
}

void Problem::printall()
{
	std::cout << "\n\n    " << deliveries.size() <<" DELIVERY POINTS: \n" << std::endl;

	unsigned long int tmpq = 0;//adds total qauntitiy to be delivered
	unsigned int tmpc = 0;//adds total vehicle capacity.
	for (auto& p : deliveries) {
		std::cout << p.id << " : (" << p.x << " , " << p.y << ") qauntity = " << p.qauntitiy << std::endl;
		tmpq += p.qauntitiy;
	}

	std::cout << "\n\n    " << vehicles.size() << " DELIVERY VEHICLES: \n" << std::endl;

	for (auto& v : vehicles) {
		std::cout << v.id << " : capacity = " << v.capacity << std::endl;
		tmpc += v.capacity;
	}
	
	std::cout << "\n\n" << deliveries.size() << " delivery points with a combined delivery qaunitity of " << tmpq << "." << std::endl;
	std::cout << vehicles.size() << " delivery vehicles with a combined capacity of " << tmpc << "."
		<< std::endl;
	if(tmpc >0) std::cout << "Estimated minimum number dispatches per vehicle : " << (tmpq / tmpc) + 1 << "." << std::endl;
}
