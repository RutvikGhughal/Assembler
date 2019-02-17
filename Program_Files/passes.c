# include <stdio.h> 
# include <stdlib.h>
# include <string.h>

# include "passes.h"
# include "conversions.h"


//reads input.asm and generates symTable.txt and opTable.txt
void firstPass()						
{
	char availInstr[12][5] = {"MOV", "ADD", "SUB", "MUL", "CMP", 		//stores all possible insturctions
	"AND", "OR", "NOT", "JMP", "LOOP", "HLT", "JNZ"
	};
	int memoryOfInstr[12] = {4, 4, 4, 2, 4, 2, 2, 1, 2, 4, 0, 2};		//Memory occupied by each instruction
												

	FILE *inputPointer = fopen("input.asm", "r") ; 
	if (inputPointer == NULL)
	{
		printf("File \"input.asm\" doesn't exist\n");	
		return ;
	}
	FILE *symTable = fopen("symTable.txt", "w+");
	FILE *opTable = fopen("opTable.txt", "w+");


	char instr[5];														//Stores instructions word by word
	int deciAddr = 0;													//Stores decimal address of current instruction
	int isDuplicate[12] = {0};											//To avoid putting duplicates in opTable
	while( fscanf(inputPointer, "%s", instr) == 1 )								//first pass
	{

		if ( (strcasecmp(instr, "START") == 0) || (strcasecmp(instr, "END") == 0))			//ignore START and END
		{
			continue;
		}

		int index;												//stores the index of instruction found in availInstr[]
		for (index = 0; index < 12; index++)
		{
			if (strcasecmp(availInstr[index], instr) == 0)
				break;
		}

		char isLabel;
		char labeltype;
		if (index == 12)									//Then the string must be a label
		{
			isLabel = '1';									//erase previously stored
			labeltype = '1';								//erase previously stored
			sscanf(instr, "%c%*d%c", &isLabel, &labeltype);

			if (isLabel == 'L')
			{
				if (labeltype == ':')						//if of form L1:
				{	
					fprintf(symTable, "%s\t\t%s\n", instr, decToHexa(deciAddr));	
					continue;
				}
				else
					deciAddr += 1;							//if of form L1
			}
		}

		else												//If it is an instruction word (store in opTable)
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
	char availInstr[12][5] = {"MOV", "ADD", "SUB", "MUL", "CMP", 				//stores all possible insturctions
	"AND", "OR", "NOT", "JMP", "LOOP", "HLT", "JNZ"
	};
	int memoryOfInstr[12] = {4, 4, 4, 2, 4, 2, 2, 1, 2, 4, 0, 2};				//Memory occupied by each instruction


	FILE *inputPointer = fopen("input.asm", "r"); 
	if (inputPointer == NULL)
	{
		printf("File \"input.asm\" doesn't exist\n");	
		return ;
	}
																			//Open all needed files
	FILE *symTable = fopen("symTable.txt", "r");
	FILE *opTable = fopen("opTable.txt", "r");
	FILE *output = fopen("output.o", "w");
	fprintf(output, "ADDRESS\t\tINSTRUCTION\n");


	char instr[5];
	int deciAddr = 0;					
	char wordType;						//used to see if instr is a label or a register
	char isFinished;					//used to store ',' which indicates line not finished
	char a0,a1,a2,a3;					//Used to store addresses word-by-word															
	char sym[5];						//Reads contents of symbol table
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
				if (labeltype == ':')			//ignore if label identifier
					continue;
				else 							//when L1 is where you go to	
				{	
					while( fscanf(symTable, "%s", sym) == 1 )		//Search from symbol table
					{
						sscanf(sym, "%*c%d%*c", &symNumber);		//get label number of that symbol in table
						if (number == symNumber)					//if we get that label
						{
							fscanf(symTable, "%s", sym);			//get next stored word(which is the required address)
																	//output in the required format
							sscanf(sym, " %c%c%c%c", &a3, &a2, &a1, &a0);
							fprintf(output, "%s%s %s%s\n", hexaToBinary(a3), hexaToBinary(a2), hexaToBinary(a1), hexaToBinary(a0));
							break;
						}

					}
					
					deciAddr += 1;									//memory label takes
					continue;
				}
			
			}

			sscanf(instr, "%c%d%c", &wordType, &number,&isFinished);
			if (wordType == 'R')									//if it's a register
			{
				fprintf(output, "%s  ", decToBinary(number));
				if (isFinished != ',')								//if not followed by ',' print newline
				{
					fprintf(output, "\n");
				}
			
				continue;
			}

			sscanf(instr, " %c%c%c%c%c", &a3, &a2, &a1, &a0, &wordType);  // if the word is a hexadecimal number
			if (wordType == 'H')
			{
				fprintf(output, "%s%s  %s%s\n", hexaToBinary(a3), hexaToBinary(a2), hexaToBinary(a1), hexaToBinary(a0));
			}

		}
		else if (strcasecmp(instr, "LOOP") == 0)			//if "LOOP", do "SUB R31, 0001H" and JNZ 
		{
			fprintf(output, "%s\t\t0010  11111  00000000  00000001\n",decToHexa(deciAddr));		//SUB R31, 0001H
			deciAddr += memoryOfInstr[2];
			fprintf(output, "%s\t\t1011  ",decToHexa(deciAddr));							//JNZ 
			deciAddr += memoryOfInstr[11];
			
			continue;
		}
		else
		{	
			//if instr is instruction word print address and instr
			fprintf(output, "%4s\t\t%s  ",decToHexa(deciAddr), binary(index));	
			deciAddr += memoryOfInstr[index];

			//if the instruction is MUL add R1 to output
			if (strcasecmp(instr, "MUL") == 0)		//MUL RN == MUL R1, RN
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