#include <cstdio>
#include <cstdlib>

using namespace std;

// A structure to represent a node in adjacency list
struct AdjListNode
{
    int dest;
    int weight;
    struct AdjListNode* next;
};
 
// A structure to represent an adjacency liat
struct AdjList
{
    struct AdjListNode *head;  // pointer to head node of list
};
 
// A structure to represent a graph. A graph is an array of adjacency lists.
// Size of array will be V (number of vertices in graph)
struct Graph
{
    int V;
    struct AdjList* array;
};
 
// A utility function to create a new adjacency list node
struct AdjListNode* newAdjListNode(int dest, int weight)
{
    struct AdjListNode* newNode =
            (struct AdjListNode*) malloc(sizeof(struct AdjListNode));
    newNode->dest = dest;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}
 
// A utility function that creates a graph of V vertices
struct Graph* createGraph(int V)
{
    struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));
    graph->V = V;
 
    // Create an array of adjacency lists.  Size of array will be V
    graph->array = (struct AdjList*) malloc(V * sizeof(struct AdjList));
 
     // Initialize each adjacency list as empty by making head as NULL
    for (int i = 0; i < V; ++i)
        graph->array[i].head = NULL;
 
    return graph;
}
 
// Adds an edge to graph
void addEdge(struct Graph* graph, int src, int dest, int weight, int directed)
{
    // Add an edge from src to dest.  A new node is added to the adjacency
    // list of src.  The node is added at the begining
    struct AdjListNode* newNode = newAdjListNode(dest, weight);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;
 
    if(directed == 0)
    {
    // Since graph is undirected, add an edge from dest to src also
    newNode = newAdjListNode(src, weight);
    newNode->next = graph->array[dest].head;
    graph->array[dest].head = newNode;
    }
}

// Calculates out-degree of a vertex
//// Note : out-degree can be calculated
//// quickly at the time of scanning itself
int outDeg(struct Graph *graph, int queryVertex)
{
    struct AdjListNode* node;
    int count=0;

    node = graph->array[queryVertex].head;

    while(node!=NULL)
    {
        ++count;
        node = node->next;
    }
    return count;
}

// Calculates in-degree of a vertex
//// Note : in-degree can be calculated
//// quickly at the time of scanning itself
int inDeg(struct Graph *graph, int queryVertex)
{
    struct AdjListNode* node;
    int count=0;

    for(int i = 0; i < graph->V; ++i)
    {
        node = graph->array[i].head;
        while(node!=NULL)
        {
            if(node->dest == queryVertex)
                ++count;
            node = node->next;
        }
    }
    return count;
}

// Prints the edges of the graph
void printGraph(struct Graph *graph)
{
    struct AdjListNode* node;

    for(int i = 0; i < graph->V; ++i)
    {
        printf("Out-Degree of vertex %d = %d\nIn-Degree of vertex %d = %d\n", i, outDeg(graph, i), i, inDeg(graph,i));
        node = graph->array[i].head;
        while(node != NULL)
        {
            printf("%d to %d with weight %d\n", i, node->dest, node->weight);
            node = node->next;
        }
    }
}


 // Driver program to test above functions
int main()
{
    int numVertices, directed, src, dest, weight;
    
    printf("Enter the number of vertices\n");
    scanf("%d", &numVertices);
    struct Graph* graph = createGraph(numVertices);
    
    printf("Enter 1 if graph is directed and 0 if undirected : \n");
    scanf("%d", &directed);

	// Assuming Indexing of vertices start from 0...
    printf("Enter source destination and weight in this order only and to stop enter -1 -1 -1\n ***Note: zero indexed vertices*** \n");
	while(src!=-1 && dest!=-1 && weight!=-1)
	{
		scanf("%d%d%d", &src, &dest, &weight);
        if(src!=-1 && dest!=-1 && weight!=-1)
        {
            if(src >= graph->V || dest >= graph->V)printf("Wrong Entry : Remember zero indexing is there..\n");
            else
        	addEdge(graph, src, dest, weight, directed);
        }
	}

	// Print the graph
    printGraph(graph);


	return 0;
}