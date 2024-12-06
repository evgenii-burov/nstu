#include "graph.h"

using namespace std;

bool input_graph(vector < vector < int>>& graph);
bool BFS(const vector < vector < int>>& graph, const int& start, const int& destination);

int main()
{
    std::ifstream input_stream;
    input_stream.open("graph.txt");
    Graph g = Graph(input_stream);
    std::ofstream output_stream;
    output_stream.open("adjacency_list.txt");
    g.writeAdjacencyList(output_stream);
    output_stream.open("vertex_list.txt");
    g.BreadthFirstSearch(output_stream, 0);
}

bool input_graph(vector < vector < int>>& graph)
{
    ifstream in("graph10.txt");
    if (in)
    {
        int numVertices, numEdges;
        in >> numVertices >> numEdges;
        graph.resize(numVertices);
        int a, b;
        for (int i = 0; i < numEdges; i++)
        {
            in >> a >> b;
            graph[a-1].push_back(b-1);
            graph[b-1].push_back(a-1);
        }
        in.close();
        return true;
    }
    else
        return false;
}

bool BFS(const vector < vector < int>>& graph, const int& start, const int& destination)
{
    vector<bool> visited(graph.size(), false);
    queue<int> plan;
    plan.push(start);
    int currentVertex;
    while (!plan.empty())
    {
        currentVertex = plan.front();
        plan.pop();
        if (currentVertex == destination)
            return true;
        if (visited[currentVertex])
            continue;
        visited[currentVertex] = true;
        for (const auto& adjVertex : graph[currentVertex]) {
            if (!visited[adjVertex])
                plan.push(adjVertex);
        }
    }
    return false;
}

bool DFS(const vector < vector < int>>& graph, const int& start, const int& destination)
{
    vector<bool> visited(graph.size(), false);
    stack<int> plan;
    plan.push(start);
    int currentVertex;
    while (!plan.empty())
    {
        currentVertex = plan.top();
        plan.pop();
        if (currentVertex == destination)
            return true;
        if (visited[currentVertex])
            continue;
        visited[currentVertex] = true;

        for (const auto& adjVertex : graph[currentVertex]) {
            if (!visited[adjVertex])
                plan.push(adjVertex);
        }
        for (int i = 0; i < graph[currentVertex].size(); i++) {
            if (!visited[graph[currentVertex][i]])
                plan.push(graph[currentVertex][i]);
        }
    }
    return false;
}