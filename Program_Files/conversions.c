# include <stdio.h> 
# include <stdlib.h>
# include <string.h>

# include "conversions.h"

//converts decimal digit to 4 digit binary
char * binary(int deciNum)
{
	
	char * binaryCode[16]= {"0000", "0001", "0010", "0011", "0100", "0101",
	"0110", "0111", "1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111"}; 

	return binaryCode[deciNum];
}

//converts Hexadecimal character to 4 digit binary
char * hexaToBinary(char symbol)
{
	char * binaryCode[16]= {"0000", "0001", "0010", "0011", "0100", "0101",
	"0110", "0111", "1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111"};

	if ((symbol >= '0') && (symbol <= '9'))
	{
		symbol -= '0';
	}
	else
		symbol -= 'A' - 10;

	return binaryCode[symbol];

}

//converts decimal number to 4 digit hexadecimal
char * decToHexa(int deciNum) 
{
    char hexaDeciNum[5]; 

	int i = 0; 
	int remainder  = 0; 
	while(deciNum != 0)
	{		
		remainder = deciNum % 16; 
		
		if(remainder < 10) 
		{ 
			hexaDeciNum[i] = remainder + 48; 
			i++; 
		} 
		else
		{ 
			hexaDeciNum[i] = remainder + 55; 
			i++; 
		} 
          
		deciNum /= 16; 
	} 

	char * returnBuffer = (char *)malloc(4*sizeof(char));

	for(int j=3; j>=0; j--) 						//Printing in reverse order
	{
		if(j<=i-1)
			returnBuffer[3-j] = hexaDeciNum[j]; 	
		else
			returnBuffer[3-j] = '0';  
	}	
	returnBuffer[4] = 0;

	return returnBuffer;
} 



char * decToBinary(int deciNum) 
{ 
    char hexaDeciNum[6]; 

	int i = 0; 
	int remainder  = 0; 
	while(deciNum != 0)
	{		
		remainder = deciNum % 2; 

		hexaDeciNum[i] = remainder+ '0'; 
		i++; 
          
		deciNum /= 2; 
		
	} 

	char * returnBuffer = (char *)malloc(6*sizeof(char));

	for(int j=4; j>=0; j--) 						//Printing in reverse order
	{
		if(j<=i-1)
			returnBuffer[4-j] = hexaDeciNum[j]; 	
		else
			returnBuffer[4-j] = '0';
	}	
	returnBuffer[5] = 0;

	return returnBuffer;
}