#include "Problem.h"

Problem::Problem(unsigned int nDeliveries, unsigned int vehCapacity)
{
	//seed random number gen.
	srand(time(NULL));

	deliveries.push_back({ 0, 0,0,0 });

	//index +1 as id=0 taken by distribution.
	unsigned int tmpq = 1;
	for(unsigned int x = 1; x<=nDeliveries;x++){
		double tmpx = static_cast<double>(rand())-(RAND_MAX/2);
		double tmpy = static_cast<double>(rand()) - (RAND_MAX / 2);
		if (vehCapacity > 1) tmpq = 1 + rand() % (vehCapacity - 1);//ensure delivery point have at least 1 qauntity.
		else tmpq = 1;
		deliveries.push_back({x, tmpx,tmpy,tmpq });
	}

	vehicles.push_back({0, vehCapacity });
}

Problem::~Problem()
{
}

std::vector<double> Problem::getCoords()
{
	std::vector<double> tmp;

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
unsigned int Problem::distributionIndex()
{
	return 0;
}

unsigned int Problem::getC()
{
	return vehicles[0].capacity;
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
