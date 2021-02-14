#include "revert_string.h"

void RevertString(char *str)
{
	int amount = strlen(str);
	char* temp = (char*)malloc((amount + 1) * sizeof(char));
	strcpy(temp, str);
	int i, j = 0;
	for (i = amount - 1; i >= 0; i--)
	{
		*(str + j) = *(temp + i);
		j++;
	}
	*(str + j) = *(temp + j);
	free(temp); 
}

