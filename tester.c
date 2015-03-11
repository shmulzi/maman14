#include "defines.h"

int assemble(int rone, int rtwo)
{
	printf("rone = %d, rtwo = %d\n",rone,rtwo);
	int result;
	
	int tmpone, tmptwo;
	
	tmpone = rone;
	tmpone = tmpone << 7;
	
	tmptwo = rtwo;
	tmptwo = tmptwo << 2;
	
	printf("tmpone = %d, tmptwo = %d\n",tmpone, tmptwo);
	
	result = tmpone | tmptwo;
	
	printf("result = %d\n",regccsult);
	
	return result;

}


int main()
{
	int i = assemble(R1,R4);
	printf("%X\n",i);
}
