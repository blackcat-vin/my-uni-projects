#ifndef NNetwork
#define NNetwork

#include "dataReader.h"

class neuralNetworkTrainer;

class neuralNetwork
{
	//class members
	//--------------------------------------------------------------------------------------------
private:

	//number of neurons
	int nInput, nHidden, nOutput;
	
	//neurons
	double* inputNeurons;
	double* hiddenNeurons;
	double* outputNeurons;

	//weights
	double** wInputHidden;
	double** wHiddenOutput;
		
	//Friends
	//--------------------------------------------------------------------------------------------
	friend class neuralNetworkTrainer;
	
	//public methods
	//--------------------------------------------------------------------------------------------

public:

	//constructor & destructor
	neuralNetwork(int numInput, int numHidden, int numOutput);
	~neuralNetwork();

	//weight operations
	bool loadWeights(char* inputFilename);
	bool saveWeights(char* outputFilename);
	double* feedForwardPattern( double* pattern );
	double getSetAccuracy( std::vector<dataEntry*>& set );
	double getSetSSE( std::vector<dataEntry*>& set );

	//private methods
	//--------------------------------------------------------------------------------------------

private: 
	
	void initializeWeights();
	inline double activationFunction( double x );
	double clampOutput( double x );
	void feedForward( double* pattern );
	
};

#endif
