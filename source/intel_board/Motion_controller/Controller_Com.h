/*

*/

#ifndef _CONTROLLER_COM_H
#define _CONTROLLER_COM_H

class Controller_Com
{
private:
	int 
	static uint32_t write(void *buff,uint32_t length);
	static uint32_t read(void *buff,uint32_t length);
public:
	static uint8_t send_cmd(struct * IntelCarCmd);
};

#endif
