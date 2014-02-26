#include <cstdio>
#include <cstdlib>

using namespace std;

// For Disjoint set Data Structure
struct Node
{
	int id;
	int size;	
};

// Structure for Edges of the connected graph
struct Edge
{
	int left;
	int right;
	int weight;
	int min_span_tree_edge;    // 0 if no 1 if yes
};

//
//  THIS PART IS THE IMPLEMENTATION OF THE DIS-JOINT SET DATA STRUCTURE
//
int root(int i, struct Node *node)
{
    while (i != node[i].id) 
    {
    	node[i].id = node[node[i].id].id;
    	i = node[i].id;
    }
    return i;
}

bool find(int p, int q, struct Node *node)
{
    return root(p, node) == root(q, node);
}

void unite(int p, int q, struct Node *node)
{
    int i = root(p, node);
    int j = root(q, node);
    if(i != j)
    {
        if(node[i].size < node[j].size)
        {
    	    node[i].id = j;
    	    node[j].size += node[i].size;
        }
        else
        {
        	node[j].id = i;
        	node[i].size += node[j].size;
        }
    }
}

struct Node* intialize(int n)
{
	struct Node *node = (struct Node*)malloc(n * sizeof(struct Node));

	for(int i = 0; i < n; ++i)
	{
		node[i].id = i;
		node[i].size = 1;
	}

	return node;
}

//
//  HERE IS THE END OF DATA STRUCTURE OPERATIONS
//

// Sorts the edges according to their weights
void sortEdges(int numEdges, struct Edge* edge)
{
    for(int i = 0; i < numEdges - 1; ++i)
    {
        for(int j = 0; j < numEdges - 1 - i; ++j)
        {
            if(edge[j].weight > edge[j + 1].weight)
            {
                int temp1, temp2, temp3;
                temp1 = edge[j].weight;
                temp2 = edge[j].left;
                temp3 = edge[j].right;
                edge[j].weight = edge[j + 1].weight;
                edge[j].left = edge[j + 1].left;
                edge[j].right = edge[j + 1].right;
                edge[j + 1].weight = temp1;
                edge[j + 1].left = temp2;
                edge[j + 1].right = temp3;
            }
        }
    }
}

//
//  KRUSKAL MINIMUM SPANNING TREE ALGORITHM
//
void mst_kruskal(struct Edge* edge, int numEdges, struct Node* node)
{
	for(int i = 0; i < numEdges; ++i)
	{
		if(!find(edge[i].left,edge[i].right,node))
		{
			edge[i].min_span_tree_edge = 1;

			unite(edge[i].left, edge[i].right, node);
		}
	}
}

// Prints the edges of the minimum spanning tree
void printTreeEdges(struct Edge* edge, int numEdges)
{
	int x, y, z;

	for(int i = 0; i < numEdges; ++i)
	{
		if(edge[i].min_span_tree_edge == 1)
		{
		    x = edge[i].left ;
		    y = edge[i].right ;
		    z = edge[i].weight;

		    printf("%d %d %d\n", x, y, z);
	    }
	}
}

int main()
{
	struct Node* node;
	struct Edge* edge;
	int numVertices, numEdges, x = 0, y = 0, z = 0;

	printf("Enter the number of vertices\n");
	scanf("%d", &numVertices);
    
    printf("Enter the number of edges\n");
    scanf("%d",&numEdges);

	node = (struct Node*)malloc(numVertices * sizeof(struct Node));
	edge = (struct Edge*)malloc(numEdges * sizeof(struct Edge));

	node = intialize(numVertices);

	printf("Enter edges x y weight\n");

	for(int i = 0; i < numEdges; ++i)
	{
		scanf("%d%d%d", &x, &y, &z);
			
		edge[i].left = x;
		edge[i].right = y;
		edge[i].weight = z;
		edge[i].min_span_tree_edge = 0;
	}

	sortEdges(numEdges, edge);

	mst_kruskal(edge, numEdges, node);

    printf("Tree edges and their weights are\n");

	printTreeEdges(edge, numEdges);

	return 0;
}