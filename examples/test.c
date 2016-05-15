#include <stdio.h>

int str_contains_duplicate(char* a_str,const char a_delim){
	char* tmp = a_str;
	int count=0;
	while(*tmp){
		if(a_delim == *tmp){
			count++;
		}
		tmp++;
	}
	if(count>1)
		return 1;
	else
		return 0;
}

void main() {
	char arr[] = ",hello,";
	int contains = str_contains_duplicate(arr,',');
	printf("Contains duplicate: %d\n",contains);
	return;
}
