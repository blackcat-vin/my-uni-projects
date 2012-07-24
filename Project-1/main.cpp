/*******************************************************************
* This program uses a Hybrid GA-NN approach to solve the problem of
* Letter Recognition from the UCI Machine Learning Repository dataset
* ------------------------------------------------------------------
* Vineet Kashyap
* 02302675
*********************************************************************/

//standard libraries
#include <iostream>
#include <ctime>
#include <string>
#include <math.h>

//custom includes
#include "neuralNetwork.h"
#include "neuralNetworkTrainer.h"
#include "ga.h"

using namespace std;

int main()
{		
	float mseForTestingFitness;
	float errorThreshold = 0.006;   // can be changed 
	float result;
	
	//seed random number generator
	srand( (unsigned int) time(0) );
	
	//create data set reader and load data file
	dataReader d;
	d.loadDataFile("ABCDE.csv",16,5);
	d.setCreationApproach( STATIC, 10 );	

	//create neural network
	neuralNetwork nn(16,10,5);

	//create neural network trainer
	neuralNetworkTrainer nT( &nn );
	nT.setTrainingParameters(0.009, 0.8, false);
	nT.setStoppingConditions(50, 0.01);
	nT.enableLogging("log.csv", 5);
	
	//storage for our population of chromosomes.
	chromo_typ Population[POP_SIZE];
	
	//first create a random population, all with zero fitness.
	for (int i=0; i<POP_SIZE; i++)
	{
		Population[i].bits = GetRandomBits(CHROMO_LENGTH);
		Population[i].fitness = 0.0f;
	}

	cout	<< endl << "Genetic Algorithm Starting: " << endl
			<< "==========================================================================" << endl
			<< "POP SIZE: " << POP_SIZE << ", MAX ALLOWABLE GENERATIONS: " << MAX_ALLOWABLE_GENERATIONS << ", CHROMO LENGTH: " << CHROMO_LENGTH << endl
			<< "GENE LENGTH: " << GENE_LENGTH << " CROSSOVER RATE: " << CROSSOVER_RATE << " MUTATION RATE: " << MUTATION_RATE << endl
			<< "==========================================================================" << endl << endl;
	
	int GenerationsRequiredToFindASolution = 0;

	//we will set this flag if a solution has been found
	bool bFound = false;
   
	//enter the main GA loop
   while(!bFound)
	{
		//this is used during roulette wheel sampling
		double TotalFitness = 0.0f;
      //train neural network on data sets
	   	 
	   
		// test and update the fitness of every chromosome in the population
		
		for (int i=0; i<POP_SIZE; i++)
		{
			PrintChromo(Population[i].bits); // initialize weights for backpropagation
			//print success
			cout << endl << "Neuron weights saved to weights.csv"  << endl;
			nn.loadWeights("weights.csv");
			cout << "======================================================" << endl;
			cout << "Generations: " << GenerationsRequiredToFindASolution << endl;
			cout << "======================================================" << endl;
			
			for (int j=0; j < d.getNumTrainingSets(); j++ )
	      {
		      mseForTestingFitness = nT.trainNetwork( d.getTrainingDataSet() );
	      }	
			
			if(mseForTestingFitness < 0.007) {
				cout << "\nSolution found after " << GenerationsRequiredToFindASolution << " generations!" << endl << endl;
				cout << "Testing using the generalization data set" << endl;
				//get generalization set accuracy and MSE
				double generalizationSetAccuracy = nn.getSetAccuracy( d.tSet.generalizationSet );
				double generalizationSetSSE = nn.getSetSSE( d.tSet.generalizationSet );
	
				cout << "Generalization Set Accuracy: " << generalizationSetAccuracy << "%, SSE: " << generalizationSetSSE << endl;
	
            bFound = true;
            break;
			}
			
			Population[i].fitness = AssignFitness(Population[i].bits, errorThreshold, ceil(mseForTestingFitness));
         TotalFitness += Population[i].fitness;
			
			if (Population[i].fitness == 999.0f)
			{
				cout << "\nSolution found after " << GenerationsRequiredToFindASolution << " generations!" << endl << endl;
				cout << "Testing using the generalization data set" << endl;
				
				//get generalization set accuracy and SSE
				double generalizationSetAccuracy = nn.getSetAccuracy( d.tSet.generalizationSet );
				double generalizationSetSSE = nn.getSetSSE( d.tSet.generalizationSet );
	
				cout << "Generalization (Testing) Set Accuracy: " << generalizationSetAccuracy << "%, SSE: " << generalizationSetSSE << endl;
	         
            bFound = true;
            break;
			}
			
			// create a new population by selecting two parents at a time and creating offspring
			// by applying crossover and mutation. Do this until the desired number of offspring
			// have been created. 
		  
			//define some temporary storage for the new population we are about to create
			chromo_typ temp[POP_SIZE];

			int cPop = 0;
	      
			//loop until we have created POP_SIZE new chromosomes
			while (cPop < POP_SIZE)
			{
				
				// we are going to create the new population by grabbing members of the old population
				// two at a time via roulette wheel selection.
				string offspring1 = Roulette((int)TotalFitness, Population);
				string offspring2 = Roulette((int)TotalFitness, Population);

				
				//add crossover dependent on the crossover rate
				Crossover(offspring1, offspring2);
				
				//now mutate dependent on the mutation rate
				Mutate(offspring1);
				Mutate(offspring2);
            
				//add these offspring to the new population. (assigning zero as their fitness scores)
				temp[cPop++] = chromo_typ(offspring1, 0.0f);
				temp[cPop++] = chromo_typ(offspring2, 0.0f);
				
			}//end loop

			//copy temp population into main population array
			for (int i=0; i<POP_SIZE; i++)
			{
				Population[i] = temp[i];
			}
			
			++GenerationsRequiredToFindASolution;
      
			// exit app if no solution found within the maximum allowable number
			// of generations
			if (GenerationsRequiredToFindASolution > MAX_ALLOWABLE_GENERATIONS)
			{
				cout << "No solutions found after " << GenerationsRequiredToFindASolution << " Generations" << endl ;
				bFound = true;
				break;
			}
		}
	}
	
	cout << endl << endl << "-- END OF PROGRAM --" << endl;
}

