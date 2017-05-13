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
int population[POPULATION_SIZE][GRAPH_BLOCKS];
std::vector<int>fitness;

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
			population[i][j] = dis(gen);
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

std::vector<int> initializeFitness() {
	 for (int i = 0; i < POPULATION_SIZE; ++i) {
		 fitness.push_back(0);
	 }
	 return fitness;
 }

std::vector<int> applyFitness() {
	 for (int i = 0; i < POPULATION_SIZE; ++i) {
		 for (int j = 0; j < GRAPH_BLOCKS; ++j) {
			 for (int k = 0; k < graph[j].size(); ++k) {
				 if (population[i][j] == population[i][graph[j][k] - 1]) {
					 fitness.at(i) += 0;
				 }
				 else {
					 fitness.at(i) += 1;
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

int main() {
	graph = touchingBlocks();
	initialisePopulation();
	printPopulation();
	fitness = initializeFitness();
	fitness = applyFitness();
	printFitness(fitness);
	int i;
	std::cin >>  i;
	return 0;
}
