#include <cstdio>
#include <cstdlib>

using namespace std;

// Adjacency List representation
//***********************************************************

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
	int check_in_heap;         // Checks in the heap or not
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

//********************************************


// Min-Priority que Data Structure
//******************************************
struct Heap
{
	int length;
	int heapSize;
	int *array;	                // this contains keys of the vertices i.e. key attributes
	int *handle;
};

int parentof(int i)
{
	return (i - 1) / 2;
}

int left_child(int i)
{
	return 2 * i + 1;
}

int right_child(int i)
{
	return 2 * (i + 1);
}

void max_heapify(struct Heap* a, int i)
{
	int l, r, largest;

	l = left_child(i);
	r = right_child(i);
	
	if(l < a->heapSize && a->array[l] > a->array[i])
	{
		largest = l;
	}
	else largest = i;

	if(r < a->heapSize && a->array[r] > a->array[largest])
	{
		largest = r;
	}
	if(largest != i)
	{
		int temp = a->array[i];
		a->array[i] = a->array[largest];
		a->array[largest] = temp; 
		max_heapify(a, largest);
	}
	
}

void min_heapify(struct Heap* a, int i)
{
	int l, r, smallest;

	l = left_child(i);
	r = right_child(i);
	if(l < a->heapSize && a->array[l] < a->array[i])
	{
		smallest = l;
	}
	else smallest = i;

	if(r < a->heapSize && a->array[r] < a->array[smallest])
	{
		smallest = r;
	}
	if(smallest != i)
	{
		int temp = a->array[i], temp2 = a->handle[i];
		a->array[i] = a->array[smallest];
		a->handle[i] = a->handle[smallest];
		a->array[smallest] = temp;
		a->handle[smallest] = temp2;
		min_heapify(a, smallest);
	}
	
}

void build_max_heap(struct Heap* a)
{
	a->heapSize = a->length;

	for(int i = (a->length/2) - 1; i >= 0; --i)
	{
		max_heapify(a, i);
	}
}

void build_min_heap(struct Heap* a)
{
	a->heapSize = a->length;

	for(int i = (a->length/2) - 1; i >= 0; --i)
	{
		min_heapify(a, i);
	}
}

void heapSortIncreasing(struct Heap* a)
{
	build_max_heap(a);
	for(int i = a->length - 1; i > 0; --i)
	{
		int temp = a->array[0];
		a->array[0] = a->array[i];
		a->array[i] = temp;

		a->heapSize--;

		max_heapify(a, 0);
	}
}

void heapSortDecreasing(struct Heap* a)
{
	build_min_heap(a);
	for(int i = a->length - 1; i > 0; --i)
	{
		int temp = a->array[0];
		a->array[0] = a->array[i];
		a->array[i] = temp;

		a->heapSize--;

		min_heapify(a, 0);
	}
}

int heap_maximum(struct Heap* a)
{
	return a->array[0];
}

int heap_minimum(struct Heap* a)
{
	return a->array[0];
}

int heap_extract_max(struct Heap* a)
{
	if(a->heapSize < 1)
	{
		printf("heap underflow\n");
		return -1;
	}
	int max = a->array[0];
	a->array[0] = a->array[a->heapSize - 1];

	a->heapSize--;
    max_heapify(a, 0);

	return max;

}

int heap_extract_min(struct Heap* a)
{
	if(a->heapSize < 1)
	{
		printf("heap underflow\n");
		return -1;
	}
	int min = a->array[0], tempHandle = a->handle[0];
	a->array[0] = a->array[a->heapSize - 1];
	//a->handle[0] = a->handle[a->heapSize - 1];

	a->heapSize--;
    min_heapify(a, 0);

	return tempHandle;

}

void heap_increase_key(struct Heap* a, int i, int key)
{
	if(key < a->array[i])
	{
		printf("new key is smaller than current key");
		return;
	}
	a->array[i] = key;
	while(i > 0 && a->array[parentof(i)] < a->array[i])
	{
		int temp = a->array[i];
		a->array[i] = a->array[parentof(i)];
		a->array[parentof(i)] = temp;

		i = parentof(i);
	}
}

void heap_decrease_key(struct Heap* a, int i, int key)
{
	if(key > a->array[i])
	{
		printf("new key is larger than current key");
		return;
	}
	a->array[i] = key;
	while(i > 0 && a->array[parentof(i)] > a->array[i])
	{
		int temp = a->array[i], temp2 = a->handle[i];printf("%d temp2 \n", temp2);

		a->array[i] = a->array[parentof(i)];
		a->handle[i] = a->handle[parentof(i)];
		a->array[parentof(i)] = temp;
		a->handle[parentof(i)] = temp2;

		i = parentof(i);
	}
}

void max_heap_insert(struct Heap* a, int key)
{
	a->heapSize += 1;
	a->array[a->heapSize] = -2;  // Here -2 == -ve of infinity
	heap_increase_key(a, a->heapSize, key);
}

//**************************************************

int root(int dest, struct Heap *a, struct Graph *graph)
{
	int i;
	for(i = 0; i < graph->V; ++i)
	{
		if(a->handle[i] == dest)
			break;
	}
	return i;
}

// PRIM Algorithm to find minimum spanning tree
void mst_prim(struct Graph* graph, int r)
{
	struct Heap* a = NULL;
	a = (struct Heap *) malloc(sizeof(struct Heap));
	int n = graph->V;

	a->array = (int *)malloc(n * sizeof(int));
	a->handle = (int *)malloc(n * sizeof(int));
	a->length = n;
	a->heapSize = n;

	for(int i = 0; i < graph->V; ++i)
	{
		a->array[i] = 1000;               // 1000 serves as infinity
		a->handle[i] = i;
		graph->array[i].check_in_heap = 1;
		graph->array[i].parent = NULL;
		graph->array[i].parent_value = -1;
	}
	
	heap_decrease_key(a, r, 0);

	while(a->heapSize != 0)
	{for(int i = 0; i < graph->V; ++i){printf("%d ", a->array[i]);}printf("\n");
		int u = heap_extract_min(a);printf("%d\n",u);printf("%d %d\n", a->array[0], a->handle[0]);
		graph->array[u].check_in_heap = 0;

		AdjListNode* temp = graph->array[u].head;
		while(temp != NULL)
		{
			if(graph->array[temp->dest].check_in_heap == 1 && temp->weight < a->array[temp->dest])
			{
				graph->array[temp->dest].parent = &graph->array[u];
				graph->array[temp->dest].parent_value = u;
				//a->array[temp->dest] = temp->weight;
				heap_decrease_key(a, root(temp->dest, a, graph), temp->weight);
				for(int i = 0; i < graph->V; ++i){printf("%d ", a->array[i]);}printf("\n");
					printf("%d %d\n", a->array[0], a->handle[0]);
			}
			temp = temp->next;
		}

	}
	
}

int main()
{
	int numVertices, directed, src, dest, weight, root;
    
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

    printf("\nEnter the root/source for prim-mst : ");
    scanf("%d",&root);

   mst_prim(graph, root);

   printGraph(graph, root);

    return 0;
}