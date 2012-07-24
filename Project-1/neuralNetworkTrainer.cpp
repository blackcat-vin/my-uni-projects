//standard includes
#include <iostream>
#include <fstream>
#include <math.h>

//include definition file
#include "neuralNetworkTrainer.h"

using namespace std;

/*******************************************************************
* constructor
********************************************************************/
neuralNetworkTrainer::neuralNetworkTrainer( neuralNetwork *nn )	:	NN(nn),
																	epoch(0),
																	learningRate(LEARNING_RATE),
																	momentum(MOMENTUM),
																	maxEpochs(MAX_EPOCHS),
																	desiredAccuracy(DESIRED_SSE),																	
																	useBatch(false),
																	trainingSetAccuracy(0),
																	validationSetAccuracy(0),
																	generalizationSetAccuracy(0),
																	trainingSetSSE(0),
																	validationSetSSE(0),
																	generalizationSetSSE(0)																	
{
	//create delta lists
	//--------------------------------------------------------------------------------------------------------
	deltaInputHidden = new( double*[NN->nInput + 1] );
	for ( int i=0; i <= NN->nInput; i++ ) 
	{
		deltaInputHidden[i] = new (double[NN->nHidden]);
		for ( int j=0; j < NN->nHidden; j++ ) deltaInputHidden[i][j] = 0;		
	}

	deltaHiddenOutput = new( double*[NN->nHidden + 1] );
	for ( int i=0; i <= NN->nHidden; i++ ) 
	{
		deltaHiddenOutput[i] = new (double[NN->nOutput]);			
		for ( int j=0; j < NN->nOutput; j++ ) deltaHiddenOutput[i][j] = 0;		
	}

	//create error gradient storage
	//--------------------------------------------------------------------------------------------------------
	hiddenErrorGradients = new( double[NN->nHidden + 1] );
	for ( int i=0; i <= NN->nHidden; i++ ) hiddenErrorGradients[i] = 0;
	
	outputErrorGradients = new( double[NN->nOutput + 1] );
	for ( int i=0; i <= NN->nOutput; i++ ) outputErrorGradients[i] = 0;
}


/*******************************************************************
* Set training parameters
********************************************************************/
void neuralNetworkTrainer::setTrainingParameters( double lR, double m, bool batch )
{
	learningRate = lR;
	momentum = m;
	useBatch = batch;
}
/*******************************************************************
* Set stopping parameters
********************************************************************/
void neuralNetworkTrainer::setStoppingConditions( int mEpochs, double dAccuracy )
{
	maxEpochs = mEpochs;
	desiredAccuracy = dAccuracy;
}
/*******************************************************************
* Enable training logging
********************************************************************/
void neuralNetworkTrainer::enableLogging(const char* filename, int resolution = 1)
{
	//create log file 
	if ( ! logFile.is_open() )
	{
		logFile.open(filename, ios::out);

		if ( logFile.is_open() )
		{
			
			
			//enable logging
			loggingEnabled = true;
			
			//resolution setting;
			logResolution = resolution;
			lastEpochLogged = -resolution;
		}
	}
}
/*******************************************************************
* calculate output error gradient
********************************************************************/
inline double neuralNetworkTrainer::getOutputErrorGradient( double desiredValue, double outputValue)
{
	//return error gradient
	return  (outputValue * ( 1 - outputValue ) *( desiredValue - outputValue ));
}	

