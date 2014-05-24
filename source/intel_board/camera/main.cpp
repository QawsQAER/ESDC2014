#include "camera.h"
#include <iostream>
#include <string>

using namespace std;

int main()
{

Camera camera;
string ip;
cin>>ip;
camera.setip(ip);
camera.photo_af();



return 0;

}