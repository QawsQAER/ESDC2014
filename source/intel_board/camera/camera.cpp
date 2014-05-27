// http://zhidao.baidu.com/link?url=ZP_yu6SqfLRxltc0nulVHbGPOD3CIXukjxKa1jc4sTvcjgikJPOtDHzN3HaMgJDhAEmWDcE9KFC3wRdDtm-8yA-vA7f8Yir_v0ZxjvRPvsi 

#include "camera.h"
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <string>


#include <stdio.h>
#include <sys/socket.h> 
#include <sys/types.h> 
#include <time.h> 
#include <netdb.h>
#include <errno.h> 
#include <signal.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <sys/wait.h> 
#include <sys/time.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 




#define DEBUG_MODE 1

using namespace std;

int my_itoa(int val, char* buf);



Camera::Camera()
{
    path_temp = PATH_TEMP;
    path_capture = PATH_CAPTURE;
    count_temp_photo=0;
    count_capture_photo=0;
    ip=IP_PORT;
}

Camera::~Camera()
{
     cout<<"camera instance destroyed"<<endl;
}

void Camera::setip(string ip_address)
{
    ip=ip_address;
}

string Camera::photo()
{
     if(DEBUG_MODE) cout<<"Function:photo()"<<endl;

        int temp_count = count_temp_photo;
          char buffer[20];   
        count_temp_photo++;
        my_itoa(temp_count,buffer);

        string temp(buffer);

        cout << temp<<endl;

     if(DEBUG_MODE) cout<<"Create command"<<endl;

        string path_temp_function = path_temp+temp+".jpg";

             if(DEBUG_MODE) cout<<"set path finished"<<endl;

       string str_command="wget  -O "+path_temp_function+" http://"+ip+"/photo.jpg";

     if(DEBUG_MODE) cout<<"convert command"<<endl;

      const char* command=str_command.c_str();

      if(DEBUG_MODE) cout<<"wget command"<<" "<<command<<endl;

        if ((fp = popen(command, "r")) == NULL) {// fp = popen("wget  -O ~/a.jpg http://192.168.43.1:8080/photoaf.jpg", "r"))
                perror("popen failed");
                // return -1;
            }
          
            if (pclose(fp) == -1) {
                perror("pclose failed");
                // return -2;
            }

        return path_temp_function;
}

string Camera::photo_af()
{
    if(DEBUG_MODE) cout<<"Function:photoaf()"<<endl;

        int temp_count = count_temp_photo;
          char buffer[20];   
        count_temp_photo++;
        my_itoa(temp_count,buffer);

        string temp(buffer);

        cout << temp<<endl;

     if(DEBUG_MODE) cout<<"Create command"<<endl;

        string path_temp_function = path_temp+temp+".jpg";

             if(DEBUG_MODE) cout<<"set path finished"<<endl;

       string str_command="wget  -O "+path_temp_function+" http://"+ip+"/photoaf.jpg";

     if(DEBUG_MODE) cout<<"convert command"<<endl;

      const char* command=str_command.c_str();

      if(DEBUG_MODE) cout<<"wget command"<<" "<<command<<endl;

        if ((fp = popen(command, "r")) == NULL) {// fp = popen("wget  -O ~/a.jpg http://192.168.43.1:8080/photoaf.jpg", "r"))
                perror("popen failed");
                // return -1;
            }
          
            if (pclose(fp) == -1) {
                perror("pclose failed");
                // return -2;
            }

        return path_temp_function;
}

