#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fstream>
#include <vector>
#include <unistd.h>
#include "Bridge.hpp"

using namespace std;

int main()
{
	processFile();
	pthread_t thread_id[totalCarNo];
	currentDir = 0;
	for(int i = 0; i < totalCarNo; i++)
	{
		Vehicle* car =  &cars[i];
		pthread_create(&thread_id[i], NULL, oneVehicle, (void*) car);
		//pthread_join(thread_id[i],NULL);
	}	
	//printf("%d \n", totalCarNo);
	for (int i=0; i<totalCarNo; i++)
	{
		Vehicle* car =  &cars[i];
		//printf("Car %d is going in direction %d \n", car->id, car->direction);
		pthread_join(thread_id[i], NULL);
	}
	
	return 0;
}

void* oneVehicle(void* ptr)
{
	Vehicle* car = (Vehicle*) ptr;
	//printf("Car %d is going in direction %d\n", car->id, car->direction);
 	ArriveBridge(car);
	CrossBridge(car);
	ExitBridge(car);
	return car;
}

void ArriveBridge(Vehicle* car){
	pthread_mutex_lock(&directionMutex);
	std::cout << "Car " << car->id << " arrives traveling direction " << car->direction << std::endl;
	if(currentCars > 3 || currentDir != car->direction){
		std::cout << "Car " << car->id << " waits to travel in direction " << car->direction << std::endl;
		if(car->direction == 0){
			pthread_cond_wait(&spaceZeroOpen, &directionMutex);
		}
		else{
			pthread_cond_wait(&spaceOneOpen, &directionMutex);
		}
	}
	currentCars ++;
	pthread_mutex_unlock(&directionMutex);
}

void CrossBridge(Vehicle* car){
	pthread_mutex_lock(&directionMutex);
	std::cout << "Car " << car->id << " crossing bridge in direction " << car->direction << std::endl;
	pthread_mutex_unlock(&directionMutex);
}

void ExitBridge(Vehicle* car){
	pthread_mutex_lock(&directionMutex);
	currentCars--;

	if(currentCars == 0){
		car->direction = (car->direction == 0) ? 1 : 0;
	}
	if(car->direction == 0){
		pthread_cond_signal(&spaceZeroOpen);
	}
	else{
		pthread_cond_signal(&spaceOneOpen);
	}
	pthread_mutex_unlock(&directionMutex);
}

void processFile()
{
	std::ifstream file ("text.txt");
	if (file.is_open())
	{
		string info;
		getline(file, info);
		totalCarNo = atoi(info.c_str());
		//printf("%d \n", info.c_str());
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
