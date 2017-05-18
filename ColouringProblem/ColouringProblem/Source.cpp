#include<iostream>
#include <fstream>
#include<sstream>
#include<list>
#include<vector>
#include <string>
#include <random>

std::vector< std::vector<int> > graph;
int GRAPH_BLOCKS;
int const POPULATION_SIZE = 100;
int bestSolutionPosition;
int const PARENT_RECOMBINATION_NUMBER = 50;
std::vector< std::vector<int> > population(POPULATION_SIZE);
std::vector<int>fitness(POPULATION_SIZE);
int maxFitness = 0;
std::vector<int>bestMember;
int bestFitness = 0;
bool shouldStop = false; 
int NUMBER_OF_ITERATIONS_FOR_CHANGES = 10;
int const NEGATIVE_FITNESS_POINTS = -1;
int const POSITIVE_FITNESS_POINTS = 1;

std::vector< std::vector<int> > getTouchingBlocks() {
	std::string line, number = "";
	int i;
	std::ifstream colouredBlocks("blocks.txt");
	//open file of coloured blocks
	if (colouredBlocks.is_open()) {
		// delimit per new line
		while (getline(colouredBlocks, line)) {
			std::vector<int>subgraph;
			//splitting lines per comma
			std::stringstream ss(line);
			while (ss >> i) {
				//add each number to the variable number
				number += std::to_string(i);
				//when meeting a comma, ignore it, parse the string to int and add it to the back of the subgraph
				//we don't insert the number directly because it might contain more than one digits
				if (ss.peek() == ',') {
					ss.ignore();
				}
				subgraph.push_back(stoi(number));
				number = "";
				//push the vector in the main vector and switch to the next line
			}
			graph.push_back(subgraph);
		}
		colouredBlocks.close();
	}
	else std::cout << "Unable to open file";
	GRAPH_BLOCKS = graph.size();
	return graph;
}

 void initialisePopulation() {
	 
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<> dis(0, 3);
	for (int i = 0; i < POPULATION_SIZE; ++i) {
		for (int j = 0; j < GRAPH_BLOCKS; ++j) {
			population[i].push_back(dis(gen));
		}
	}
}

std::vector<int> applyFitness() {
	 for (int i = 0; i < POPULATION_SIZE; ++i) {
		 fitness[i] = maxFitness;
		 for (int j = 0; j < GRAPH_BLOCKS; ++j) {
			 for (int k = 0; k < graph[j].size(); ++k) {
				 if (population[i][j] == population[i][graph[j][k] - 1]) {
					 fitness[i] += NEGATIVE_FITNESS_POINTS;
				 }
				 else {
					 fitness[i] += POSITIVE_FITNESS_POINTS;
				 }
			 }
		 }
	 }
	 return fitness;
 }

