#include <cstdio>
#include <cstdlib>

using namespace std;

struct Heap
{
	int length;
	int heapSize;
	int *array;	
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
		int temp = a->array[i];
		a->array[i] = a->array[smallest];
		a->array[smallest] = temp; 
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
	int min = a->array[0];
	a->array[0] = a->array[a->heapSize - 1];

	a->heapSize--;
    min_heapify(a, 0);

	return min;

}

void heap_increase_key(struct Heap* a, int i, int key)
{
	if(key < a->array[i])
	{
		printf("new key is smaller than current key");
		return;
	}
	a->array[i] = key;
	while(i > 1 && a->array[parentof(i)] < a->array[i])
	{
		int temp = a->array[i];
		a->array[i] = a->array[parentof(i)];
		a->array[parentof(i)] = temp;

		i = parentof(i);
	}
}

void max_heap_insert(struct Heap* a, int key)
{
	a->heapSize += 1;
	a->array[a->heapSize] = -2;  // Here -2 == -ve of infinity
	heap_increase_key(a, a->heapSize, key);
}

int main()
{
	struct Heap* input;
	int n;

	printf("Enter the length of array :   ");
	scanf("%d", &n);

	input->array = (int *) malloc(n * sizeof(int));
	input->length = n;
	

	printf("Enter the elements of array :   \n");

	for(int i = 0; i < n; ++i)
	{
		scanf("%d",&input->array[i]);
	}
	
	heapSortIncreasing(input);

	for(int i = 0; i < n; ++i)
	{
		printf("%d",input->array[i]);
	}

	return 0;
}