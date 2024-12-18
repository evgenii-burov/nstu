#include "graph.h"

using namespace std;

bool input_graph(vector < vector < int>>& graph);
bool BFS(const vector < vector < int>>& graph, const int& start, const int& destination);

std::vector<std::ifstream&> getInputStreams()
{
    int number_of_graphs = -1;
    std::ifstream input_stream;
    input_stream.open("n.txt");
    if (!input_stream)
    {
        throw std::string("n.txt file couldn't be opened");
    }
    input_stream >> number_of_graphs;
    if(number_of_graphs < 3)
        throw std::string("Incorrect parameter: number of graphs must be greater than 2");
    input_stream.close();
    std::vector<std::ifstream&> input_streams;
    for (int i = 1; i <= number_of_graphs; i++)
    {
        input_stream.open(std::to_string(i) + ".txt");
        input_streams.push_back(input_stream);
    }
    return input_streams;
}

int main()
{
    //try
    //{
    //    std::ifstream input_stream;
    //    input_stream.open("graph.txt");
    //    Graph g = Graph(input_stream);
    //    std::ofstream output_stream;
    //    output_stream.open("adjacency_list.txt");
    //    g.writeAdjacencyList(output_stream);
    //    output_stream.open("vertex_list.txt");
    //    g.breadthFirstSearch(output_stream, 1);
    //    g.printGraphComponents();
    //}
    //catch (std::string error)
    //{
    //    std::cout << error;
    //}
    std::vector<std::ifstream> input_streams;
    std::ifstream stream1;
    stream1.open("1.txt");
    std::ifstream stream2;
    stream1.open("2.txt");
    input_streams.push_back(stream1);
    int a;
    input_streams[0] >> a;
    std::cout << a;
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