// A program to format the Letter Recognition Dataset
// Vineet Kashyap, 02302675

#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <vector>
#include <iterator>
#include <stdlib.h>
#include <string.h>

using namespace std;

void processLine( string &line );

ofstream outputFile1;

int countForLetterA = 0; 
int countForLetterB = 0; 
int countForLetterC = 0; 
int countForLetterD = 0;
int countForLetterE = 0;

int main() {
	int i=0;
	
	ifstream inputFile;
	ofstream outputFile;
	
	inputFile.open("dataset.txt", ios::in);	
	outputFile1.open("ABCDE.csv", ios::out);
	
	string line;
	if ( inputFile.is_open() )
	{
		while ( getline(inputFile, line)) {
		
			//process line
			processLine(line);
			
			char *cstr = new char[line.size()+1];
			strcpy(cstr, line.c_str()); // creates a c string equivalent
			switch(cstr[0]) {
				
				case 'A': outputFile1 << "1" << "," << "0" << "," << "0" << "," << "0" << "," << "0" << endl;
							 countForLetterA++;
							 break;
				case 'B': outputFile1 << "0" << "," << "1" << "," << "0" << "," << "0" << "," << "0" << endl;
							 countForLetterB++;
							 break;
				case 'C': outputFile1 << "0" << "," << "0" << "," << "1" << "," << "0" << "," << "0" << endl;
							 countForLetterC++;
							 break;
				case 'D': outputFile1 << "0" << "," << "0" << "," << "0" << "," << "1" << "," << "0" << endl;
							 countForLetterD++;
							 break;
				case 'E': outputFile1 << "0" << "," << "0" << "," << "0" << "," << "0" << "," << "1" << endl;	
							 countForLetterE++;				
							 break;
				default : outputFile1 << "0" << "," << "0" << "," << "0" << "," << "0" << "," << "0" << endl;
							 break;
		
			}
		}
		inputFile.close();
	}	
	else 
	{
		cout << "Error Opening Input File: " << "dataset.csv" << endl;
		return false;
	}
	
   cout << "Finished writing file" << endl;
   cout << "======================" << endl;
	cout << "Count for Letter A = " << countForLetterA << endl;
	cout << "Count for Letter B = " << countForLetterB << endl;
	cout << "Count for Letter C = " << countForLetterC << endl;
	cout << "Count for Letter D = " << countForLetterD << endl;
	cout << "Count for Letter E = " << countForLetterE << endl;
	
   
	outputFile1.close();
}

/*******************************************************************
* Processes a single line from the data file
********************************************************************/
void processLine( string &line )
{
	int* pattern = new int[17];
	char *cstr = new char[line.size()+1];
	char* t;
	
	strcpy(cstr, line.c_str()); // creates a c string equivalent
	
	//tokenise
	int i = 1;
	t=strtok (cstr,",");
   
	while ( t!=NULL && i < 17)
	{	
		
		if(i < 17) {
			
			   t = strtok(NULL,","); //move token onwards
				pattern[i] = atoi(t);	
				outputFile1 << pattern[i] << "," ;
		}
		
		i++;
	}
}
