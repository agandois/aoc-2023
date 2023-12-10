#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>

typedef struct {
	int x;
	int y;
} pos;

char* tiles = "|-LJ7F";
enum { N, E, S, W };

pos directions[] = {
	{0, -1}, // N
	{1, 0}, // E
	{0, 1}, // S
	{-1, 0}, // W
};

typedef struct {
	int to[2];
	int from[2];
} pipe_conn;

pipe_conn conn[] = {
	{{N, S},{N, S}},
	{{E, W},{E, W}},
	{{N, E},{S, W}},
	{{N, W},{S, E}},
	{{S, W},{N, E}},
	{{S, E},{N, W}},
};

bool compat(int p1, int dir, int p2)
{
	for (int i = 0; i < 2; i++)
		if (conn[p2].from[i] == dir)
			return true;
	return false;
}

char grid[256][256];
int w = 0, h = 0;
pos start;

int tileIndex(char c)
{
	return strchr(tiles, c) - tiles;
}

void parse(const char* filename)
{
	FILE* fp = fopen(filename, "r");
	char line[256];
	while (fgets(line, sizeof(line), fp) != NULL)
	{
		for (int x = 0; x < strlen(line) - 1; x++)
		{
			w = fmax(w, x+1);
			grid[h][x] = line[x];
			if (grid[h][x] == 'S')
			{
				start.x = x;
				start.y = h;
			}
		}
		h++;
	}
}

int next(pos current, pos new[])
{
	char c = grid[current.y][current.x];
	int index = tileIndex(c);
	int count = 0;
	for (int d = 0; d < 2; d++)
	{
		int dir = conn[index].to[d];
		pos next;
		next.x = current.x + directions[dir].x;
		next.y = current.y + directions[dir].y;
		
		if (next.x < 0 || next.x >= w || next.y < 0 || next.y >= h)
			continue;
		
		char c2 = grid[next.y][next.x];
		if (c2 == '.')
			continue;
		
		int index2 = tileIndex(c2);
		if (compat(index, dir, index2))
			new[count++] = next;
	}

	return count;
}

bool find(pos p, pos arr[], int len)
{
	for (int i = 0; i < len; i++)
		if (arr[i].x == p.x && arr[i].y == p.y)
			return true;
	return false;
}

pos pop(pos arr[], int len)
{
	pos first = arr[0];
	for (int i = 1; i < len; i++)
	{
		arr[i-1] = arr[i];
	}
	return first;
}

int bfs(pos start, pos path[])
{
	pos Q[32768];
	pos V[32768];
	int n = 0;
	int nv = 0;
	Q[n++] = start;
	V[nv++] = start;
	int np = 0;
	
	while (n > 0)
	{
		pos current = pop(Q, n);
		n--;
		path[np++] = current;
		
		pos neigh[8];
		int nn = next(current, neigh);
		for (int i = 0; i < nn; i++)
		{
			if (!find(neigh[i], V, nv))
			{
				Q[n++] = neigh[i];
				V[nv++] = neigh[i];
			}
		}
	}
	
	return np;
}

int main(int argc, const char* argv[])
{
	const char* filename = "10";
	if (argc > 1)
		filename = argv[1];
	char buf[32];
	snprintf(buf, sizeof(buf), "inputs/%s", filename);
	parse(buf);
	
	for (int i = 0; i < strlen(tiles); i++)
	{
		char c = tiles[i];
		grid[start.y][start.x] = c;
		pos neigh[8];
		int nn = next(start, neigh);
		if (nn == 2)
			break;
	}
	
	pos path[32768];
	int len = bfs(start, path);
	int p1 = len/2;
	printf("p1 : %d\n", p1);

	int p2 = 0;
	for (int y = 0; y < h; y++)
	{
		bool in = false;
		/*
		 * FFFTTTTTTTFFFFTTTFFTFFTT
		 * ...|..F---J..L7..|.||.|.
		 *     ^^         ^^      ^
		 */
		for (int x = 0; x < w; x++)
		{
			bool isPath = false;
			char cur;
			
			for (int i = 0; i < len; i++)
			{
				pos pp = path[i];
				if (pp.x == x && pp.y == y)
				{
					isPath = true;
					char c = grid[y][x];
					if (c == '|')
						in = !in;
					else if (c == 'F' || c == 'L')
						cur = c;
					else if (c == '7' && cur == 'L')
						in = !in;
					else if (c == 'J' && cur == 'F')
						in = !in;
				}
			}
			
			if (!isPath && in)
				p2++;
		}
	}
	
	printf("p2 : %d\n", p2);
	
	return 0;
}
