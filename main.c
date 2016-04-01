/*
Parikh Shivam
ICSI 402
Assignment 4
UNIX ID: sp191221
*/


#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>  
#include "constants.h"
  #include "struct.h"
#include "globals.h"
//#include "struct.h"
#include "prototypes.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////
///							     ///
///		              MAIN METHOD STARTS	     ///
///							     ///
////////////////////////////////////////////////////////////////


int main(int args, char *argv[])
{

char *inFileName; //input file name 

FILE *inFile; //input file pointer
FILE *objFileP; //object file pointer
FILE *symFileP; //symbol file pointer
FILE *errFileP; //error file pointer

char *rootName; //root name which is 2nd argument
char *objFile; // object file name with extension
char *symFile; // symbol file name with extension
char *errFile; // error file name with extension

char *token; // holds token

char line[LINESIZE]; // copies line on to this
  
  
  
  if(args != COMMANDENTRY){ // if entered argument is not equal to required number then
  printf("ERROR: NUMBER of COMMAND ENTRY MISMATCH \n"); // print message
  exit(1); // exit the program
  }
   // malloc the size of input file name to size of second argumrnt
   
   
  inFileName = malloc( (sizeof(char)*strlen(argv[ARGFILE])) ); 
  
  strcpy(inFileName, argv[ARGFILE]); // copy file name on
  
  if ((inFile = fopen(inFileName, "r")) == NULL) { // if it fails opening input file
      fprintf(stderr, "Could not open file %s for reading.\n"); // print error 
      exit(1); // exit the program
    }
  
  
  rootName = malloc( (sizeof(char)*strlen(argv[ARGFILE])) ); //create space for rootname
  
        if( strrchr(argv[ARGFILE], '.')  == NULL ){   // If there is no extension
                strcpy(rootName, argv[ARGFILE]);     // 2nd argument is root name
        }
        else { // get rid off the last dot
        // replace last dot with \0
			argv[ARGFILE][strlen(argv[ARGFILE])-strlen(strrchr(argv[ARGFILE], '.'))] = '\0';
			// copy onto rootname
			strcpy(rootName, argv[ARGFILE]);
		} 
		
		
		objFile = malloc( (sizeof(char)*(strlen(rootName) +  EXTENSION)) ); // create space with extension
		symFile = malloc( (sizeof(char)*(strlen(rootName) +  EXTENSION)) ); // create space with extension
		errFile = malloc( (sizeof(char)*(strlen(rootName) +  EXTENSION)) ); // create space with extension
		strcpy(objFile, rootName); // copy rootname this has no extension
		strcpy(symFile, rootName); // copy rootname this has no extension 
		strcpy(errFile, rootName); // copy rootname this has no extension
		objFile = strcat(objFile, ".obj"); //attach extension at the end
		errFile = strcat(errFile, ".err"); //attach extension at the end
		symFile = strcat(symFile, ".sym"); //attach extension at the end
		
		
		if( (objFileP = fopen(objFile, "w")) == NULL ){ // create a file for writing- if fails
			fprintf(stderr, "error opening the object code file for writing"); //print error
			exit(1); // exit the program
			}
			
		if( (errFileP = fopen(errFile, "w")) == NULL ){ // create a file for writing- if fails 
			fprintf(stderr, "error opening the  error file for writing"); // print error
			exit(1); // exit the program
			}
			
		if( (symFileP = fopen(symFile, "w")) == NULL ){ // create a file for wrinting- if fails
			fprintf(stderr, "error opening the symbol table file for writing"); // print error
			exit(1); //exit the program
			}
    
   
        ///////////////////////////////////////////////////////////////////////////////////////////////////
        
        // now it will read through line by line and perform it's tasks.
        
  /*      
   while(fgets(line,sizeof(line), inFile) != NULL){     //Read till the end of the file
        lineCount++; // increment line number
        //printf("%s\n", line);
   }
   */
        //printf("num lines: %d\n", lineCount);
        
        
        setUpOpcode(); //call method to set up the opcode
        
        //setUp symbol table method will also call 
        //pass 2 method
        setUpSymbolTable(inFileName); // call method to setup symbol table
        
        printSymbol(symFileP); // writes onto file
        
        writeErrorOnFile(inFile, errFileP); // write error.err
        
        writeObj(objFileP); // write object file
        
        //printSymbolTable(); // prints on stdout
        
        
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    
    
    
    return 0; // return 0
}


///////////////////////////////////////////////////////////////
///						            ///
///			     MAIN METHOD ENDS	            ///
///				       			    ///
///////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





    
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
