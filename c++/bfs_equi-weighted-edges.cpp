#include <cstdio>
#include <cstdlib>

using namespace std;

// Queue data structure
class myqueue
{
    private :
        int *array;
        int head;
        int tail;
        int length;
    public :
        myqueue(int x)
        {
            length = x;
            array = (int *) malloc(length * sizeof(int));
            head = 0;
            tail = 0;
        }
        void enqueuing(int x)
        {
            array[tail++] = x;
        }
        int dequeuing()
        {
            return array[head++];
        }
        bool isEmptyQ()
        {
            return tail == head;
        }
};

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
    int color;                 // for bfs color = 0 means White, 1 means Gray, 2 means Black
    int parent_value;          // value of parent vertex / node
    struct AdjList* parent;    // parent / predecessor for bfs
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

// Print Path from Source to the query Vertex
void printPath(struct Graph *graph, int source, int queryVertex)
{
	if(source == queryVertex)
		printf("%d ",source);
	else if (graph->array[queryVertex].parent_value == -1)
		printf("No path from source to queryVertex Exists");
	else 
		{
			printPath(graph, source, graph->array[queryVertex].parent_value);
			printf("%d ",queryVertex);
		}
}

// Prints the edges of the graph
void printGraph(struct Graph *graph, int source)
{
    struct AdjList *node;
    struct AdjListNode* temp;

    for(int i = 0; i < graph->V; ++i)
    {
        node = &graph->array[i];
        printf("\n\nOut-Degree of vertex %d = %d\nIn-Degree of vertex %d = %d\n", i, outDeg(graph, i), i, inDeg(graph, i));
        printf("distance from source of %d = %d\n", i, node->dist_from_source);
        printPath(graph, source, i);
        printf("\n");
        
        temp = node->head;
        while(temp != NULL)
        {
            printf("%d to %d with weight %d\n", i, temp->dest, temp->weight);
            temp = temp->next;
        }
    }
}
///
///   BBB   FFFFF  SSS
///   B  B  F     S
///   BBB   FFF    SSS
///   B  B  F         s
///   BBB   F      SSS
///

void bfs(struct Graph *graph, int source)
{
	struct AdjList *node, *temp1;
    struct AdjListNode *temp;

	myqueue Q(graph->V); 

	for(int i = 0; i < graph->V; ++i)
	{
		node = &graph->array[i];

		if(i != source)
		{
			node->color = 0;
		    node->dist_from_source = -1;
		    node->parent = NULL;
		    node->parent_value = -1;
		}
		else
		{
			node->color = 1;
		    node->dist_from_source = 0;
		    node->parent = NULL;
		    node->parent_value = -1;
		}
	}
	Q.enqueuing(source);
	while(!Q.isEmptyQ())
	{
		int v = Q.dequeuing();
      //  printf("%d",graph->array[1].dist_from_source);
		node = &graph->array[v];
		temp = node->head;
		
		while(temp!=NULL)
		{
			temp1 = &graph->array[temp->dest];
			
			if(temp1->color == 0)
			{
				temp1->color = 1;
		        temp1->dist_from_source = node->dist_from_source + 1;
		        temp1->parent = node;
		        temp1->parent_value = v;
		        Q.enqueuing(temp->dest);
		    }
		    temp = temp->next;
		}
		node->color = 2;
	}
	
}

 // Driver program to test above functions
int main()
{
    int numVertices, directed, src, dest, weight, root_bfs;
    
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

    printf("\nEnter the root/source for bfs : ");
    scanf("%d",&root_bfs);
   
    bfs(graph, root_bfs);
	// Print the graph
    printGraph(graph, root_bfs);

	return 0;
}