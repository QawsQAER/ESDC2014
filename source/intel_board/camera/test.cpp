#include <iostream>
#include <cstdlib>
#include <cstring>
using namespace std;


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

int main()
{

char buffer[20];       
int  i = 1234;   
my_itoa( i, buffer);
string s(buffer); 

cout<<buffer<<endl;

return 0;

}
     