#include <iostream>
#include <string>
#include <fstream>
using namespace std;
int main()
{
	fstream file("bridge.in");
	string info;
	getline(file, info);
	std::cout<<info <<"Dam "<<endl;
	return 0;
}
