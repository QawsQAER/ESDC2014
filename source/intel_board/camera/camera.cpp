// http://zhidao.baidu.com/link?url=ZP_yu6SqfLRxltc0nulVHbGPOD3CIXukjxKa1jc4sTvcjgikJPOtDHzN3HaMgJDhAEmWDcE9KFC3wRdDtm-8yA-vA7f8Yir_v0ZxjvRPvsi 

#include "camera.h"
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <string>

using namespace std;

int my_itoa(int val, char* buf);



Camera::Camera()
{
    path_temp = PATH_TEMP;
    path_capture = PATH_CAPTURE;
    count_temp_photo=0;
    count_capture_photo=0;
}

Camera::~Camera()
{
    delete fp;
}

void Camera::setip(string ip_address)
{
    ip=ip_address;
}

string Camera::photo()
{
        int temp_count = count_temp_photo;
          char buffer[20];   
        count_temp_photo++;
        my_itoa(temp_count,buffer);
        string temp(buffer);

        string path_temp = path_temp+temp+".jpg";

    string str_command="wget  -O "+path_temp+"http://"+ip+"/photoaf.jpg";
      const char* command=str_command.c_str();

        if ((fp = popen(command, "r")) == NULL) {// fp = popen("wget  -O ~/a.jpg http://192.168.43.1:8080/photoaf.jpg", "r"))
                perror("popen failed");
                // return 0;
            }
          
            if (pclose(fp) == -1) {
                perror("pclose failed");
                // return 0;
            }

        return path_temp;
}

string Camera::photo_af()
{
        int temp_count = count_temp_photo;
          char buffer[20];   
        count_temp_photo++;
        my_itoa(temp_count,buffer);
        string temp(buffer);

        string path_temp = path_temp+temp+".jpg";

       string str_command="wget  -O "+path_temp+"http://"+ip+"/photoaf.jpg";
      const char* command=str_command.c_str();

        if ((fp = popen(command, "r")) == NULL) {// fp = popen("wget  -O ~/a.jpg http://192.168.43.1:8080/photoaf.jpg", "r"))
                perror("popen failed");
                // return -1;
            }
          
            if (pclose(fp) == -1) {
                perror("pclose failed");
                // return -2;
            }

        return path_temp;
}

string Camera::photo_JPEG()
{

        int temp_count = count_temp_photo;
          char buffer[20];   
        count_temp_photo++;
        my_itoa(temp_count,buffer);
        string temp(buffer);



        string path_temp = path_temp+temp+".jpg";

 string str_command="wget  -O "+path_temp+"http://"+ip+"/photoaf.jpg";
      const char* command=str_command.c_str();


        if ((fp = popen(command, "r")) == NULL) {// fp = popen("wget  -O ~/a.jpg http://192.168.43.1:8080/photoaf.jpg", "r"))
                perror("popen failed");
                // return -1;
            }
          
            if (pclose(fp) == -1) {
                perror("pclose failed");
                // return -2;
            }

        return path_temp;
}


string Camera::take_photo_af()
{
      
       int temp_count = count_capture_photo;
          char buffer[20];   
        count_capture_photo++;
        my_itoa(temp_count,buffer);
        string temp(buffer);



        string path_capture = path_capture+temp+".jpg";

        string str_command="wget  -O "+path_capture+"http://"+ip+"/photoaf.jpg";
      const char* command=str_command.c_str();

        if ((fp = popen(command, "r")) == NULL) {// fp = popen("wget  -O ~/a.jpg http://192.168.43.1:8080/photoaf.jpg", "r"))
                perror("popen failed");
                // return -1;
            }
          
            if (pclose(fp) == -1) {
                perror("pclose failed");
                // return -2;
            }

        return path_capture;
}


/*void Camera::zoom(float scaler);// (scaler>1 in) (scaler< 1 out)
    void Camera::flash_open();
    void Camera::flash_close();
    void Camera::af_open();
    void Camera::af_close();*/






int my_itoa(int val, char* buf)
{
        const unsigned int radix = 10;
        char* p;
        unsigned int a; //every digit
        int len;
        char* b; //start of the digit char
        char temp;
        unsigned int u;
        p = buf;
        if (val < 0)
        {
        *p++ = '-';
        val = 0 - val;
        }
        u = (unsigned int)val;
        b = p;
        do
        {
        a = u % radix;
        u /= radix;
        *p++ = a + '0';
        } while (u > 0);
        len = (int)(p - buf);
        *p-- = 0;
        //swap
        do
        {
        temp = *p;
        *p = *b;
        *b = temp;
        --p;
        ++b;
        } while (b < p);
        return len;
}

