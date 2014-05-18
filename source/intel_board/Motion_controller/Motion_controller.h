/*

*/

#ifndef _MOTION_CONTROLLER_H
#define _MOTION_CONTROLLER_H

#include "Car_controller.h"
#include "Controller_Com.h"

class Motion_controller
{
private:
	static queue cmd_queue;
	static Controller_Com Com;
public:
	Motion_controller();
	~Motion_controller();
	static void cal_cmd();
	static void send_cmd();
};

#endif
