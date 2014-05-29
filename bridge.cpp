#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include <fstream>
#include <vector>

const int Max_Car = 3;
int totalCar;

using namespace std;

struct Vehicle
{
	int Id;
	int Direction;
	
};
void* OneVehicle(void* ptr)
{
	Vehicle* car = (Vehicle *)ptr;
	ArriveBridge(car);
	CrossBridge(car);
	ExitBridge(car);
}

Vehicle* processFile(std::string filename)
{
	ifstream file (filename);
	string info;
	getline(file, info);
	totalCar = atoi(info.c_str());
	Vehicle* car = new Vehicle[totalCar];
	for(int i= 0; i < totalCar;i++)
	{
		std::getline(file, info);
		int pos = info.find(" ");
		std::string temp1 = info.substr(0,pos);
		std::string temp2 = info.substr(pos,info.length());
		int ID = atoi(temp1.c_str());
		int Direc = atoi(temp2.c_str());
		std::cout <<ID <<Direc <<std::endl;
		car[i].Id = ID;
		car[i].Direction = Direc;
	}
	return car;
}

int main()
{
	Vehicle* Car = processFile("text.txt");
	pthread_t thread_id[totalCar];
	for(int i = 0; i < totalCar; i++)
	{
		Vehicle* car =  &Car[i];
		pthread_create(&thread_id[i], NULL, OneVehicle, (void*) car);
		pthread_join(thread_id[i],NULL);
	}
	return 0;

}