int calculateTotalFitness() {
	int totalFitness = 0;
	for (int vec : fitness) {
		totalFitness += vec;
	}
	return totalFitness;
}

  int rouletteWheelSelection(int maxSize) {
	 std::random_device rd;  //Will be used to obtain a seed for the random number engine
	 std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	 std::uniform_int_distribution<> dis(0, calculateTotalFitness()-1);
		 int randInt = dis(gen);
		 for (int j = 0; j<maxSize; ++j) {
			 if (randInt < fitness[j])
				 return j;
			 randInt -= fitness[j];
		 }
	 }

 std::vector< std::vector<int> > newPopulationSelectionFromOld() {
	 std::vector< std::vector<int> >selectedMembers;
	 for (int i = 0; i < POPULATION_SIZE-PARENT_RECOMBINATION_NUMBER; ++i) {
		 int selectedMemberPosition = rouletteWheelSelection(POPULATION_SIZE - PARENT_RECOMBINATION_NUMBER);
		 selectedMembers.push_back(population[selectedMemberPosition]);
		 fitness[selectedMemberPosition] = -maxFitness;
	 }
	 return selectedMembers;
 }



 std::vector< std::vector<int> > parentRecombinationSelection() {
	 std::vector< std::vector<int> >selectedParents;
	 for (int i = 0; i < PARENT_RECOMBINATION_NUMBER; ++i) {
		 int temp = rouletteWheelSelection(POPULATION_SIZE);
		 selectedParents.push_back(population[temp]);
		 //uncomment for uniform selection algorithm
		 //selectedParents[i] = population[uniformSelection()];
	 }
	 return selectedParents;
 }

 std::vector< std::vector<int> > ParentRecombination(std::vector< std::vector<int> > selectedParents){
	 std::vector< std::vector<int> > newPopulation;
	 std::random_device rd;  //Will be used to obtain a seed for the random number engine
	 std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	 std::uniform_int_distribution<> dis(0, GRAPH_BLOCKS-1);
	 int splitPoint = dis(gen);
	 for (int i = 0; i < PARENT_RECOMBINATION_NUMBER; i+=2) {
		 std::vector<int> childA, childB;
		 for (int j = 0; j < splitPoint; ++j) {
			 childA.push_back(selectedParents[i][j]);
			 childB.push_back(selectedParents[i + 1][j]);
		 }	
		 for (int j = splitPoint; j < GRAPH_BLOCKS; ++j) {
			 childA.push_back(selectedParents[i + 1][j]);
			 childB.push_back(selectedParents[i][j]);
		 }
		 newPopulation.push_back(childA);
		 newPopulation.push_back(childB);
	 }
	 return newPopulation;
 }

 std::vector < std::vector<int> >childrenMutationCreation(std::vector < std::vector<int> > selectedPopulation) {
	 std::vector< std::vector<int> > mutatedPopulation;
	 std::random_device rd;  //Will be used to obtain a seed for the random number engine
	 std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	for (int i = 0; i < selectedPopulation.size(); ++i) {
		 std::uniform_int_distribution<> dis(0, 9);
		 int randomNumber = dis(gen);
		 if (randomNumber == 0) {
			 std::uniform_int_distribution<> disA(0, GRAPH_BLOCKS - 1);
			 int mutationPoint = disA(gen);
			 int previousColour = selectedPopulation[i][mutationPoint];
			 std::uniform_int_distribution<> disB(0, 3);
			 int newColour = disB(gen);
			 while (newColour == previousColour) {
				 newColour = disB(gen);
			 }
			 selectedPopulation[i][mutationPoint] = newColour;
			 
		 }
		 mutatedPopulation.push_back(selectedPopulation[i]);
		 }
	 return mutatedPopulation;
 }

 void assignMaxFitness() {
	 int maxFitness = 0;
	 for (std::vector<int> vec : graph) {
		 maxFitness += vec.size();
	 }
 }

 bool wasPerfectSolutionFound() {
	 for (int i = 0; i < fitness.size(); ++i) {
		 if (fitness[i] == 2 * maxFitness) {
			 bestSolutionPosition = i;
			 bestMember = population[i];
			 return true;
		 }
	 }
	 return false;
 }

 bool wasNoSignificantChangeNoticed() {
 
 }

 void findBestMember() {
	 for (int i = 0; i < POPULATION_SIZE; ++i) {
		 if (bestFitness < fitness[i]) {
			 bestFitness = fitness[i];
			 bestMember = population[i];
		 }
	 }
 }


 bool canTerminate() {
	 if (wasPerfectSolutionFound()) {
		 shouldStop = true;
		 return true;
	 }
	 else if (wasNoSignificantChangeNoticed){
		 shouldStop = true;
		 return true;
	 }
	 else {
		 shouldStop = false;
		 return false;
	 }

 }


 void printVector(std::vector<int> vectorToPrint) {
	 for (int vec : vectorToPrint) {
		 std::cout << vec << "\n ";
	 }
	 std::cout << "\n";
 }

 void printVector(std::vector< std::vector<int> > vectorToPrint) {
	 for (std::vector<int> vec : vectorToPrint) {
		 for (int vecc : vec) {
			 std::cout << vecc<< " ";
		 }
		 std::cout << "\n";
	 }
	 std::cout << "\n";
 }

 int main() {
	 graph = getTouchingBlocks();
	 assignMaxFitness();
	 initialisePopulation();
	 printVector(population);
	 while (!canTerminate()) {
		 fitness = applyFitness();
		 int totalFitness = calculateTotalFitness();
		 std::cout << totalFitness;
		 printVector(fitness);
		 if (!shouldStop) {
			 std::vector< std::vector<int> > selectedParents = parentRecombinationSelection();
			 //printVector(selectedParents);
			 std::vector< std::vector<int> > newPopulation = ParentRecombination(selectedParents);
			 std::vector< std::vector<int> > newPopulationSelectedFromOlder = newPopulationSelectionFromOld();
			 for (std::vector<int> vec : newPopulationSelectedFromOlder) {
				 newPopulation.push_back(vec);
			 }
			 newPopulation = childrenMutationCreation(newPopulation);
			 population = newPopulation;
			 }
	 }
	 std::cout<< "\n \n \n \n \n \n \n \n \n" ;
	 for (int i = 0; i < GRAPH_BLOCKS; ++i) {
		 std::cout << bestMember[i] ;
	 }
	 std::cout << "  Best Solution with Fitness" << bestFitness;
	 int i;
	std::cin >>  i;
	return 0;
}
