main.exe		:		main.o	dataReader.o	neuralNetwork.o	neuralNetworkTrainer.o	ga.o	 
	g++ -Wl,-s -o main.exe main.o dataReader.o	neuralNetwork.o	neuralNetworkTrainer.o	ga.o

main.o		:		main.cpp	 dataEntry.h	 dataReader.h	neuralNetwork.h	neuralNetworkTrainer.h
	g++ -c -fpermissive -fconserve-space main.cpp

dataReader.o		:		dataReader.cpp	dataReader.h
	g++ -c -fpermissive -fconserve-space dataReader.cpp

neuralNetwork.o		:		neuralNetwork.cpp	neuralNetwork.h
	g++ -c -fpermissive -fconserve-space neuralNetwork.cpp

neuralNetworkTrainer.o		:		neuralNetworkTrainer.cpp	neuralNetworkTrainer.h
	g++ -c -fpermissive -fconserve-space neuralNetworkTrainer.cpp

ga.o		:		ga.cpp	ga.h
	g++ -c -fpermissive -fconserve-space ga.cpp
