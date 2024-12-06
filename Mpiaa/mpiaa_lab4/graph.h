#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <stack>

class Graph
{
private:
	int numEdges, numVertices;
	std::vector<std::vector<int>> graph;

public:
	Graph(std::ifstream& input_stream);

	void writeAdjacencyList(std::ofstream& output_stream) const;

	void BreadthFirstSearch(std::ofstream& output_stream, int start) const;
};