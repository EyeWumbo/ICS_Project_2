#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <sstream>
#include "Bridge.hpp"

using namespace std;
static std::stringstream ss_log;

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
	sleep(1);
	return car;
}

void ArriveBridge(Vehicle* car){
	pthread_mutex_lock(&directionMutex);
	ss_log << "Car " << car->id << " arrives traveling direction " << car->direction << std::endl;
	printf("%s", ss_log.str().c_str());
	if(currentCars == 0){
		currentDir = car->direction;
	}
	if(car->direction == currentDir){
		if(currentCars < 3){
			currentCars++;
		}
		else{
			ss_log << "Car " << car->id << " waits to travel in direction " << car->direction << std::endl;
			printf("%s", ss_log.str().c_str());
			pthread_cond_wait(&queueSlotAvailable, &directionMutex);
			currentCars++;
		}
	}
	if(car->direction != currentDir){
		ss_log << "Car " << car->id << " waits to travel in direction " << car->direction << std::endl;
		printf("%s", ss_log.str().c_str());
		pthread_cond_wait(&allCarsPassed, &queueMutex);
	}	
	pthread_mutex_unlock(&directionMutex);
}

void CrossBridge(Vehicle* car){
	pthread_mutex_lock(&directionMutex);
	ss_log << "Car " << car->id << " crossing bridge in direction " << car->direction << std::endl;
	printf("%s", ss_log.str().c_str());
	pthread_mutex_unlock(&directionMutex);
}

void ExitBridge(Vehicle* car){
	pthread_mutex_lock(&queueMutex);
	currentCars--;
	pthread_cond_signal(&queueSlotAvailable);
	if(currentCars == 0){
		currentDir = (currentDir == 0) ? 1 : 0;
		pthread_cond_signal(&allCarsPassed);
	}
	pthread_mutex_unlock(&queueMutex);
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
	ss_log << "ERROR: File Not Found" << std::endl;
	printf("%s", ss_log.str().c_str());
	printf("%d \n", totalCarNo);
}
