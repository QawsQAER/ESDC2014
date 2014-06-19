enum Direction{
	UPPER_LEFT,
	LOWER_LEFT,
	UPPER_RIGHT,
	LOWER_RIGHT,
	null;
};



int absolute(int num)
{
	if(num<0)
		num=0-num;

	return num;
}

 /*******************************************************
						usage:
  degree_rotation(car_angle,phone_angle,&degree,&direction)

 car->car_angle
 phone->phone_angle
 create two int variable to store the information

 for car_angle,phone_angle,degree, they are postive values

 for direction 1 stands for rotate right
 			  -1 stands for rotate left

********************************************************/


void degree_rotation(int car,int phone,int *degree,int *direction)
{
	Direction phone_angle=null;
	Direction car_angle=null;

	int rotate_degree=0;//postive means clockwise

	if(phone<=90)
		phone_angle=LOWER_LEFT;
	else if(phone<=180)
		phone_angle=UPPER_LEFT;
	else if(phone<=270)
		phone_angle=UPPER_RIGHT;
	else 
		phone_angle=LOWER_RIGHT;


	if(car<=90)
		car_angle=LOWER_LEFT;
	else if(car<=180)
		car_angle=UPPER_LEFT;
	else if(car<=270)
		car_angle=UPPER_RIGHT;
	else 
		car_angle=LOWER_RIGHT;


		switch(car_angle)
		{
			case UPPER_LEFT:
			if(phone_angle==LOWER_RIGHT)
			{
				int temp_car=180-car;
				int temp_phone=360-phone;
				rotate_degree=temp_car-temp_phone;
				break;
			}
			else if(phone_angle==LOWER_LEFT)
			{

				rotate_degree=(180-car)+phone;
				break;
			}

			else if(phone_angle==UPPER_LEFT)
			{

				rotate_degree=360-((180-phone)+car);
				break;
			}
			else
			{

				rotate_degree=360-(car-(phone-180));
				break;
			}

			case UPPER_RIGHT:

			if(phone_angle==LOWER_LEFT)
			{
				int temp_car=car-180;
				int temp_phone=phone;
				rotate_degree=temp_phone-temp_car;
				break;
			}
			else if(phone_angle==UPPER_LEFT)
			{

				int temp_car=270-car;
				int temp_phone=phone-90;
				rotate_degree=temp_phone+temp_car;
				break;
			}
			else if(phone_angle==UPPER_RIGHT)
			{

				int temp_car=360-car;
				int temp_phone=phone;
				rotate_degree=temp_phone+temp_car;
				break;
			}
			else
			{

				int temp_car=car-180;
				int temp_phone=360-phone;
				rotate_degree=360-(temp_phone+temp_car);
				break;
			}

			case LOWER_LEFT:
			if(phone_angle==UPPER_RIGHT)
			{
				int temp_car=car;
				int temp_phone=phone-180;
				rotate_degree=temp_phone-temp_car;
				break;
			}
			else if(phone_angle==LOWER_RIGHT)
			{
				int temp_car=car;
				int temp_phone=180-(360-phone);
				rotate_degree=temp_phone-temp_car;
				break;
			}
			else if(phone_angle==LOWER_LEFT)
			{
				int temp_car=car;
				int temp_phone=180+phone;
				rotate_degree=temp_phone-temp_car;
				break;
			}

			else 
			{
				int temp_car=car;
				int temp_phone=180-phone;
				rotate_degree=360-(temp_phone+temp_car);
				break;
			}

			case LOWER_RIGHT:
			if(phone_angle==UPPER_LEFT)
			{
				int temp_car=360-car;
				int temp_phone=180-phone;
				rotate_degree=temp_car-temp_phone;
				break;
			}
			else if(phone_angle==UPPER_RIGHT)
			{
				int temp_car=360-car;
				int temp_phone=phone-180;
				rotate_degree=temp_car+temp_phone;
				break;
			}
			else if(phone_angle==LOWER_RIGHT)
			{
				int temp_car=360-car;
				int temp_phone=180-(360-phone);
				rotate_degree=temp_car+temp_phone;
				break;
			}
			else
			{
				int temp_car=360-car;
				int temp_phone=phone+180;
				rotate_degree=temp_car+temp_phone;
				break;
			}


		}//end switch


	if((rotate_degree>=0)&&(rotate_degree<=180))
 	{
 			*degree=rotate_degree;
 			*direction=1;
 	}
 	else if((rotate_degree>=180)&&(rotate_degree<=360))
 	{
 		*degree=360-rotate_degree;
 		*direction=-1;
 	}
 	else if((rotate_degree>=-360)&&(rotate_degree<=-180))
 	{
 		*degree=360-absolute(rotate_degree);
 		*direction=1;
 	}
 	else if((rotate_degree>=-180)&&(rotate_degree<=0))
 	{
 		*degree=absolute(rotate_degree);
 		*direction=-1;
 	}

}//end void


