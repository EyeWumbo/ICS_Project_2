#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fstream>
#include <vector>
#include "Bridge.hpp"

using namespace std;

int main()
{
	processFile();
	pthread_t thread_id[totalCarNo];
	for(int i = 0; i < totalCarNo; i++)
	{
		Vehicle* car =  &cars[i];
		pthread_create(&thread_id[i], NULL, oneVehicle, (void*) car);
		pthread_join(thread_id[i],NULL);
	}
	
	printf("%d \n", totalCarNo);
	for (int i=0; i<totalCarNo; i++)
	{
		Vehicle* car =  &cars[i];
		printf("Car %d is going in direction %d \n", car->id, car->direction);
	}
	
	return 0;
}

void* oneVehicle(void* ptr)
{
	Vehicle* car = (Vehicle*) ptr;
	printf("Car %d is going in direction %d", car->id, car->direction);
/* 	ArriveBridge(car);
	CrossBridge(car);
	ExitBridge(car); */
	return car;
}

void processFile()
{
	std::ifstream file ("src/text.txt");
	if (file.is_open())
	{
		string info;
		getline(file, info);
		totalCarNo = atoi(info.c_str());
		printf("%d \n", info.c_str());
		cars = new Vehicle[totalCarNo];
		for(int i= 0; i < totalCarNo;i++)
		{
			std::getline(file, info);
			int pos = info.find(" ");
			std::string temp1 = info.substr(0,pos);
			std::string temp2 = info.substr(pos,info.length());
			int id = atoi(temp1.c_str());
			int dir = atoi(temp2.c_str());
			cars[i].id = id;
			cars[i].direction = dir;
		}
	}
	else
		std::cout << "ERROR: File Not Found" << std::endl;
	printf("%d \n", totalCarNo);
}