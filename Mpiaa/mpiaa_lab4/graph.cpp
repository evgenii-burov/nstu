#include "graph.h"

Graph::Graph(std::ifstream& input_stream)
{
    if (!input_stream)
    {
        throw "Graph file couldn't be opened";
    }
    int numVertices0, numEdges0;
    input_stream >> numVertices0 >> numEdges0;
    if (numVertices0 < 1)
    {
        throw "Incorrect parameter: number of vertices must be positive";
    }
    if (numEdges0 < 0 || numEdges >numVertices0 * (numVertices0 - 1) / 2)
    {
        throw "Incorrect parameter: number of vertices must be positive";
    }
    numEdges = numEdges0;
    numVertices = numVertices0;
    graph.resize(numVertices);
    int a, b;
    for (int i = 0; i < numEdges; i++)
    {
        input_stream >> a >> b;
        graph[a - 1].push_back(b - 1);
        graph[b - 1].push_back(a - 1);
    }
    input_stream.close();
}

void Graph::writeAdjacencyList(std::ofstream &output_stream) const
{
    for (int i = 0; i < numVertices; i++)
    {
        output_stream << i + 1 << ": ";
        for (const auto& vertex : graph[i])
        {
            output_stream << vertex + 1 << "\t";
        }
        output_stream << "\n";
    }
    output_stream.close();
}

void Graph::BreadthFirstSearch(std::ofstream& output_stream, int start) const
{
    std::vector<bool> visited(graph.size(), false);
    std::queue<int> plan;
    plan.push(start);
    int currentVertex;
    while (!plan.empty())
    {
        currentVertex = plan.front();
        plan.pop();
        if (visited[currentVertex])
            continue;
        output_stream << currentVertex << "\t";
        visited[currentVertex] = true;
        for (const auto& adjVertex : graph[currentVertex]) {
            if (!visited[adjVertex])
                plan.push(adjVertex);
        }
    }
    output_stream.close();
}