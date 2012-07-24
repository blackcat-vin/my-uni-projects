#include <string>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <math.h>

//include definition file
#include "ga.h"


using namespace std;
using std::string;

//---------------------------------GetRandomBits-----------------------------------------
//
//	This function returns a string of random 1s and 0s of the desired length.
//
//-----------------------------------------------------------------------------------------
string GetRandomBits(int length)
{
	string bits;

	for (int i=0; i<length; i++)
	{
		if (RANDOM_NUM1 > 0.5f)

			bits += "1";

		else

			bits += "0";
	}

	return bits;
}

//---------------------------------BinToDec-----------------------------------------
//
//	converts a binary string into a decimal
//
//-----------------------------------------------------------------------------------
int BinToDec(string bits)
{
	
	int val = 0;
	int value_to_add = 1 ;
	int indexBit = 0;
	
	for (int i = bits.length()-1; i > 0; i--)
	{
		if (bits.at(i-1) == '1') {
          val += value_to_add;//value_to_add;
      }     
		value_to_add *= 2 ;
		
	}//next bit

	//return val;
	if (bits.at(indexBit) == '1') { return -1 * val; }  // look at the 9th bit which is the index bit and make the value negative
	else return val;

}


//---------------------------------ParseBits------------------------------------------
//
// Given a chromosome this function will step through the genes one at a time and insert 
// the float values of each gene into a buffer. Returns the number of weights in the buffer.
//------------------------------------------------------------------------------------
int ParseBits(string bits, float* buffer)
{
	//set range
	double rH = -1; 
	double rO = 1; 
	
	//counter for buffer position
	int cBuff = 0;
	
	// step through bits a gene at a time until end and store float values
	// of valid numbers. 
	
	//storage for float value of currently tested gene
	float this_gene = 0.0f;
	
	for (int i=0; i<CHROMO_LENGTH; i+=GENE_LENGTH)
	{
		//convert the current gene to float ; set weights to random values between -1 and 1 0.0002 is the scaling factor
		this_gene =   0.0002 * BinToDec(bits.substr(i, GENE_LENGTH)) ; // ( ( (double)(rand()%100)+1)/100  * 2 * rH ) - rH; //
		buffer[cBuff++] = this_gene;
		
	} // next gene
	

	return cBuff;
}
	
//---------------------------------AssignFitness--------------------------------------
//
//	given a string of bits and a target value this function will calculate its  
// representation and return a fitness score accordingly
//------------------------------------------------------------------------------------
double AssignFitness(string bits, double desiredValue, double targetValue)
{
	//cout << "Assign Fitness" << endl;

	//holds float values of gene sequence
	float buffer[(int)(CHROMO_LENGTH / GENE_LENGTH)];

	int num_weights = ParseBits(bits, buffer);
	int i;
	
	// Now we calculate the fitness. First check to see if a solution has been found
	// and assign an arbitarily high fitness score if this is so.

	if ( desiredValue == ceil(targetValue)) {
		return 999.0f;
      
	}	
	else {
		return (1/fabs(desiredValue-ceil(targetValue)));  
	}
	
}

//---------------------------------PrintChromo---------------------------------------
//
// decodes and prints a chromo to screen
//-----------------------------------------------------------------------------------
void PrintChromo(string bits)
{	
	//cout << "PrintChromo" << endl;
	//holds floating point values of gene sequence
	float buffer[(int)(CHROMO_LENGTH / GENE_LENGTH)];
	
	//open file for writing
	fstream outputFile;
	outputFile.open("weights.csv", ios::out);
	outputFile.precision(5);
	
	//parse the bit string
	int num_weights = ParseBits(bits, buffer);
	
	for (int i=0; i<num_weights; i++)
   {
		outputFile << buffer[i] << "," ;
		//PrintGeneSymbol(buffer[i]); uncomment to print weights
   }
	outputFile << endl;
	
	//close file
	outputFile.close();
	
	return;
}
	
//--------------------------------------PrintGeneSymbol-----------------------------
//	
//	given a float this function outputs its symbol to the screen 
//----------------------------------------------------------------------------------
void PrintGeneSymbol(float val)
{
	
		cout << val << " "; // prints the weight value
}

//------------------------------------Mutate---------------------------------------
//
//	Mutates a chromosome's bits dependent on the MUTATION_RATE
//-------------------------------------------------------------------------------------
void Mutate(string &bits)
{
	//cout << "Mutate" << endl;
	for (int i=0; i<bits.length(); i++)
	{
		if (RANDOM_NUM1 < MUTATION_RATE)
		{
			if (bits.at(i) == '1')

				bits.at(i) = '0';

			else

				bits.at(i) = '1';
		}
	}

	return;
}

//---------------------------------- Crossover ---------------------------------------
//
//  Dependent on the CROSSOVER_RATE this function selects a random point along the 
//  length of the chromosomes and swaps all the  bits after that point.
//------------------------------------------------------------------------------------
void Crossover(string &offspring1, string &offspring2)
{
  
  //dependent on the crossover rate
  if (RANDOM_NUM1 < CROSSOVER_RATE)
  {
	 //create a random crossover point not at the boundary
	 int crossover = (int) (RANDOM_NUM1 * (CHROMO_LENGTH-1));
	 
    std::string t1 = offspring1.substr(0, crossover) + offspring2.substr(crossover, CHROMO_LENGTH-1);
    std::string t2 = offspring2.substr(0, crossover) + offspring1.substr(crossover, CHROMO_LENGTH-1);
    
    offspring1 = t1; 
    offspring2 = t2;
	 
      
  }
}


//--------------------------------Roulette-------------------------------------------
//
//	selects a chromosome from the population via roulette wheel selection
//------------------------------------------------------------------------------------
string Roulette(double total_fitness, chromo_typ* Population)
{
	//cout << "Roulette" << endl;
	//generate a random number between 0 & total fitness count
	double Slice = (RANDOM_NUM1 * total_fitness);
	
	//go through the chromosones adding up the fitness so far
	double FitnessSoFar = 0.0f;
	
	for (int i=0; i<POP_SIZE; i++)
	{
		FitnessSoFar += Population[i].fitness;
		
		//if the fitness so far > random number return the chromo at this point
		if (FitnessSoFar >= Slice)

			return Population[i].bits;
		
		else return Population[i].bits;
	}
}	
