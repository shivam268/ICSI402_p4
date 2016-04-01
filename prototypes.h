/*
Parikh Shivam
ICSI 402
Assignment 4
UNIX ID: sp191221
*/

     
        
void setUpSymbolTable(char *fileName); // sets up the symbol table

void addSymbolList(char *label, int lc); // add symbol on to list

void setUpOpcode(); // setup the opcode table size of 36

int checkMultiplyImp(char *label); // check if the symbol is multiply defined or not

void printSymbol(FILE* filename); // write symbols onto file

void validOpcode(char *validityCheck); // check if the opcode is valid or not

void opCodeProcess1(char *opcodeLine); // process for second pass

void addInvalidOpcode(); // add invalid opcode will add line number

void writeErrorOnFile(FILE *asmFile, FILE *errorLog); // populates the error file

int returnOpcode(char *opcodeName); // returns the opcode for the given mnemonic

void addUndefined(char *undefined); // add's undefined symbols on the list

int returnLC(char *labelForLc); // returns lc value for given label

int checkMulDef(char *label); // checking just defined before or not
