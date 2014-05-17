/*

*/

#ifndef _MOTION_CONTROLLER_H
#define _MOTION_CONTROLLER_H

#include "Car_controller.h"

class Motion_controller
{
private:
	static Car_controller car;
	static queue cmd_queue;
public:
	Motion_controller();
	~Motion_controller();
	static void cal_cmd();
	static void send_cmd();
};

#endif