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
	return car;
}

void ArriveBridge(Vehicle* car){
	pthread_mutex_lock(&directionMutex);
<<<<<<< HEAD
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
=======
	std::cout << "Car " << car->id << " arrives traveling direction " << car->direction << std::endl;
	if(currentCars >= 3 || currentDir != car->direction){
		std::cout << "Car " << car->id << " waits to travel in direction " << car->direction << std::endl;
		if(car->direction == 0){
			carsWaitingZero += 1;
			pthread_cond_wait(&spaceZeroOpen, &directionMutex);
			carsWaitingZero -= 1;
		}
		else{
			carsWaitingOne += 1;
			pthread_cond_wait(&spaceOneOpen, &directionMutex);
			carsWaitingOne -= 1;	
		}
	}
	currentCars ++;
>>>>>>> FETCH_HEAD
	pthread_mutex_unlock(&directionMutex);
}

void CrossBridge(Vehicle* car){
	pthread_mutex_lock(&directionMutex);
<<<<<<< HEAD
	ss_log << "Car " << car->id << " crossing bridge in direction " << car->direction << std::endl;
	printf("%s", ss_log.str().c_str());
=======
	std::cout << "Car " << car->id << " crossing bridge in direction " << car->direction << std::endl;
	sleep(1);
>>>>>>> FETCH_HEAD
	pthread_mutex_unlock(&directionMutex);
}

void ExitBridge(Vehicle* car){
	pthread_mutex_lock(&directionMutex);
	currentCars--;
	if(currentCars == 0){
		if(car->direction == 0){
			if(carsWaitingZero == 0){
				std::cout << "Traffic Direction is being changed to 1" << std::endl;
				currentDir = 1;
			}
		}
		if(car->direction == 1){
			if(carsWaitingOne == 0){
				std::cout << "Traffic Direction is being changed to 0" << std::endl;
			currentDir = 0;
			}
		}
	}
	if(currentDir == 0){
		pthread_cond_signal(&spaceZeroOpen);
	}
	else{
		pthread_cond_signal(&spaceOneOpen);
	}
	std::cout << "Car " << car->id << " exits bridge" << std::endl;
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
	ss_log << "ERROR: File Not Found" << std::endl;
	printf("%s", ss_log.str().c_str());
	printf("%d \n", totalCarNo);
}
