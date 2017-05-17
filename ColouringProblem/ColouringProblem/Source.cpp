#include<iostream>
#include <fstream>
#include<sstream>
#include<list>
#include<vector>
#include <string>
#include <random>

std::vector< std::vector<int> > graph;
int const GRAPH_BLOCKS = 16;
int const POPULATION_SIZE = 100;
int bestSolutionPosition;
int const PARENT_RECOMBINATION_NUMBER = 50;
std::vector< std::vector<int> > population(POPULATION_SIZE);
std::vector<int>fitness(POPULATION_SIZE);
int maxFitness = 0;

std::vector< std::vector<int> > touchingBlocks() {
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

	return graph;
}

void const printGraph() {

	//printing the vector
	for (int i = 0; i < graph.size(); ++i) {
		for (int j = 0; j < graph[i].size(); ++j) {
			std::cout << graph[i][j] << " ";
		}
		std::cout << "\n";
	}
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

 void const printPopulation() {
	 for (int i = 0; i < POPULATION_SIZE; ++i) {
		 for (int j = 0; j < GRAPH_BLOCKS; ++j) {
			 std::cout << population[i][j] << " ";
		 }
		 std::cout << "\n";
	 }
 }

std::vector<int> applyFitness() {
	 for (int i = 0; i < POPULATION_SIZE; ++i) {
		 for (int j = 0; j < GRAPH_BLOCKS; ++j) {
			 for (int k = 0; k < graph[j].size(); ++k) {
				 if (population[i][j] == population[i][graph[j][k] - 1]) {
					 fitness[i] += -1;
				 }
				 else {
					 fitness[i] += 1;
				 }
			 }
		 }
	 }
	 return fitness;
 }

 void const printFitness(std::vector<int> fitness) {
	 for (int i = 0; i < POPULATION_SIZE; ++i) {
		 std::cout << fitness[i] << "\n";
	 }
 }

 

  int rouletteWheelSelection(int totalFitness) {
	 std::random_device rd;  //Will be used to obtain a seed for the random number engine
	 std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	 std::uniform_int_distribution<> dis(0, totalFitness);
		 int randInt = dis(gen);
		 for (int j = 0; j<POPULATION_SIZE; ++j) {
			 if (randInt < fitness[j])
				 return j;
			 randInt -= fitness[j];
		 }
	 }


 int uniformSelection() {
	 std::vector< std::vector<int> >selectedParents;
	 std::random_device rd;  //Will be used to obtain a seed for the random number engine
	 std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	 std::uniform_int_distribution<> dis(0, POPULATION_SIZE);
	 return dis(gen);

	 }

 std::vector< std::vector<int> > newPopulationSelectionFromOld() {
	 int totalFitness = 0;
	 for (int vec : fitness) {
		 totalFitness += vec;
	 }
	 std::vector< std::vector<int> >selectedParents;
	 for (int i = 0; i < POPULATION_SIZE-PARENT_RECOMBINATION_NUMBER; ++i) {
		 int positionOfSelectedMember = rouletteWheelSelection(totalFitness);
		 selectedParents.push_back(population[positionOfSelectedMember]);
		 population[positionOfSelectedMember].erase(population[positionOfSelectedMember].begin(), population[positionOfSelectedMember].end());
		 fitness.erase(fitness.begin()+positionOfSelectedMember - 1);
		 //uncomment for uniform selection algorithm
		 //selectedParents[i] = population[uniformSelection()];
	 }
	 return selectedParents;
 }



 std::vector< std::vector<int> > parentRecombinationSelection(int totalFitness) {
	 std::vector< std::vector<int> >selectedParents;
	 for (int i = 0; i < PARENT_RECOMBINATION_NUMBER; ++i) {
		 selectedParents.push_back(population[rouletteWheelSelection(totalFitness)]);
		 //uncomment for uniform selection algorithm
		 //selectedParents[i] = population[uniformSelection()];
	 }
	 return selectedParents;
 }

 std::vector< std::vector<int> > parentMutationSelection(int totalFitness) {
	 std::vector< std::vector<int> >selectedParents;
	 for (int i = 0; i < PARENT_RECOMBINATION_NUMBER; ++i) {
		 selectedParents.push_back(population[rouletteWheelSelection(totalFitness)]);
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

 int calculateTotalFitness(){
	 int totalFitness = 0;
	 for (int vec : fitness) {
		 totalFitness += vec;
	 }
	 return totalFitness;
 }

 void assignMaxFitness() {
	 
	 for (std::vector<int> vec : graph) {
		 maxFitness += vec.size();
	 }
	 std::cout << "max Fitness" << maxFitness;

 }

 bool canTerminate() {
	 for (int i = 0; i < fitness.size(); ++i) {
		 if (fitness[i] == maxFitness) {
			 bestSolutionPosition = i;
			 return true;
		 }
	 } 
	 return false;
 }


 void printVector(std::vector<int> vectorToPrint) {
	 for (int vec : vectorToPrint) {
		 std::cout << vec << " ";
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
	 graph = touchingBlocks();
	 assignMaxFitness();
	 initialisePopulation();
	 printPopulation();
	 while (!canTerminate()) {
		 fitness = applyFitness();
		 int totalFitness = calculateTotalFitness();
		 printVector(fitness);
		 if (!canTerminate()) {
			 std::vector< std::vector<int> > selectedParents = parentRecombinationSelection(totalFitness);
			 printVector(selectedParents);
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
		 std::cout << population[bestSolutionPosition][i] ;
	 }
	 std::cout << "  Best Solution with Fitness" << fitness[bestSolutionPosition];
	 int i;
	std::cin >>  i;
	return 0;
}
