# include <stdio.h> 
# include <stdlib.h>
# include <string.h>

# include "passes.h"
# include "conversions.h"

void firstPass()
{
	char availInstr[12][5] = {"MOV", "ADD", "SUB", "MUL", "CMP", 
	"AND", "OR", "NOT", "JMP", "LOOP", "HLT", "JNZ"
	};
	int memoryOfInstr[12] = {4, 4, 4, 2, 4, 2, 2, 1, 2, 4, 0, 2};
	int isDuplicate[12] = {0};

	FILE *inputPointer = fopen("input.asm", "r") ; 
	if (inputPointer == NULL)
	{
		printf("File \"input.asm\" doesn't exist\n");	
		return ;
	}
	FILE *symTable = fopen("symTable.txt", "w+");
	FILE *opTable = fopen("opTable.txt", "w+");


	char instr[5];
	int deciAddr = 0;
	char isLabel;
	while( fscanf(inputPointer, "%s", instr) == 1 )								//first pass
	{

		if ( (strcasecmp(instr, "START") == 0) || (strcasecmp(instr, "END") == 0))
		{
			continue;
		}

		int index;
		for (index = 0; index < 12; index++)
		{
			if (strcasecmp(availInstr[index], instr) == 0)
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
				if (strcasecmp(instr, "LOOP") == 0)										//just to adjust tabs in opTable.txt
					fprintf(opTable, "%s\t\t%s\n", instr, binary(index));
				else
					fprintf(opTable, "%s\t\t\t%s\n", instr, binary(index));
				isDuplicate[index] = 1;
			}
		}
	}

	fclose(inputPointer);
	fclose(symTable);
	fclose(opTable);

}

void secondPass()
{
	char availInstr[12][5] = {"MOV", "ADD", "SUB", "MUL", "CMP", 
	"AND", "OR", "NOT", "JMP", "LOOP", "HLT", "JNZ"
	};
	int memoryOfInstr[12] = {4, 4, 4, 2, 4, 2, 2, 1, 2, 4, 0, 2};


	FILE *inputPointer = fopen("input.asm", "r") ; 
	if (inputPointer == NULL)
	{
		printf("File \"input.asm\" doesn't exist\n");	
		return ;
	}
	FILE *symTable = fopen("symTable.txt", "r");
	FILE *opTable = fopen("opTable.txt", "r");
	FILE *output = fopen("output.o", "w");
	fprintf(output, "ADDRESS\t\tINSTRUCTION\n");


	char instr[5];
	int deciAddr = 0;
	char wordType;
	char isFinished;
	int a0,a1,a2,a3;
	char sym[5];																//Reads contents of symbol tabel
	while( fscanf(inputPointer, "%s", instr) == 1 )								//Second Pass
	{

		if ( (strcasecmp(instr, "START") == 0) || (strcasecmp(instr, "END") == 0))
		{
			continue;
		}

		int index;
		for (index = 0; index < 12; index++)
		{
			if (strcasecmp(availInstr[index], instr) == 0)
				break;
		}


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
							fprintf(output, "%s%s %s%s\n", binary(a3), binary(a2), binary(a1), binary(a0));
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
		else if (strcasecmp(instr, "LOOP") == 0)
		{
			fprintf(output, "%s\t\t0010  11111  00000000  00000001\n",decToHexa(deciAddr));
			deciAddr += memoryOfInstr[2];
			fprintf(output, "%s\t\t1011  ",decToHexa(deciAddr));
			deciAddr += memoryOfInstr[11];
			
			continue;
		}
		else
		{		
			fprintf(output, "%4s\t\t%s  ",decToHexa(deciAddr), binary(index));
			deciAddr += memoryOfInstr[index];

			if (strcasecmp(instr, "MUL") == 0)
			{
				fprintf(output, "%s  ",decToBinary(1));
			}
		}
	}
	
	fclose(inputPointer);
	fclose(symTable);
	fclose(opTable);
	fclose(output);
}