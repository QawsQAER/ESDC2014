#include "camera.h"
#include <iostream>
#include <string>

using namespace std;

int main()
{

Camera camera;
cout<<"input ip"<<endl;
string ip;
ip="192.168.43.1:8080";
// cin>>ip;
camera.setip(ip);
cout<<"setip finished"<<endl;


while(1)
{
	float scaler;
	cout<<"waiting:";
	cin>>scaler;
	camera.zoom(scaler);
	cout<<endl;

}



return 0;

}