#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>

#define ll long long

char grid[256][256];
int w = 0, h = 0;

typedef struct pos {
	int x;
	int y;
} pos;

pos directions[] = {
	{0, -1},
	{1, 0},
	{0, 1},
	{-1, 0}
};

int next(pos curr, pos new[])
{
	int count = 0;
	
	for (int i = 0; i < 4; i++)
	{
		pos dir = directions[i];
		pos next;
		next.x = curr.x + dir.x;
		next.y = curr.y + dir.y;
		if (next.x < 0 || next.x >= w || next.y < 0 || next.y >= h)
			continue;
		
		if (grid[next.y][next.x] != '.')
			continue;
		
		new[count++] = next;
	}
	
	return count;
}

int nextInf(pos curr, pos new[])
{
	int count = 0;
	
	for (int i = 0; i < 4; i++)
	{
		pos dir = directions[i];
		pos next;
		next.x = curr.x + dir.x;
		next.y = curr.y + dir.y;
		
		pos tmp;
		tmp.x = next.x % w;
		tmp.y = next.y % h;
		
		// C negative modulo is not like Python negative modulo
		// in Python, -1 % k = k-1, whereas in C, -1 % k = -1
		// So we need to handle this case manually
		// Maybe there is a better way?
		if (tmp.x < 0)
			tmp.x += ceil((double)abs(tmp.x)/w)*w;
		if (tmp.y < 0)
			tmp.y += ceil((double)abs(tmp.y)/h)*h;
		
		if (grid[tmp.y][tmp.x] != '.')
			continue;
		
		new[count++] = next;
	}
	
	return count;
}

typedef struct node {
	pos pos;
	int depth;
} node;

bool find(node n, node arr[], int len)
{
	for (int i = 0; i < len; i++)
		if (arr[i].pos.x == n.pos.x && arr[i].pos.y == n.pos.y)
			return true;
	return false;
}

node pop(node arr[], int len)
{
	node first = arr[0];
	for (int i = 1; i < len; i++)
	{
		arr[i-1] = arr[i];
	}
	return first;
}

ll bfs(pos start, int depth, int (*next)(pos, pos[]))
{
	node init;
	init.pos = start;
	init.depth = 0;
	node Q[32768];
	node V[65535*4];
	int n = 0, nv = 0;
	Q[n++] = init;
	ll count = 0;
	
	while (n > 0)
	{
		node current = pop(Q, n);
		n--;
		
		if (current.depth > depth)
			break;
		
		if (find(current, V, nv))
			continue;
		
		V[nv++] = current;
		
		if (current.depth % 2 == depth % 2)
			count++;
		
		pos neigh[8];
		int nn = next(current.pos, neigh);
		for (int i = 0; i < nn; i++)
		{
			node next;
			next.pos = neigh[i];
			next.depth = current.depth + 1;
			Q[n++] = next;
		}
	}
	
	return count;
}

void parse(const char* filename)
{
	FILE* fp = fopen(filename, "r");
	char line[256];
	pos init;
	while (fgets(line, sizeof(line), fp) != NULL)
	{
		for (int x = 0; x < strlen(line) - 1; x++)
		{
			w = fmax(w, x+1);
			grid[h][x] = line[x];
			if (line[x] == 'S')
			{
				init.x = x;
				init.y = h;
				grid[h][x] = '.';
			}
		}
		h++;
	}
	fclose(fp);
	printf("p1 : %lld\n", bfs(init, 64, next));
	ll k = 26501365; // p2
	ll rep = k % w;
	ll n = k / w + 1;
	ll seq[3];
	for (int i = 0; i < 3; i++)
		seq[i] = bfs(init, rep+w*i, nextInf);
	
	ll secondDiff = (seq[2] - seq[1]) - (seq[1] - seq[0]);
	ll a = secondDiff/2;
	ll b = (seq[1]-seq[0])-3*a;
	ll c = seq[0]-b-a;
	ll p2 = a * pow(n, 2) + b*n + c;
	printf("p2 : %lld\n", p2);
}

int main(int argc, const char* argv[])
{
	const char* filename = "21";
	if (argc > 1)
		filename = argv[1];
	char buf[32];
	snprintf(buf, sizeof(buf), "inputs/%s", filename);
	parse(buf);
	return 0;
}
