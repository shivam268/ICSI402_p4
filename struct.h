/*
Parikh Shivam
ICSI 402
Assignment 4
UNIX ID: sp191221
*/ 
        
        // struct node for symbol table
struct symbolNode{ // will hold 3 fields
  char symbol[LINESIZE]; //symbol name
  int lcValue; //lc value
  int mulDefined; // if 0 then not multiply defined
  struct symbolNode *next; //pointer to next
};     

//// struct for holding illegal opcode
struct illegalOpcode{
int lineNum; // field to hold the line number
struct illegalOpcode *next; // pointer to next node
};

// struct to hold undefined symbols --- 3rd error
struct undefinedSymbol{
char *undefinedSymbol; // name of the symbol
struct undefinedSymbol *next; // pointer to next
};

// struct to hold opcode table
struct opCodeTable{
  char name[MNEMONIC]; // mnemonic of opcode
  char format[FORMAT]; // format i,r,j 
  int opcode; // number of opcode
}; 