string Camera::photo_JPEG()
{

        if(DEBUG_MODE) cout<<"Function:photo_JPEG()"<<endl;

        int temp_count = count_temp_photo;
          char buffer[20];   
        count_temp_photo++;
        my_itoa(temp_count,buffer);

        string temp(buffer);

        cout << temp<<endl;

     if(DEBUG_MODE) cout<<"Create command"<<endl;

        string path_temp_function = path_temp+temp+".jpg";

             if(DEBUG_MODE) cout<<"set path finished"<<endl;

       string str_command="wget  -O "+path_temp_function+" http://"+ip+"/shoot.jpg";

     if(DEBUG_MODE) cout<<"convert command"<<endl;

      const char* command=str_command.c_str();

      if(DEBUG_MODE) cout<<"wget command"<<" "<<command<<endl;

        if ((fp = popen(command, "r")) == NULL) {// fp = popen("wget  -O ~/a.jpg http://192.168.43.1:8080/photoaf.jpg", "r"))
                perror("popen failed");
                // return -1;
            }
          
            if (pclose(fp) == -1) {
                perror("pclose failed");
                // return -2;
            }

        return path_temp_function;
}


string Camera::take_photo_af()
{
              if(DEBUG_MODE) cout<<"Function:take_photo_af()"<<endl;

       int temp_count = count_capture_photo;
          char buffer[20];   
        count_capture_photo++;
        my_itoa(temp_count,buffer);

        if(DEBUG_MODE) cout<<"After my_itoa()"<<endl;
        string temp(buffer);



        string path_capture = path_capture+temp+".jpg";
     if(DEBUG_MODE) cout<<"Create command"<<endl;

        string str_command="wget  -O "+path_capture+"http://"+ip+"/photoaf.jpg";
      const char* command=str_command.c_str();

   if(DEBUG_MODE) cout<<"wget command"<<endl;
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



int Camera::test_connection()
{


    int sd=socket(AF_INET,SOCK_STREAM,0); 
    struct sockaddr_in server_addr; 
    memset(&server_addr,0,sizeof(server_addr)); 
    server_addr.sin_family=AF_INET; 
    string ip=IP;
    string port=PORT;
    server_addr.sin_addr.s_addr=inet_addr(ip.c_str()); 
    server_addr.sin_port=htons(atoi(port.c_str())); 
    if(connect(sd,(struct sockaddr *)&server_addr,sizeof(server_addr))<0){ 
        printf("connection error: %s (Errno:%d)\n",strerror(errno),errno); 
       return -1;
    } 
    printf("connect success \n"); 


    char str1[4096];
    memset(str1, 0, 4096); 
    int ret;
    sprintf(str1,"GET / HTTP/1.0\r\nHost:%s\r\nContent-Type=text/plain\r\nConnection:Close\r\n\r\n",ip.c_str());   
    ret = send(sd,(void *)str1,strlen(str1),0); 
    if (ret < 0) 
    { 
        printf("send error %d，Error message'%s'\n",errno, strerror(errno)); 
        return -1;
       
    }
    else
    { 
        printf("send success ,total send %d \n", ret); 
    } 
    

        return 1;
}


void Camera::zoom(float scaler){
// (scaler>1 in) (scaler< 1 out)

//conver scaler to index
    int index=0;
    
        if(scaler-1<0.1) index=0;  
       else if(scaler-1.02<0.1) index=1;  
       else  if(scaler-1.04<0.1) index=2;  
        else if(scaler-1.09<0.1) index=3;  
         else if(scaler-1.11<0.1) index=4;  
         else if(scaler-1.13<0.1) index=5;  
         else if(scaler-1.19<0.1) index=6;  
          else if(scaler-1.21<0.1) index=7;  
         else if(scaler-1.24<0.1) index=8;  
         else if(scaler-1.31<0.1) index=9;  
         else if(scaler-1.34<0.1) index=10;  
         else if(scaler-1.38<0.1) index=11;  
         else if(scaler-1.46<0.1) index=12;  
          else if(scaler-1.5<0.1) index=13;  
         else if(scaler-1.55<0.1) index=14;  
         else if(scaler-1.59<0.1) index=15;  
         else if(scaler-1.65<0.1) index=16;  
         else if(scaler-1.7<0.1) index=17;  
         else if(scaler-1.82<0.1) index=18;  
          else if(scaler-1.89<0.1) index=19;  
         else if(scaler-2<0.1) index=20;  
         else if(scaler-2.13<0.1) index=21;  
         else if(scaler-2.22<0.1) index=22;  
         else if(scaler-2.32<0.1) index=23;  
         else if(scaler-2.43<0.1) index=24;  
             else if(scaler-2.55<0.1) index=25;  
         else if(scaler-2.83<0.1) index=26;  
         else if(scaler-3<0.1) index=27;  
         else if(scaler-3.19<0.1) index=28;  
         else if(scaler-3.64<0.1) index=29;  
         else if(scaler-4<0.1) index=30;  
         else {cout<< "Error:scaler error"<<endl;
                 exit(0);}



  if(DEBUG_MODE) cout<<"Create command"<<endl;

     char buffer[20];   
        count_capture_photo++;
        my_itoa(index,buffer);

        string temp(buffer);

        string str_command="wget http://"+ip+"/ptz?zoom="+temp;
      const char* command=str_command.c_str();

   if(DEBUG_MODE) cout<<"wget command"<<" " << command<<endl;

        if ((fp = popen(command, "r")) == NULL) {// fp = popen("wget  -O ~/a.jpg http://192.168.43.1:8080/photoaf.jpg", "r"))
                perror("popen failed");
                // return -1;
            }
          
            if (pclose(fp) == -1) {
                perror("pclose failed");
                // return -2;
            }



}
    void Camera::flash_open()
    {


     if(DEBUG_MODE) cout<<"Create command"<<endl;

        string str_command="wget http://"+ip+"/enabletorch";
      const char* command=str_command.c_str();

   if(DEBUG_MODE) cout<<"wget command"<<" " << command<<endl;

        if ((fp = popen(command, "r")) == NULL) {// fp = popen("wget  -O ~/a.jpg http://192.168.43.1:8080/photoaf.jpg", "r"))
                perror("popen failed");
                // return -1;
            }
          
            if (pclose(fp) == -1) {
                perror("pclose failed");
                // return -2;
            }




    }

    void Camera::flash_close()
    {



     if(DEBUG_MODE) cout<<"Create command"<<endl;

        string str_command="wget http://"+ip+"/disabletorch";
      const char* command=str_command.c_str();

   if(DEBUG_MODE) cout<<"wget command"<<" " << command<<endl;

        if ((fp = popen(command, "r")) == NULL) {// fp = popen("wget  -O ~/a.jpg http://192.168.43.1:8080/photoaf.jpg", "r"))
                perror("popen failed");
                // return -1;
            }
          
            if (pclose(fp) == -1) {
                perror("pclose failed");
                // return -2;
            }
    }

    void Camera::af_open()
    {


 if(DEBUG_MODE) cout<<"Create command"<<endl;

        string str_command="wget http://"+ip+"/focus";
      const char* command=str_command.c_str();

   if(DEBUG_MODE) cout<<"wget command"<<" " << command<<endl;

        if ((fp = popen(command, "r")) == NULL) {// fp = popen("wget  -O ~/a.jpg http://192.168.43.1:8080/photoaf.jpg", "r"))
                perror("popen failed");
                // return -1;
            }
          
            if (pclose(fp) == -1) {
                perror("pclose failed");
                // return -2;
            }

    }


    void Camera::af_close()
    {

if(DEBUG_MODE) cout<<"Create command"<<endl;

        string str_command="wget http://"+ip+"/nofocus";
      const char* command=str_command.c_str();

   if(DEBUG_MODE) cout<<"wget command"<<" " << command<<endl;

        if ((fp = popen(command, "r")) == NULL) {// fp = popen("wget  -O ~/a.jpg http://192.168.43.1:8080/photoaf.jpg", "r"))
                perror("popen failed");
                // return -1;
            }
          
            if (pclose(fp) == -1) {
                perror("pclose failed");
                // return -2;
            }





    }






int my_itoa(int val, char* buf)
{
        // if(DEBUG_MODE) cout<<"Function:my_itoa()"<<endl;

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
        // if(DEBUG_MODE) cout<<"finished my_itoa()"<<endl;
        return len;
}

