#include<iostream>
#include <fstream>
#include<sstream>
#include<list>
#include<vector>
#include <string>
#include <random>
#include "Source.h"

std::vector< std::vector<int> > graph;
int const GRAPH_BLOCKS = 16;
int const POPULATION_SIZE = 100;
int const PARENT_POPULATION_SIZE = 50;
std::vector< std::vector<int> > population(POPULATION_SIZE);
std::vector<int>fitness(POPULATION_SIZE);

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
	int totalFitness = 0;
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
		 totalFitness += fitness[i];
	 }
	 fitness.push_back(totalFitness);
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

 std::vector< std::vector<int> > parentSelection(int totalFitness) {
	 std::vector< std::vector<int> >selectedParents;
	 for (int i = 0; i < PARENT_POPULATION_SIZE; ++i) {
		 selectedParents.push_back(population[rouletteWheelSelection(totalFitness)]);
		 //uncomment for uniform selection algorithm
		 //selectedParents[i] = population[uniformSelection()];
	 }
	 return selectedParents;
 }

int main() {
	graph = touchingBlocks();
	initialisePopulation();
	printPopulation();
	fitness = applyFitness();
	int totalFitness = fitness.back();
	fitness.pop_back();
	printFitness(fitness);
	std::vector< std::vector<int> > selectedParents = parentSelection(totalFitness);
	for (std::vector<int> vec : selectedParents) {
		for (int vecc : vec) {
			std::cout << vecc << " ";
		}
		std::cout << "\n";
	}
	
	int i;
	std::cin >>  i;
	return 0;
}
