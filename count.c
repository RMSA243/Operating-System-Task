#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>



int main(int argc, char *argv[])
{

    int count = 0, size = 0;

    	for (int i = 0; i < strlen(argv[1]); i++) 
	{

       		if (isalpha(argv[1][i])||isdigit(argv[1][i])) 
		{

           		count++;

       		}

        	size++;

    	}



    printf("Num. of elements of array: %i\n", count);

    printf("Array size in bytes : %lu \n", sizeof( argv[1][0] ) * size);



    return 0;
}
