#include<iostream>
#include <fstream>
#include<sstream>
#include<list>
#include<vector>
#include <string>
std::vector< std::vector<int> > graph;

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
			// TODO: remove; is unnecessary
			subgraph.erase(subgraph.begin(), subgraph.end());
		}
		colouredBlocks.close();
	}
	else std::cout << "Unable to open file";

	return graph;
}

void printGraph() {

	//printing the vector
	for (int i = 0; i < graph.size(); i++) {
		for (int j = 0; j < graph[i].size(); j++) {
			std::cout << graph[i][j] << " ";
		}
		std::cout << "\n";
	}
}

void initialisePopulation() {

}

int main() {
	graph = touchingBlocks();
	int population[100][16];


	std::cin >> i;
	return 0;
}
