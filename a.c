# include <stdio.h> 
# include <stdlib.h>
# include <string.h>

char * binary(int deciNum);
char * decToHexa(int deciNum) ;
char * decToBinary(int n) ;

int main()
{
	FILE *inputPointer = fopen("input.asm", "r") ; 
	if (inputPointer == NULL)
	{
		printf("File \"input.asm\" doesn't exist\n");	
		return 1;
	}

	char availInstr[12][5] = {"MOV", "ADD", "SUB", "MUL", "CMP", 
	"AND", "OR", "NOT", "JMP", "LOOP", "HLT", "JNZ"
	};
	int memoryOfInstr[12] = {4, 4, 4, 2, 4, 2, 2, 1, 4, 4, 0, 4};
	int isDuplicate[12] = {0};
	
	FILE *symTable = fopen("symTable.txt", "w+");
	FILE *opTable = fopen("opTable.txt", "w");


	char instr[5];
	int deciAddr = 0;
	char isLabel;
	while( fscanf(inputPointer, "%s", instr) == 1 )								//first pass
	{

		if ( (strcmp(instr, "START") == 0) || (strcmp(instr, "END") == 0))
		{
			continue;
		}

		int index;
		for (index = 0; index < 12; index++)
		{
			if (strcmp(availInstr[index], instr) == 0)
				break;
		}

		int number;
		char isLabel;
		char labeltype;
		if (index == 12)									//Then the string must be a label
		{
			isLabel = '1';
			labeltype = '1';
			sscanf(instr, "%c%*d%c", &isLabel, &labeltype);

			if (isLabel == 'L')
			{
				if (labeltype == ':')
				{	
					fprintf(symTable, "%s\t\t%s\n", instr, decToHexa(deciAddr));	
					continue;
				}
				else
					deciAddr += 1;
			}
		}

		else
		{	
			deciAddr += memoryOfInstr[index];
			if(!isDuplicate[index])
			{
				fprintf(opTable, "%s\t\t%s\n", instr, binary(index));
				isDuplicate[index] = 1;
			}
		}

		//printf("%s\n", instr );
	}

	rewind(symTable);
	rewind(inputPointer);
	FILE *output = fopen("output.o", "w");
	fprintf(output, "ADDRESS\t\tINSTRUCTION\n");

	deciAddr = 0;
	char wordType;
	char isFinished;
	int a0,a1,a2,a3;
	char sym[5];																//Reads contents of symbol tabel
	while( fscanf(inputPointer, "%s", instr) == 1 )								//Second Pass
	{

		if ( (strcmp(instr, "START") == 0) || (strcmp(instr, "END") == 0))
		{
			continue;
		}

		int index;
		for (index = 0; index < 12; index++)
		{
			if (strcmp(availInstr[index], instr) == 0)
				break;
		}


		isLabel = '1';
		wordType = '1';
		isFinished = '1';
		int number;
		char isLabel;
		char labeltype;
		int symNumber;
		if (index == 12)									//Then the string must be a label
		{

			isLabel = '1';
			labeltype = '1';
			sscanf(instr, "%c%d%c", &isLabel, &number, &labeltype);
			if (isLabel == 'L')
			{
				if (labeltype == ':')
					continue;
				else 
				{	
					while( fscanf(symTable, "%s", sym) == 1 )
					{
						sscanf(sym, "%*c%d%*c", &symNumber);
						if (number == symNumber)
						{
							fscanf(symTable, "%s", sym);
							sscanf(sym, " %1d%1d%1d%1d", &a3, &a2, &a1, &a0);
							fprintf(output, "%s %s %s %s\n", binary(a3), binary(a2), binary(a1), binary(a0));
							break;
						}

					}
					
					deciAddr += 1;
					continue;
				}
			
			}

			sscanf(instr, "%c%d%c", &wordType, &number,&isFinished);
			if (wordType == 'R')
			{
				fprintf(output, "%s  ", decToBinary(number));
				if (isFinished != ',')
				{
					fprintf(output, "\n");
				}
			
				continue;
			}

			sscanf(instr, " %1d%1d%1d%1d%c", &a3, &a2, &a1, &a0, &wordType);
			if (wordType == 'H')
			{
				fprintf(output, "%s%s  %s%s\n", binary(a3), binary(a2), binary(a1), binary(a0));
			}

		}
		else if (strcmp(instr, "LOOP") == 0)
		{
			fprintf(output, "%s\t\t0010  11111  00000000  00000001\n",decToHexa(deciAddr));
			deciAddr += memoryOfInstr[2];
			fprintf(output, "%s\t\t1011  ",decToHexa(deciAddr));
			
			continue;
		}
		else
		{		
			fprintf(output, "%s\t\t%s  ",decToHexa(deciAddr), binary(index));
			deciAddr += memoryOfInstr[index];

			if (strcmp(instr, "MUL") == 0)
			{
				fprintf(output, "%s  ",decToBinary(1));
			}
		}

		
	}

	return 0;
}

char * binary(int deciNum)
{

	char * binaryCode[16]= {"0000", "0001", "0010", "0011", "0100", "0101",
	"0110", "0111", "1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111"}; 

	return binaryCode[deciNum];
}

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

	return returnBuffer;
}