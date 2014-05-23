#include <stdio.h>

int main()
{
    FILE *fp;
    if ((fp = popen("wget  -O ~/Desktop/a.jpg http://192.168.43.1:8080/photoaf.jpg", "r")) == NULL) {//用“curl www.baidu.com”也是一样的
        perror("popen failed");
        return -1;
    }
    char buf[256];
    // while (fgets(buf, 255, fp) != NULL) {
    //  printf("%s", buf);
    // }
    if (pclose(fp) == -1) {
        perror("pclose failed");
        return -2;
    }
    return 0;
}