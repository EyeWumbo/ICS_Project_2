#ifndef BRIDGE_HPP
#define BRIDGE_HPP

struct Vehicle
{
	int id;
	int direction;
};

int totalCarNo;
Vehicle* cars;

void processFile();
void* oneVehicle(void* ptr);

#endif