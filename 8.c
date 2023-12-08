#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>

#define ll long long

typedef struct node node;
struct node {
	char value[3+1];
	node* left;
	node* right;
};

char seq[1024];
node *nodes[1024];
int N = 0;
node* init[32]; // p2
int Ni = 0;

node* find(char* value)
{
	for (int i = 0; i < N; i++)
	{
		if (!strcmp(nodes[i]->value, value))
			return nodes[i];
	}
	
	return NULL;
}

node* init_node(char* value)
{
	node* n = malloc(sizeof(node));
	n->left = NULL;
	n->right = NULL;
	strcpy(n->value, value);
	return n;
}

void parse(const char* filename)
{
	FILE* fp = fopen(filename, "r");
	char line[256];
	fgets(seq, sizeof(seq), fp);
	seq[strlen(seq)-1] = 0;
	
	while (fgets(line, sizeof(line), fp))
	{
		int len = strlen(line)-1;
		if (len == 0)
			continue;
		line[len--] = 0;
		char* start = strtok(line, "=");
		start[strlen(start)-1] = 0;
		char* next = strtok(NULL, "=");
		char* left = strtok(next, ",")+2;
		char* right = strtok(NULL, ",")+1;
		right[strlen(right)-1] = 0;
		node *n = find(start);
		if (n == NULL)
		{
			n = init_node(start);
			nodes[N++] = n;
		}
		
		node* nl = find(left);
		if (nl == NULL)
		{
			nl = init_node(left);
			nodes[N++] = nl;
		}

		node* nr = find(right);
		if (nr == NULL)
		{
			nr = init_node(right);
			nodes[N++] = nr;
		}
		
		n->left = nl;
		n->right = nr;

		if (start[2] == 'A') // p2
		{
			init[Ni++] = n;
		}
	}

	fclose(fp);
}

ll gcd(ll a, ll b)
{
	if (b == 0)
		return a;
	return gcd(b, a%b);
}

ll lcm(int reach[], int n)
{
	ll res = reach[0];
	
	for (int i = 1; i < n; i++)
	{
		res = (reach[i] * res) / gcd(reach[i], res);
	}
	
	return res;
}

int steps(node* start, bool p2)
{
	if (!start)
		return 0;
	
	int ins = 0;
	int res = 0;
	while (strcmp(start->value, "ZZZ"))
	{
		if (p2 && start->value[2] == 'Z')
			break;
		char dir = seq[ins];
		if (dir == 'L')
			start = start->left;
		else
			start = start->right;
			
		ins = (ins + 1) % strlen(seq);
		res++;
	}
	
	return res;
}

int main(int argc, const char* argv[])
{
	const char* filename = "8";
	if (argc > 1)
		filename = argv[1];
	char buf[32];
	snprintf(buf, sizeof(buf), "inputs/%s", filename);
	
	parse(buf);

	int p1 = steps(find("AAA"), false);
	printf("p1 : %d\n", p1);
	
	int reach[32] = {0};
	for (int i = 0; i < Ni; i++)
	{
		reach[i] = steps(init[i], true);
	}
	printf("p2 : %lld\n", lcm(reach, Ni));
	
	for (int i = 0; i < N; i++)
		free(nodes[i]);
	return 0;
}
