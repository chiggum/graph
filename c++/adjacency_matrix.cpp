#include <cstdio>
#include <cstdlib>

using namespace std;

// A structure to represent an entry in the matrix
struct Node
{
	int edge;    // for directed graph from to to edge..
	int weight;
};

// Creates the graph
Node** createGraph(int numVertices)
{
	Node **graph = (Node **)malloc(numVertices * sizeof(Node*));

	for(int i = 0; i < numVertices; ++i)
	{
		graph[i] = (Node *)malloc(numVertices * sizeof(Node));
	}
	return graph;
}

// Adds an edge .. for both directed and undirected
void addEdge(struct Node** graph, int src, int dest, int weight, int directed)
{
	graph[src][dest].edge = 1;
	graph[src][dest].weight = weight;
	if(directed == 0)
	{
		graph[dest][src].edge = 1;
		graph[dest][src].weight = weight;
	}
}

//calculates out-degree .. can be calculate while scanning
int outDeg(struct Node **graph, int queryVertex, int numVertices)
{
	int count = 0;

	for(int i = 0; i < numVertices; ++i)
	{
		if(graph[queryVertex][i].edge == 1)
			++count;
	}
	return count;
}

////calculates in-degree .. can be calculated while scanning
int inDeg(struct Node **graph, int queryVertex, int numVertices)
{
	int count = 0;

	for(int i = 0; i < numVertices; ++i)
	{
		if(graph[i][queryVertex].edge == 1)
			++count;
	}
	return count;
}

// Prints graph
void printGraph(struct Node **graph,int numVertices)
{
	for(int i = 0; i < numVertices; ++i)
    {
        printf("Out-Degree of vertex %d = %d\nIn-Degree of vertex %d = %d\n", i, outDeg(graph, i, numVertices), i, inDeg(graph,i,numVertices));
        for(int j = 0; j < numVertices; ++j)
        {
        	if(graph[i][j].edge!=0)
            printf("%d to %d with weight %d\n", i, j, graph[i][j].weight);
        }
    }
}

int main()
{
	int numVertices, directed, src, dest, weight;
    
    printf("Enter the number of vertices\n");
    scanf("%d", &numVertices);
    struct Node **graph = createGraph(numVertices);

    printf("Enter 1 if graph is directed and 0 if undirected : \n");
    scanf("%d", &directed);

	// Assuming Indexing of vertices start from 0...
    printf("Enter source destination and weight in this order only and to stop enter -1 -1 -1\n ***Note: zero indexed vertices*** \n");
	while(src!=-1 && dest!=-1 && weight!=-1)
	{
		scanf("%d%d%d", &src, &dest, &weight);
        if(src!=-1 && dest!=-1 && weight!=-1)
        {
            if(src >= numVertices || dest >= numVertices)printf("Wrong Entry : Remember zero indexing is there..\n");
            else
        	addEdge(graph, src, dest, weight, directed);
        }
	}

	// Print the graph
    printGraph(graph, numVertices);

    return 0 ;
}