/*******************************************************************
* calculate input error gradient
********************************************************************/
double neuralNetworkTrainer::getHiddenErrorGradient( int j )
{
	//get sum of hidden->output weights * output error gradients
	double weightedSum = 0;
	for( int k = 0; k < NN->nOutput; k++ ) weightedSum += NN->wHiddenOutput[j][k] * outputErrorGradients[k];

	//return error gradient
	return NN->hiddenNeurons[j] * ( 1 - NN->hiddenNeurons[j] ) * weightedSum;
}
/*******************************************************************
* Train the NN using gradient descent
********************************************************************/
double neuralNetworkTrainer::trainNetwork( trainingDataSet* tSet )
{
	
	clock_t startTime, endTime;
	cout	<< endl << "Neural Network Training Starting: " << endl
			<< "==========================================================================" << endl
			<< "LR: " << learningRate << ", Momentum: " << momentum << ", Max Epochs: " << maxEpochs << endl
			<< NN->nInput << " Input Neurons: " << NN->nHidden << " Hidden Neurons: " << NN->nOutput << " Output Neurons: " << endl
			<< "==========================================================================" << endl << endl;

	//reset epoch and log counters
	epoch = 0;
	lastEpochLogged = -logResolution;
	startTime = clock();	
	//train network using training dataset for training and stop training trainging using bothe training abd validation dataset
	//-------------------------------------------------------------------------------------------------------------------------
	
	//write log file header
	//~ logFile << "Epoch, Training Set SSE, Validation Set SSE" << endl;
	
	while ( epoch < maxEpochs )				
	{	
		//use training set to train network
		trainingSetSSE = runTrainingEpoch( tSet->trainingSet );
 
		//get validation set accuracy and SSE
	   validationSetAccuracy = NN->getSetAccuracy(tSet->validationSet);
	   validationSetSSE = NN->getSetSSE(tSet->validationSet);
		
		cout << "Epoch :" << epoch;
		cout << " TSet Acc:" << trainingSetAccuracy << "%, SSE: " << trainingSetSSE ;
		cout << " VSet Acc:" << validationSetAccuracy << "%, SSE: " << validationSetSSE << endl;
		
		
		//Log Training results
		if ( loggingEnabled && logFile.is_open() && ( epoch - lastEpochLogged == logResolution ) ) 
		{
			logFile << epoch << "," << trainingSetSSE << "," << validationSetSSE << endl;
			lastEpochLogged = epoch;
		}
		
		
		//print out change in training / validation mse
		if ( trainingSetSSE <= desiredAccuracy && validationSetSSE <= desiredAccuracy) 
		{	
			desiredAccuracy /= 2; // successively lower the error threshold
			cout << "======================================" << endl;
			cout << "Lowering the error threshold to " << desiredAccuracy << endl;
			cout << "======================================" << endl;
			if((trainingSetSSE < 0.006) && (validationSetSSE < 0.006)) { // stop when error threshold below 0.007 is reached
				break;
			}	
		}
		//~ else {
			//~ learningRate /= 2; // successively lowering the learning rate if desired training and validation error level not reached
		//~ }	
		
		//once training set is complete increment epoch
		epoch++;
		

	}//end while
	
   cout << endl; 
	endTime = clock();
	cout << "Time taken for backpropagation training = " << (endTime - startTime)/1000/60 << " minutes" << endl;
	
	//get training set accuracy and SSE
	trainingSetAccuracy = NN->getSetAccuracy( tSet->trainingSet );
	trainingSetSSE = NN->getSetSSE( tSet->trainingSet );
	
	//get validation set accuracy and SSE
	validationSetAccuracy = NN->getSetAccuracy(tSet->validationSet);
	validationSetSSE = NN->getSetSSE(tSet->validationSet);
	
	//out validation accuracy and MSE
	cout << endl << "Training Complete!!! - > Elapsed Epochs: " << epoch << endl;
	
	return trainingSetSSE;
}
/*******************************************************************
* Run a single training epoch
********************************************************************/
double neuralNetworkTrainer::runTrainingEpoch( vector<dataEntry*> trainingSet )
{
	//incorrect patterns
	double incorrectPatterns = 0;
	double sse = 0;
	
	
	//for every training pattern
	for ( int tp = 0; tp < (int) trainingSet.size(); tp++)
	{						
		//feed inputs through network and backpropagate errors
		NN->feedForward( trainingSet[tp]->pattern );
		backpropagate( trainingSet[tp]->target );	

		//pattern correct flag
		bool patternCorrect = true;

		//check all outputs from neural network against desired values
		for ( int k = 0; k < NN->nOutput; k++ )
		{		
			//int temp = NN->clampOutput( NN->outputNeurons[k] ) ;
			//
		   //out << temp << "," << trainingSet[tp]->target[k] << endl;
			//pattern incorrect if desired and output differ
			if ( NN->clampOutput( NN->outputNeurons[k] ) != trainingSet[tp]->target[k] ) {
				patternCorrect = false;  
				  
			}	
			//calculate SSE
			sse +=  pow(( NN->outputNeurons[k] - trainingSet[tp]->target[k] ), 2);
		}
		
		//if pattern is incorrect add to incorrect count
		if ( !patternCorrect ) incorrectPatterns++;	
		
	}//end for
   
	//if using batch learning - update the weights
	if ( useBatch ) updateWeights();
	
	//update training accuracy and SSE
	trainingSetAccuracy = 100 - (incorrectPatterns/trainingSet.size() * 100);
	trainingSetSSE = (0.5 * sse) / ( NN->nOutput * trainingSet.size() );
	
	return trainingSetSSE;
	
}
/*******************************************************************
* Propagate errors back through NN and calculate delta values
********************************************************************/
void neuralNetworkTrainer::backpropagate( double* desiredOutputs )
{		
	//modify deltas between hidden and output layers
	//--------------------------------------------------------------------------------------------------------
	for (int k = 0; k < NN->nOutput; k++)
	{
		//get error gradient for every output node
		outputErrorGradients[k] = getOutputErrorGradient( desiredOutputs[k], NN->outputNeurons[k] );
		
		//for all nodes in hidden layer and bias neuron
		for (int j = 0; j <= NN->nHidden; j++) 
		{				
			//calculate change in weight
			if ( !useBatch ) deltaHiddenOutput[j][k] = learningRate * NN->hiddenNeurons[j] * outputErrorGradients[k] + momentum * deltaHiddenOutput[j][k];
			else deltaHiddenOutput[j][k] += learningRate * NN->hiddenNeurons[j] * outputErrorGradients[k];
		}
	}

	//modify deltas between input and hidden layers
	//--------------------------------------------------------------------------------------------------------
	for (int j = 0; j < NN->nHidden; j++)
	{
		//get error gradient for every hidden node
		hiddenErrorGradients[j] = getHiddenErrorGradient( j );

		//for all nodes in input layer and bias neuron
		for (int i = 0; i <= NN->nInput; i++)
		{
			//calculate change in weight 
			if ( !useBatch ) deltaInputHidden[i][j] = learningRate * NN->inputNeurons[i] * hiddenErrorGradients[j] + momentum * deltaInputHidden[i][j];
			else deltaInputHidden[i][j] += learningRate * NN->inputNeurons[i] * hiddenErrorGradients[j]; 

		}
	}
	
	//if using stochastic learning update the weights immediately
	if ( !useBatch ) updateWeights();
}
/*******************************************************************
* Update weights using delta values
********************************************************************/
void neuralNetworkTrainer::updateWeights()
{
	//input -> hidden weights
	//--------------------------------------------------------------------------------------------------------
	for (int i = 0; i <= NN->nInput; i++)
	{
		for (int j = 0; j < NN->nHidden; j++) 
		{
			//update weight
			NN->wInputHidden[i][j] += deltaInputHidden[i][j];	
			
			//clear delta only if using batch (previous delta is needed for momentum
			if (useBatch) deltaInputHidden[i][j] = 0;				
		}
	}
	
	//hidden -> output weights
	//--------------------------------------------------------------------------------------------------------
	for (int j = 0; j <= NN->nHidden; j++)
	{
		for (int k = 0; k < NN->nOutput; k++) 
		{					
			//update weight
			NN->wHiddenOutput[j][k] += deltaHiddenOutput[j][k];
			
			//clear delta only if using batch (previous delta is needed for momentum)
			if (useBatch)deltaHiddenOutput[j][k] = 0;
		}
	}
}
