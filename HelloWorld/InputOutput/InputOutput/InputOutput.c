# include <stdio.h>
# include <stdlib.h>
int main(void){

	FILE* finput;
	FILE* foutput;
	char ch;
	char* str1[15];
	char* str2[15];
		printf("Enter input file: ");
		scanf("%s", str1);
		finput = fopen(str1, "r");
		printf("Enter output file: ");
		scanf("%s", str2);
		foutput = fopen(str2, "w");
		ch = fgetc(finput);
		while (ch != EOF){
			putc(ch, foutput);
			ch = fgetc(finput);
		}
		fclose(finput);
		fclose(foutput);
		printf("Data has been successfully copied");
		return 0;
}