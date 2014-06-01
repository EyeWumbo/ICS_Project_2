#ifndef BRIDGE_HPP
#define BRIDGE_HPP

struct Vehicle
{
	int id;
	int direction;
};

int totalCarNo, currentCars, currentDir, carsWaitingZero, carsWaitingOne;
Vehicle* cars;
pthread_mutex_t directionMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t spaceZeroOpen = PTHREAD_COND_INITIALIZER, spaceOneOpen = PTHREAD_COND_INITIALIZER;

void processFile();
void ArriveBridge(Vehicle* car);
void CrossBridge(Vehicle* car);
void ExitBridge(Vehicle* car);
void* oneVehicle(void* ptr);

bool noCarOnBridge();

#endif
