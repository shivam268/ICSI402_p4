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
#include "externs.h"
#include "prototypes.h"


////////////////////////////////////////////           SYMBOL TABLE MAKING AND PRINTING STARTS       /////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// this will go throught the file and parse out the symbols
// and process them to be added on to a linked list, before adding
// it checks if the symble is already defined or nor
// if it's defined then it will add an error
void setUpSymbolTable(char *fileName){

FILE *inFile = fopen(fileName, "r"); // open file for reading

char *opToken; // extra copy of characters

char *token; // stores token
char line[LINESIZE]; // holds the line
int tempLC = 0; // temp Location Counter
int startCountLC = 0; //flag to start counting LC
char word[LINESIZE]; // holds the word could be a register
int isLabel = 0; // flag for label
char lineCopy[LINESIZE]; //copy of the full line
    
int lcCheck = 0; // location counter flag

if(inFile == NULL){ // if infile is null
printf("ERROR: Opening a file for reading \n"); // print an error
exit(1); // exit the program
}

while(fgets(line,sizeof(line), inFile) != NULL){     //Read till the end of the file
        lineCount++; // increment the line counter
        strcpy(lineCopy, line); // make a back up copy of line for parsing opcode
        token = strtok(line, " ,\t\n"); // get the first token in the line
    
        //printf("this is line: %s\n", token);
    
		/* Check that the current line is NOT all whitespace, a comment or the .data/.text directive */
		if(token != NULL && token[0] != '#' && strcmp(token, ".data")!=0 && strcmp(token, ".text")!=0 ){
                        //printf("PRINT: %s\n", token);
			/* Check if the first token in the line is a label */
			if( token[strlen(token) - 1] == ':' ){ 
				isLabel = 1; // set flag to label
				token[strlen(token) - 1] = '\0'; // get rid off : at the end
				
                
                
                // now we have label and LC check if it is there already- error
                // if it's not there then add onto linked list
                if(checkMultiplyImp(token) == 0){ //if its multiply defined, still check for it's opcode correctness
                	token = strtok(NULL, " ,\t\n"); // get the next token which could obe three things
                    if(strcmp(token, ".resw")==0 || strcmp(token, ".word")==0 ){ // .word or .resw or opcode instruction
                    	//printf("OpToken (.word or .resw):-- %s\n", token); 
                    	
                    	opCodeProcess1(token); // process the token
                    }
                    else{ // now it must be opcode instruction
                	
		     		
		     	opToken = strtok(lineCopy, " ,\t\n"); // get the full line tokenize the label
		     		
		     	opToken = strtok(NULL, " ,\t\n"); // tokenize again to get the opcode
		     	//printf("opToken: %s\n", opToken); 
		     	
		     	opCodeProcess1(opToken); // process it for second pass
		     	
		     	validOpcode(opToken); //check the validity of opcode-- if invalid error
                	}
                    //@TODO ERROR GOES HERE --- MULTIPLY DEFINED
                }
                else{ // it's not multiply defined
                // add to symbol linked list
                    addSymbolList(token, tempLC); // it's not defined before, so add it on linked list
                    
                    
                    token = strtok(NULL, " ,\t\n"); // string tok to get the .word or .resw or opcode
                    if(strcmp(token, ".resw")==0 || strcmp(token, ".word")==0 ){// if it's either .resw or .word
                    
                        // we still need to process .word or .resw for assembly part
                               
                        	//printf("OpToken (.word or .resw) not multiply: %s\n", token); 
                        	
                        	opCodeProcess1(token); // process to opcode
                        	
                        	
			if(strcmp(token, ".resw") == 0 ){ // if it's .resw increment location counter
                            //printf("This is .resw");
			    token = strtok(NULL, " ,\n"); // strtok to get the value
                            tempLC += atoi(token); // convert that to int
                            lcCheck = 1; // set flag for LC to 1
			}
			else if(strcmp(token, ".word") == 0 ){ // if it's .word increment location counter
                            //printf("This is .resw"); 
			    token = strtok(NULL, " \t\n");  //string tok to get the next value
			    //printf("this is strtok: %s\n", token);
			    token = strrchr(token, ':')+1; // string tok to get the word num
			    //printf("this is strtok2: %s\n", token); 
       			    tempLC += atoi(token); // convert that to int and increment LC by that much
       			    
       			   // printf("tempLc: %d\n", atoi(token));
                            lcCheck = 1; // set LC flag to 1
                           
                            /* 
                           token = strtok(NULL, " :\t");
                           token = strtok(NULL, " \t\n");
                           tempLC += atoi(token);
                           printf("This is token -------- %d\n", atoi(token));
                           
                           
                           lcCheck = 1;
                            */
			}
			
			
			
			
                    } // if for .resw and .word check ends
		     else{//now it's just an opcode
		     		opToken = strtok(lineCopy, " ,\t\n"); // this is label before opcode
		     		 
		     		opToken = strtok(NULL, " ,\t\n"); // strtok again to get opcode
		     		
		     		//printf("opToken: %s\n", opToken);
		    		
		    		validOpcode(opToken); // check for validity of opcode
		    				
		     		opCodeProcess1(opToken); // process it for opcode
             }
                }// inner else ends
                if(lcCheck == 0){tempLC++;} // if the lc flag is 0 increment the lc value
            } // label check if ends
            else{ // now it's the line that contains opcode instruction without label in the front
                tempLC++; // if it's not label still increment LC value
                  //if(isLabel == 0){
                    //printf("This is OPCODE NO LABEL IN FRONT: %s\n", lineCopy);
                    
                    
                    opToken = strtok(lineCopy, " ,\t\n"); //string tok to get the opcode
                    
                    //printf("opToken: %s\n", opToken);
                    
                    validOpcode(opToken); // check for valid opcode
                    
                    opCodeProcess1(opToken); // process the opcode for assembly operation
                    
                  //}
                }
                
        } // if for white space, comments, .data, .text ends here
        
        else{
        }
        
   } // while ends
   
   rewind(inFile); // rewind back to the beginning

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//this taked label as a parameter
// returns 0 if multiply defined
// returns 1 if not defined

int checkMultiplyImp(char *label){
    temp = head; // make temp point to head
    while(temp != NULL){ // go through every node and compare
        if(strcmp(temp->symbol, label) == 0){ // if symbol from node and label matches, that means it's already defined
            //printf("Comparing Symbol: %s\n", temp->symbol);
            //printf("Comparing Label: %s\n", label);
            
            temp->mulDefined = 1; // set flag to 1 so that means that symbol exist more than once
            
            lcAssembly = temp->lcValue; // copies the lc value of the current node for J format Assembly
            
            return 0; // returns 0 if there is already defined
        }
        temp = temp->next; // keep moving temp pointer
    }
    
    return 1; // return 1 if there was no match
    
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int checkMulDef(char *label){
struct symbolNode *t = NULL; 
 
    t = head; // make temp point to head
    while(t != NULL){ // go through every node and compare
        if(strcmp(t->symbol, label) == 0){ // if symbol from node and label matches, that means it's already defined
            //printf("Comparing Symbol: %s\n", temp->symbol);
            //printf("Comparing Label: %s\n", label);
            
            // set flag to 1 so that means that symbol exist more than once
            
            lcAssembly = t->lcValue; // copies the lc value of the current node for J format Assembly
            
            return 0; // returns 0 if there is already defined
        }
        else{}
        t = t->next; // keep moving temp pointer
    }
    
    return 1; // return 1 if there was no match
    
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// addSymolList takes label and LC value as a parameter
void addSymbolList(char *label, int lc){
    
    //printf("LABEL: %s\n", label);
    //printf("LC: %d\n", lc);

  
  // make a node, if making first node, make sure to make head pointer pointing to that node
	if(head == NULL){ // if making first time
	  head = malloc(sizeof(struct symbolNode)); // malloc a new node
	  strcpy(head->symbol, label); // copy label onto struct node
	  head->lcValue = lc; // copy LC value on struct node
	  
	}
	
	else{ // there is a node already
		temp = head; // make temp point to head
		while(temp->next != NULL){ // go through the linked list and read to the last node in the linked list
		  temp = temp->next; // keep moving temp pointer
		}
		temp->next = malloc(sizeof(struct symbolNode)); // once reach to the end, malloca a struct node
		temp = temp->next; // make temp to point to new node
		
		strcpy(temp->symbol, label); // copy label on to that
		temp->lcValue = lc; // copy LC value on it
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// PRINT SYMBOL TABLE
// write on to file
void printSymbol(FILE* filename){ //input: the desired symbol table file
	temp = head; // make temp point to hea
	while(temp != NULL){ // go through the linked list
	fprintf(filename, "%s\t\t%d\n", temp->symbol, temp->lcValue); // print the symbol and LC value on .sym file
		temp = temp->next; // increment temp pointer
	}
	fclose(filename); // close the file
}


////////////////////////////////////////////           SYMBOL TABLE MAKING AND PRINTING ENDS       ///////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////













////////////////////////////////////////////           ERROR CHECKING AND PRINTING STARTS       //////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// takes label name as a parameter


void validOpcode(char *validityCheck){


int i; // loop counter
int flag = 0;
  // lop through the array of struct if any opcode matches
  // return 0 for valid opcode
  // retuen 1 for invalid opcode
for(i = 0; i < 36; i++){ // go through the array
  if(strcmp(opArray[i].name, validityCheck) == 0){ // compare them
  	flag = 1; // if match set flag to 1
  	}
  else{
  }
}

if(flag == 0){ // if flag it 0, means no match add invalid opcode
	
	addInvalidOpcode(); // call function to add an error
}

}

////////////////////////////////////////////////////////////////////////
//This method will return the opcode for given mnemonic
int returnOpcode(char *opcodeName){
int i; // loop counter

int tempNum = 0; // this will be returned at the end

for(i = 0; i < 36; i++){ // go through the array
  if(strcmp(opArray[i].name, opcodeName) == 0){ // compare them
  	tempNum = opArray[i].opcode; // if match return the opcode
  	}
  else{
  	
  }
}

return tempNum; // returns the  opcode

}

////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////
// add's invalid opcode line number
void addInvalidOpcode(){

if(illegalHead == NULL){ // if making first time
	  illegalHead = malloc(sizeof(struct illegalOpcode)); // malloc a new node
	  illegalHead->lineNum = lineCount; // copy line number on to node field
	  
	}
	
	else{ // there is a node already
		illegalTemp = illegalHead; // make temp point to head
		while(illegalTemp->next != NULL){ // go through the linked list and read to the last node in the linked list
		  illegalTemp = illegalTemp->next; // keep moving temp pointer
		}
		illegalTemp->next = malloc(sizeof(struct illegalOpcode)); // once reach to the end, malloca a struct node
		illegalTemp = illegalTemp->next; // make temp to point to new node
	
		illegalTemp->lineNum = lineCount; // copy LC value on it
	}



}
//////////////////////////////////////////////////////////////////////////////////////
// this function will take a char as a parameter and it will be added on to a linked list
// this is 3rd error check
void addUndefined(char *undefined){

	if(undefinedHead == NULL){ // if making first time
	  undefinedHead = malloc(sizeof(struct undefinedSymbol)); // malloc a new node
	  undefinedHead->undefinedSymbol = undefined; // copy symbol onto node field
	  
	}
	
	else{ // there is a node already
	        undefinedTemp = undefinedHead; // make temp point to head
		while(undefinedTemp->next != NULL){ // go through the linked list and read to the last node in the linked list
		  undefinedTemp = undefinedTemp->next; // keep moving temp pointer
		}
		undefinedTemp->next = malloc(sizeof(struct undefinedSymbol)); // once reach to the end, malloca a struct node
		undefinedTemp = undefinedTemp->next; // make temp to point to new node
	
		strcpy(undefinedTemp->undefinedSymbol, undefined); // copy symbol onto node field
	}
}


//////////////////////////////////////////////////////////////////////////////////////
///// WRITE ON TO THE ERROR FILE
// takes 2 parameters, 1. asembly file to print lines with line number and 2. error file to write on
void writeErrorOnFile(FILE *asmFile, FILE *errorLog){ 

char printLine[LINESIZE]; // holds entire line
int lineNumForError = 0; // temp line number incrementer

rewind(asmFile); // rewind the file bacck to top

while(fgets(printLine,sizeof(printLine), asmFile) != NULL){     //Read till the end of the file
        lineNumForError++; // increment line number
        fprintf(errorLog, "%d\t\t%s\n", lineNumForError, printLine); // write line number and full line

}

fprintf(errorLog, "Errors detected: \n"); // now actual error printing starts

//Illegal Opcode write

illegalTemp = illegalHead; // set temp pointer for illegal opcode to head pointer of it
while(illegalTemp != NULL){ // go through the list
  fprintf(errorLog,"line "); // print string
  fprintf(errorLog,"%d", illegalTemp->lineNum); // print the line number
  fprintf(errorLog,": Illegal opcode. \n"); // print string saying error detail

  illegalTemp = illegalTemp->next; // increment temp pointer
}

// write Multiply defined symbols
fprintf(errorLog, "\n"); //  print new line
fprintf(errorLog, "Multiply Defined Symbol(s): \n"); // print string
temp = head; // make temp pointer for symbol tablt to head of it
while(temp != NULL){ // go through the list 
  if(temp->mulDefined == 1){ // if the field for nultiply defined is 1
    fprintf(errorLog, "%s\n", temp->symbol); // process to write that symbol on file
  }
  temp = temp->next; // increment temp pointer
}

// write undefined symbols

fprintf(errorLog, "\n"); // new line
fprintf(errorLog, "Undefined Symbol(s): \n");  // print string for next errors

// this will write undefined symbols
undefinedTemp = undefinedHead; // make temp to point to head pointer

while(undefinedTemp != NULL){ // go through the list
	fprintf(errorLog, "%s\n", undefinedTemp->undefinedSymbol); // write the symbols on it
	undefinedTemp = undefinedTemp->next; // move temp pointer
}


}

////////////////////////////////////////////           ERROR CHECKING AND PRINTING ENDS       ////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// write on to object file

void writeObj(FILE *obj){
int i = 0; // loop counter

for(i = 0; i < location; i++){ // for number of location times

fprintf(obj, "%X\n", assembly[i]); // write the in hex
}
}







////////////////////////////////////////////           ASSEMBLY PART - 2ND PASS -- STARTS    /////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
This method will take atoken as a parameter and it will process to get the final assembly instruction 
it will check whether it's .word or .resw or an opcode
if it's an opcode it will read the oprators based on the opcode
*/
void opCodeProcess1(char *tokenOp){

char copytokenOp[5]; //copy of opcode

char *format; //copy format
char afterTok[8]; // after toking

char *tokenize; // tokensize

char tempFormat[FORMAT]; // holds the format r, i, j
int tempOpcode; //temp opcode holds

int value; // holds value
int wordNum; // word number
int rs1; // rs1
int rs2; // rs2
int rt; // rt
int answer; //answer holds
int sa; // sa number
int isValidSymbol; // holds 0 or 1
int tempNum; // temp number

int i = 0; // loop counter

char tempErrorCheck[LINESIZE]; //temp error line

strcpy(copytokenOp, tokenOp); // copy onto backup
//printf("%s\n",copytokenOp);
//printf("assembly: %s\n", tokenOp);

//printf("This is location Counter at the start: %d\n", location);

//printf("This is token: %s\n", tokenOp);
if(strcmp(tokenOp, ".word") == 0){
/*
tokenOp = strtok(NULL, " :");
value = atoi(tokenOp);
printf("This is value: %d\n", value);

tokenOp = strtok(NULL, " \t\n");
wordNum = atoi(tokenOp);
printf("This is value: %d\n", wordNum);
*/
}
else if(strcmp(tokenOp, ".resw") == 0){
}
else{
   tempOpcode = returnOpcode(tokenOp);
   
   strcpy(tempFormat, opArray[tempOpcode].format); // find the format out
   
   //printf("Format is: %s\n", tempFormat);

   //printf("assembly ELSE : %s\n", tokenOp);
   
   //printf("The opcode is: %d\n", tempOpcode);
   
   
   if(strcmp(tempFormat, "r")==0){
     //printf("------This is r Format \n");
     if(tempOpcode == 0){
     	answer = 0;
     	assembly[location] = answer;
     	//printf("The token name: %s\n", copytokenOp);
     	//printf("This is Answer: %.4x\n", answer);
     	location++;
     	
     }
     else if((tempOpcode > 0 && tempOpcode < 6) || (tempOpcode > 6 && tempOpcode < 10)){
     	    answer = tempOpcode << SHIFT26; // shift to left
     	    tokenOp = strtok(NULL, " ,$"); // string tok
     	    rt = atoi(tokenOp); // convert to int
     	    tokenOp = strtok(NULL, " ,$"); // string tok
     	    rs1 = atoi(tokenOp); // convert to int 
     	    tokenOp = strtok(NULL, " ,$\n"); // string tok
     	    rs2 = atoi(tokenOp); // convert to int
     	    
     	    rt = rt << SHIFT11; // shift left
     	    answer = answer | rt; // or the ans with rt
     	    
     	    rs1 = rs1 << SHIFT21; // shift left
     	    answer = answer | rs1; // or the ans with rs1
     	    
     	    rs2 = rs2 << SHIFT16; // shift left
     	    answer = answer | rs2; // or the ans with rs2
     	    
     	    assembly[location] = answer; // add the ans on the array
     	    location++; // increment location counter
     	    //printf("The token name: %s\n", copytokenOp);
     	    //printf("This is Answer: %.4x\n", answer); 
     	    
     	}
     	else if(tempOpcode > 6 && tempOpcode < 14){
     		answer = tempOpcode << SHIFT26; // shift left
     		
     		tokenOp = strtok(NULL, " ,$"); // string tok
     		rt = atoi(tokenOp); // convert to int
     		
     		tokenOp = strtok(NULL, " ,$"); // string tok
     		rs1 = atoi(tokenOp); // convert to int
     		
     		tokenOp = strtok(NULL, " ,\n"); //string tok
     		sa = atoi(tokenOp); // convert to int
     		
     		rt = rt << SHIFT11; //shift to left 11
     		answer = answer | rt; // or with rt
     		
     		rs1 = rs1 << SHIFT21; // shift left
     		answer = answer | rs1; // or with rs1
     		
     		sa = sa << SHIFT6; // shift left
     		answer = answer | sa; // or with sa
     		
     		assembly[location] = answer; //add answer on the array
     		location++; // increment location 
     		
     		//printf("The token name: %s\n", copytokenOp);
     	    	//printf("This is Answer: %.4x\n", answer); 
     	}
     	else if(tempOpcode == 6 || tempOpcode == 10){
     		answer = tempOpcode << SHIFT26; // shift left
     		
     		tokenOp = strtok(NULL, " ,$"); // string tok
     		rt = atoi(tokenOp); // convert to int
     		
     		tokenOp = strtok(NULL, " ,$\n"); // string tok
     		rs1 = atoi(tokenOp); // convert to int 
     		
     		rt = rt << SHIFT11; // shift left
     		answer = answer | rt; // or with rt
     		
     		rs1 = rs1 << SHIFT21; // shift left
     		answer = answer | rs1; // or with rs1
     		
     		assembly[location] = answer; // add answer to array
     		location++; // location counter
     		
     		//printf("The token name: %s\n", copytokenOp);
     	    	//printf("This is Answer: %.4x\n", answer);
     	}
     	else if(tempOpcode==14||tempOpcode==15||tempOpcode==16||tempOpcode==17){
     		answer = tempOpcode << SHIFT26; // shift left
     		tokenOp = strtok(NULL, " ,$\n"); // string tok
     		rt = atoi(tokenOp); // convert to int 
     		
     		rt = rt << SHIFT11; // shift left
     		answer = answer | rt; // or with rt
     		
     		assembly[location] = answer; // add answer to array
     		location++; // increment the array
     		
     		//printf("The token name: %s\n", copytokenOp);
     	    	//printf("This is Answer: %.4x\n", answer);
     	    	
     	}
   }
   else if(strcmp(tempFormat, "i") == 0){
     if(tempOpcode == 18){ // if the opcode is 18
     	answer = tempOpcode << SHIFT26; // shift to left
     	
     	tokenOp = strtok(NULL, " ,$"); // string tok
     	rt = atoi(tokenOp); // convert to int
     	
     	tokenOp = strtok(NULL, " ,\n"); // string tok
     	tempNum = (short)atoi(tokenOp); // convert to num
     	
     	tempNum = tempNum & MASK; // and mask
     	
     	answer = answer | tempNum; // or it
     	
     	rt = rt << SHIFT16; // shift left
     	answer = answer | rt; // or with rt
     	
     	assembly[location] = answer; // add ans on array
     	
     	location++; // increment location
     	
     	//printf("The token name: %s\n", copytokenOp);
     	//printf("This is Answer: %.4x\n", answer);
     }
     else if(tempOpcode > 18 && tempOpcode < 24){
     	answer = tempOpcode << SHIFT26; // shift left
     	
     	tokenOp = strtok(NULL, " ,$"); // string tok
     	rt = atoi(tokenOp); // convert to int
     	
     	tokenOp = strtok(NULL, " ,$"); // string tok
     	rs1 = atoi(tokenOp); // convert to int
     	
     	tokenOp = strtok(NULL, " ,\n"); // string tok
     	tempNum = (short)atoi(tokenOp); // convert
     	
     	tempNum = tempNum & MASK; // and mask
     	
     	answer = answer | tempNum; // or with tempNum
     	
     	rt = rt << SHIFT16; // shift left
     	answer = answer | rt; // or with rt
     	
     	rs1 =rs1 << SHIFT21; // shift left
     	answer = answer | rs1; // or with rs1
     	
     	assembly[location] = answer; // add ans on array
     	location++; // increment location counter
     	
     	//printf("The token name: %s\n", copytokenOp);
     	//printf("This is Answer: %.4x\n", answer);
     
     }
     else if(tempOpcode == 24 || tempOpcode == 25){
     	answer = tempOpcode << SHIFT26; // shift left
     	
     	tokenOp = strtok(NULL, " ,$"); // string tok
     	rt = atoi(tokenOp); // convert to int
     	
     	tokenOp = strtok(NULL, " ,("); // string tok
     	tempNum = (short)atoi(tokenOp); // convert num
     	
     	tokenOp = strtok(NULL, " ,$)\n"); // string tok
     	rs1 = atoi(tokenOp); // convert to num
     	
     	tempNum = tempNum & MASK; // and mask
     	
     	answer = answer | tempNum; // or with tempNum
     	
     	rt = rt << SHIFT16; // shift left
     	answer = answer | rt; // or with rt
     	
     	rs1 = rs1 << SHIFT21; // shift left
     	answer = answer | rs1; // or with rs1
     	
     	assembly[location] = answer; // add ans on the array
     	
     	location++; // increment location counter
     	
     	//printf("The token name: %s\n", copytokenOp);
     	//printf("This is Answer: %.4x\n", answer);
     }
   }
   else if(strcmp(tempFormat, "j") == 0){
   
   assembly[location] = 0;
   location++;
   
   
   //isValidSymbol = checkMulDef("test1");
   
   //printf("Test from J Function: %d\n", isValidSymbol);
   
   /*
   	if(tempOpcode > 25 && tempOpcode < 28){
   		
   		answer = tempOpcode << SHIFT26;
   		
   		tokenOp = strtok(NULL, " ,$");
   		rt = atoi(tokenOp);
   		
   		printf("print RT: %d\n", rt);
   		
   		rt = rt << SHIFT16;
   		
   		answer = answer | rt;
   		
   		tokenOp = strtok(NULL, " ,\n");
   		
   		printf("test: --- %s\n", tokenOp);
   		
   		isValidSymbol = checkMulDef(tokenOp);
   		
   		if(isValidSymbol == 0){
   		
   			//answer = answer | 26;
   			printf("lcassembly: %d\n", lcAssembly);
   			assembly[location] = answer;
   			printf("This is Answer2: %.4x\n", answer);
   			location++;
   		}
   		else{
   		     addUndefined(tokenOp);
   		}
   		
   		printf("The token name: %s\n", copytokenOp);
     		printf("This is Answer: %.4x\n", answer);
   		
   	}
   	else if(tempOpcode == 28 || tempOpcode == 29){
   		answer = tempOpcode << 26;
   		
   		tokenOp = strtok(NULL, " ,\n");
   		
   		if(checkMulDef(tokenOp)==0){
   			answer = answer | lcAssembly;
   			assembly[location] = answer;
   			location++;
   			//printf("The token name: %s\n", copytokenOp);
     			printf("This is Answer: %.4x\n", answer);
   		}
   		else{
   			addUndefined(tokenOp);
   		}
   		printf("The token name: %s\n", copytokenOp);
     		printf("This is Answer: %.4x\n", answer);
   	}
   	else if(tempOpcode > 29 && tempOpcode < 36){
   		answer = tempOpcode << 26;
   		
   		tokenOp = strtok(NULL, " ,$");
   		rt = atoi(tokenOp);
   		
   		tokenOp = strtok(NULL, " ,$");
   		rs1 = atoi(tokenOp);
   		
   		tokenOp = strtok(NULL, " ,\n");
   		
   		rt = rt << 16;
   		answer = answer | rt;
   		
   		rs1 = rs1 << 21;
   		answer = answer | rs1;
   		
   		if(checkMulDef(tokenOp)==0){
   			answer = answer | lcAssembly;
   			assembly[location] = answer;
   			location++;	
   		}
   		else{
   			addUndefined(tokenOp);
   		}
   		printf("The token name: %s\n", copytokenOp);
     		printf("This is Answer: %.4x\n", answer);
   	}
   	*/
   }

//printf("This is location Counter at the end: %d\n", location);
}
}








////////////////////////////////////////////           ASSEMBLY PART - 2ND PASS -- ENDS    ///////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////














///// SETUP the Opcode table
// hard coded in, the struct holds opcode, nmnemonic, format

void setUpOpcode(){
    opArray[0].opcode = 0;
    strcpy(opArray[0].name, "hlt");
    strcpy(opArray[0].format, "r");
    
    opArray[1].opcode = 1;
    strcpy(opArray[1].name, "add");
    strcpy(opArray[1].format, "r");
    
    opArray[2].opcode = 2;
    strcpy(opArray[2].name, "sub");
    strcpy(opArray[2].format, "r");
    
    opArray[3].opcode = 3;
    strcpy(opArray[3].name, "mul");
    strcpy(opArray[3].format, "r");
    
    opArray[4].opcode = 4;
    strcpy(opArray[4].name, "div");
    strcpy(opArray[4].format, "r");
    
    
    opArray[5].opcode = 5;
    strcpy(opArray[5].name, "mod");
    strcpy(opArray[5].format, "r");
    
    opArray[6].opcode = 6;
    strcpy(opArray[6].name, "move");
    strcpy(opArray[6].format, "r");
    
    opArray[7].opcode = 7;
    strcpy(opArray[7].name, "and");
    strcpy(opArray[7].format, "r");
    
    opArray[8].opcode = 8;
    strcpy(opArray[8].name, "or");
    strcpy(opArray[8].format, "r");
    
    opArray[9].opcode = 9;
    strcpy(opArray[9].name, "xor");
    strcpy(opArray[9].format, "r");
    
    opArray[10].opcode = 10;
    strcpy(opArray[10].name, "com");
    strcpy(opArray[10].format, "r");
    
    opArray[11].opcode = 11;
    strcpy(opArray[11].name, "sll");
    strcpy(opArray[11].format, "r");
    
    opArray[12].opcode = 12;
    strcpy(opArray[12].name, "srl");
    strcpy(opArray[12].format, "r");
    
    opArray[13].opcode = 13;
    strcpy(opArray[13].name, "sra");
    strcpy(opArray[13].format, "r");
    
    opArray[14].opcode = 14;
    strcpy(opArray[14].name, "jr");
    strcpy(opArray[14].format, "r");
    
    opArray[15].opcode = 15;
    strcpy(opArray[15].name, "rdr");
    strcpy(opArray[15].format, "r");
    
    opArray[16].opcode = 16;
    strcpy(opArray[16].name, "prr");
    strcpy(opArray[16].format, "r");
    
    opArray[17].opcode = 17;
    strcpy(opArray[17].name, "prh");
    strcpy(opArray[17].format, "r");
    
    opArray[18].opcode = 18;
    strcpy(opArray[18].name, "li");
    strcpy(opArray[18].format, "i");
    
    opArray[19].opcode = 19;
    strcpy(opArray[19].name, "addi");
    strcpy(opArray[19].format, "i");
    
    opArray[20].opcode = 20;
    strcpy(opArray[20].name, "subi");
    strcpy(opArray[20].format, "i");
    
    opArray[21].opcode = 21;
    strcpy(opArray[21].name, "muli");
    strcpy(opArray[21].format, "i");
    
    opArray[22].opcode = 22;
    strcpy(opArray[22].name, "divi");
    strcpy(opArray[22].format, "i");
    
    opArray[23].opcode = 23;
    strcpy(opArray[23].name, "modi");
    strcpy(opArray[23].format, "i");
    
    opArray[24].opcode = 24;
    strcpy(opArray[24].name, "lwb");
    strcpy(opArray[24].format, "i");
    
    opArray[25].opcode = 25;
    strcpy(opArray[25].name, "swb");
    strcpy(opArray[25].format, "i");
    
    opArray[26].opcode = 26;
    strcpy(opArray[26].name, "lwa");
    strcpy(opArray[26].format, "j");
    
    opArray[27].opcode = 27;
    strcpy(opArray[27].name, "swa");
    strcpy(opArray[27].format, "j");
    
    opArray[28].opcode = 28;
    strcpy(opArray[28].name, "j");
    strcpy(opArray[28].format, "j");
    
    opArray[29].opcode = 29;
    strcpy(opArray[29].name, "jal");
    strcpy(opArray[29].format, "j");
    
    opArray[30].opcode = 30;
    strcpy(opArray[30].name, "jeq");
    strcpy(opArray[30].format, "j");
    
    opArray[31].opcode = 31;
    strcpy(opArray[31].name, "jne");
    strcpy(opArray[31].format, "j");
    
    opArray[32].opcode = 32;
    strcpy(opArray[32].name, "jlt");
    strcpy(opArray[32].format, "j");
    
    opArray[33].opcode = 33;
    strcpy(opArray[33].name, "jle");
    strcpy(opArray[33].format, "j");
    
    opArray[34].opcode = 34;
    strcpy(opArray[34].name, "jgt");
    strcpy(opArray[34].format, "j");
    
    opArray[35].opcode = 35;
    strcpy(opArray[35].name, "jge");
    strcpy(opArray[35].format, "j");   
}




        

        

        
