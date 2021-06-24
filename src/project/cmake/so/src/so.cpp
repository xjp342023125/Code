#include "so.h"
#include <cstdio>
#include  "lib.h"
void so_in1(){
	printf("so_in1\n");
	
}


void so1(){
	printf("so1\n");
	printf("call lib1\n");
	lib1();
}

void so2(){
	printf("so2\n");
}



void so_class1::so_class_f1()
{
	printf("so_class1::so_class_f1\n");

}

