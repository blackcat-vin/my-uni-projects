#ifndef _GA
#define _GA

#include <string>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <math.h>

using std::string;
using namespace std;

#define CROSSOVER_RATE            0.7
#define MUTATION_RATE             0.0333
#define POP_SIZE                  100			
#define CHROMO_LENGTH             2025
#define GENE_LENGTH               9
#define MAX_ALLOWABLE_GENERATIONS 49

//returns a float between 0 & 1
#define RANDOM_NUM1		((float)rand()/(RAND_MAX+1))

//returns a float between -1 & 1
#define RANDOM_NUM2		((float)rand()/(RAND_MAX * (-1) ))

//----------------------------------------------------------------------------------------
//
//	define a data structure which will define a chromosome
//
//----------------------------------------------------------------------------------------
struct chromo_typ
{
   //the binary bit string is held in a std::string
   string	bits;  
   float	  fitness;

	chromo_typ(): bits(""), fitness(0.0f){};
	chromo_typ(string bts, float ftns): bits(bts), fitness(ftns){};
};


/////////////////////////////////prototypes/////////////////////////////////////////////////////

void    PrintGeneSymbol(int val);
string  GetRandomBits(int length);
int     BinToDec(string bits);
double  AssignFitness(string bits, double desiredValue, double targetValue);
void    PrintChromo(string bits);
void    PrintGeneSymbol(float val);
int     ParseBits(string bits, float* buffer);
string  Roulette(double total_fitness, chromo_typ* Population);
void    Mutate(string &bits);
void    Crossover(string &offspring1, string &offspring2);

/////////////////////////////////////////////////////////////////////////////////////////////////

#endif
