#include<stdio.h>
#include<stdlib.h>

int main(void){
	
	int key = 123456;
	int hl;
	printf("enter key: ");
	scanf("%d",&hl);
	
	if (hl == key){
		printf("Hurray!!");
	}
	else{
		printf("sorry!!");
	}
	
	return 0;

	
}