 
 

#include "camera.h"
#include "stdio.h"
#include "stdlib.h"

//convert int to string
int my_itoa(int val, char* buf)
{
    const int radix = 10;
    char* p;
    int a; //every digit
    int len;
    char* b; //start of the digit char
    char temp;
    p = buf;
    if (val < 0)
    {
        *p++ = '-';
        val = 0 - val;
        }
        b = p;

        do
        {
        a = val % radix;
        val /= radix;
        *p++ = a + '0';
        } while (val > 0);

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



Camera::Camera()
{
    fp= new FILE;
    path_temp = PATH_TEMP;
    path_capture = PATH_CAPTURE;
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
        string temp;
        count_temp_photo++;
        temp=to_string(temp_count);

        string path_temp = path_temp+temp+".jpg";

        string command="wget  -O "+path_temp+"http://"+ip+"/photo.jpg";

        if ((fp = popen(command, "r")) == NULL) {// fp = popen("wget  -O ~/a.jpg http://192.168.43.1:8080/photoaf.jpg", "r"))
                perror("popen failed");
                return -1;
            }
          
            if (pclose(fp) == -1) {
                perror("pclose failed");
                return -2;
            }

        return path_temp;
}

string Camera::photo_af()
{

        int temp_count = count_temp_photo;
        string temp;
        count_temp_photo++;
        temp=to_string(temp_count);

        string path_temp = path_temp+temp+".jpg";

        string command="wget  -O "+path_temp+"http://"+ip+"/photoaf.jpg";

        if ((fp = popen(command, "r")) == NULL) {// fp = popen("wget  -O ~/a.jpg http://192.168.43.1:8080/photoaf.jpg", "r"))
                perror("popen failed");
                return -1;
            }
          
            if (pclose(fp) == -1) {
                perror("pclose failed");
                return -2;
            }

        return path_temp;
}

string Camera::photo_JPEG()
{

        int temp_count = count_temp_photo;
        string temp;
        count_temp_photo++;
        temp=to_string(temp_count);



        string path_temp = path_temp+temp+".jpg";

        string command="wget  -O "+path_temp+"http://"+ip+"/shot.jpg";

        if ((fp = popen(command, "r")) == NULL) {// fp = popen("wget  -O ~/a.jpg http://192.168.43.1:8080/photoaf.jpg", "r"))
                perror("popen failed");
                return -1;
            }
          
            if (pclose(fp) == -1) {
                perror("pclose failed");
                return -2;
            }

        return path_temp;
}


string Camera::take_photo_af()
{
      
        int temp_count = count_capture_photo;
        string temp;
        count_capture_photo++;
        temp=to_string(temp_count);



        string path_capture = path_capture+temp+".jpg";

        string command="wget  -O "+count_capture_photo+"http://"+ip+"/photoaf.jpg";

        if ((fp = popen(command, "r")) == NULL) {// fp = popen("wget  -O ~/a.jpg http://192.168.43.1:8080/photoaf.jpg", "r"))
                perror("popen failed");
                return -1;
            }
          
            if (pclose(fp) == -1) {
                perror("pclose failed");
                return -2;
            }

        return count_capture_photo;
}


void Camera::zoom(float scaler);// (scaler>1 in) (scaler< 1 out)
    void Camera::flash_open();
    void Camera::flash_close();
    void Camera::af_open();
    void Camera::af_close();
