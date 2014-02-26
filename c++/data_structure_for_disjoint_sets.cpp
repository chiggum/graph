#include <cstdio>
#include <cstdlib>

using namespace std;

struct Node
{
	int id;
	int size;	
};

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
        if(node[i].size < node[i].size)
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

int main()
{
	struct Node* node;
	int n, x = 0, y = 0, z = 0;

	printf("Enter the number of nodes\n");
	scanf("%d", &n);

	node = (struct Node*)malloc(n * sizeof(struct Node));

	node = intialize(n);

	while(x != -1)
	{
		scanf("%d%d%d", &x, &y, &z);

		if(x != -1)
		{
			if(z == 0)
			{
				unite(x, y, node);
				printf("done\n");
			}
			if(z == 1)
			{
				if(find(x, y, node))
				{
					printf("yes\n");
				}
				else
					printf("no\n");
			}
		}
	}

	return 0;
}