#include<iostream>
#include <fstream>
#include<sstream>
#include<list>
#include<vector>
#include <string>

int main() {
	std::string line, number = "";
	int i;
	std::ifstream colouredBlocks("blocks.txt");
	std::vector< std::vector<int> > graph;
	//open file of coloured blocks
	if (colouredBlocks.is_open()) {
		// delimit per new line
		while (getline(colouredBlocks, line)) {
			std::vector<int>subgraph;
			//splitting lines per comma
			std::stringstream ss(line);
			while (ss >> i) {
				//add each number to the variable number
				number+=std::to_string(i);
				//when meeting a comma, ignore it, parse the string to int and add it to the back of the subgraph
				//we don't insert the number directly because it might contain more than one digits
				if (ss.peek() == ',') {
					ss.ignore();
				}
				subgraph.push_back(stoi(number));
				number = "";
				//push the vector in the main vector and switch to the next line
				graph.push_back(subgraph);
				subgraph.erase(subgraph.begin(), subgraph.end());
			}
		}
		colouredBlocks.close();
	}
	else std::cout << "Unable to open file"	;

	
	//printing the vector
	for (int i = 0; i < graph.size(); i++) {
		for (int j = 0; j < graph[i].size(); j++) {
			std::cout << graph[i][j];
		}
	}

	std::cin >> i;
	
	/*int numberOfBlocks;
	std::cout << "Enter total number of blocks to be coloured: \n";
	std::cin >> numberOfBlocks;
	
	for (int i = 0; i < numberOfBlocks; i++) {
		while (true) {
			int number;
			std::cin >> number;
			subgraph.push_back(number);
		}
		graph.push_back(subgraph);
		}*/
	
	return 0;
}