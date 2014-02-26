#include <cstdio>
#include <cstdlib>

using namespace std;

// A timer to timestamp the vertices at their discovery and finishing
int time;

// A counter for sorted nodes 
int counter = 0;

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
    int dist_from_source;      // for bfs
    int color;                 // for dfs color = 0 means White, 1 means Gray, 2 means Black
    int parent_value;          // value of parent vertex / node
    int discovery_time;        // discoverty time of the vertex
    int finished_time;         // time when the work with the vertex is finished
    struct AdjList* parent;    // parent / predecessor for dfs
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

int source;
// Print Path from Source to the query Vertex
void printPath(struct Graph *graph, int queryVertex)
{
    if(source == queryVertex)
        printf("%d ",source);
    else if (graph->array[queryVertex].parent_value == -1)
        printf("No path from source to queryVertex Exists");
    else 
        {
            printPath(graph, graph->array[queryVertex].parent_value);
            printf("%d ",queryVertex);
        }
}


// Prints the edges of the graph
void printGraph(struct Graph *graph)
{
    struct AdjList* node;
    struct AdjListNode* temp;

    for(int i = 0; i < graph->V; ++i)
    {
        node = &graph->array[i];
        printf("\n\nOut-Degree of vertex %d = %d\nIn-Degree of vertex %d = %d\n", i, outDeg(graph, i), i, inDeg(graph,i));
        printf("distance from source of %d = %d\n", i, node->dist_from_source);
        printf("Discovery Time = %d Finished Time = %d\n",node->discovery_time, node->finished_time);
        printPath(graph, i);
        printf("\n");

        temp = node->head;

        while(temp != NULL)
        {
            printf("%d to %d with weight %d\n", i, temp->dest, temp->weight);
            temp = temp->next;
        }
    }
}
// dfs
void dfs_visit(struct Graph *graph, int visited_node_val, int num)  
{
	struct AdjList* node = &graph->array[visited_node_val];
	struct AdjListNode* temp;

	time = time + 1;
    node->discovery_time = time;    // white vertex discovered
    node->color = 1;

    temp = node->head;
    while(temp != NULL)       // explore edges
    {
    	if(graph->array[temp->dest].color == 0)
    	{
            if(num == 1)
                printf("%d ",temp->dest);
    		graph->array[temp->dest].parent_value = visited_node_val;
    		graph->array[temp->dest].parent = node;
    		dfs_visit(graph, temp->dest, num);
    	}
    	temp = temp->next;
    }

    node->color = 2;                // blacken vertex as it is finished
    time += 1;
    node->finished_time = time;
}

void dfs(struct Graph *graph)   
{
	struct AdjList* node;
	for(int i = 0; i < graph->V; ++i)
	{
		node = &graph->array[i];
		node->color = 0;
		node->parent = NULL;
		node->parent_value = -1;
	}

	time = 0;

	for(int i = 0; i < graph->V; ++i)
	{
		node = &graph->array[i];

		if(node->color == 0)
			dfs_visit(graph, i, 0);    // Just this function modification
	}
}

// Modified Dfs for transpose graph
void modified_dfs(struct Graph *graph, int *sorted_nodes)
{
    struct AdjList* node;
    for(int i = 0; i < graph->V; ++i)
    {
        node = &graph->array[i];
        node->color = 0;
        node->parent = NULL;
        node->parent_value = -1;
    }

    time = 0;

    for(int i = 0; i < graph->V; ++i)
    {
        node = &graph->array[sorted_nodes[i]];           // Here is the modification part .. called in decreasing order

        if(node->color == 0)
            {
                dfs_visit(graph, sorted_nodes[i], 1);
                printf("%d \n", sorted_nodes[i]);
            }
    }
}

// Creating Transpose Graphs i.e. reverse edged graph
struct Graph* createTransposeGraph(struct Graph* graph)
{
    struct Graph* tempGraph = createGraph(graph->V);
    struct AdjListNode* node;

    for(int i = 0; i < graph->V; ++i)
    {
        node = graph->array[i].head;
        while(node != NULL)
        {
            addEdge(tempGraph, node->dest, i, node->weight, 1);  // Note that given Graph will always be directed
            node = node->next;
        }
    }
    return tempGraph;
}

// Calculating Sorted Nodes
void CalculateSortedNodes(int *sorted_nodes, struct Graph* graph)
{
    for(int i = 0; i < graph->V; ++i)
    {
        for(int j = 0; j < i; ++j)
        {
            if(graph->array[j].finished_time < graph->array[j+1].finished_time)
            {
                int temp1,temp2;
                temp1 = graph->array[j].finished_time;
                temp2 = sorted_nodes[j];
                graph->array[j].finished_time = graph->array[j+1].finished_time;
                sorted_nodes[j] = sorted_nodes[j+1];
                graph->array[j+1].finished_time = temp1;
                sorted_nodes[j+1] = temp2;
            }
        }
    }
}

 // Driver program to test above functions
int main()
{
    int numVertices, directed, src, dest, weight;
    int *sorted_nodes;
    
    printf("Enter the number of vertices\n");
    scanf("%d", &numVertices);
    struct Graph *graph, *graphTranspose;
    graph = createGraph(numVertices);
    sorted_nodes = (int *)malloc(numVertices * sizeof(int));
    
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
    
    for(int i = 0; i < graph->V; ++i)
        sorted_nodes[i] = i;

    graphTranspose = createTransposeGraph(graph);

    // dfs on original graph
    dfs(graph);

    CalculateSortedNodes(sorted_nodes, graph);
    
    printf("\nThe Connected Components Are :\n");
    // modified_dfs on transpose graph
    modified_dfs(graphTranspose, sorted_nodes);

	return 0;
}