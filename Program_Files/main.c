# include <stdio.h> 
# include <stdlib.h>
# include <string.h>

# include "passes.h"

int main()
{
	firstPass();					//reads input.asm and generates symTable.txt and opTable.txt
	secondPass();					//reads input.asm and generates output.o using symTable.txt and opTable.txt 

	return 0;
}
