#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>

int grid[256][256];
int w = 0, h = 0;

typedef struct pos {
	int x;
	int y;
} pos;

enum { UP, RIGHT, DOWN, LEFT };
pos directions[] = {
	{0, -1},
	{1, 0},
	{0, 1},
	{-1, 0}
};

typedef struct node {
	pos pos;
	int dir;
	int len;
} node;

int neighbors(node n, node ret[], bool p2)
{
	int count = 0;
	for (int i = 0; i < 4; i++)
	{
		if (p2)
			if (n.len > 0 && i != n.dir && n.len < 4)
				continue;
		if (i == n.dir && ((n.len >= 3 && !p2) || n.len >= 10))
			continue;
		pos dir = directions[i];
		node next;
		next.pos.x = n.pos.x + dir.x;
		next.pos.y = n.pos.y + dir.y;
		
		if (next.pos.x < 0 || next.pos.x >= w || next.pos.y < 0 || next.pos.y >= h)
			continue;
		if (n.dir == LEFT && i == RIGHT)
			continue;
		if (n.dir == UP && i == DOWN)
			continue;
		if (n.dir == RIGHT && i == LEFT)
			continue;
		if (n.dir == DOWN && i == UP)
			continue;
		
		next.dir = i;
		next.len = 1;
		if (i == n.dir)
			next.len = n.len + 1;
		
		ret[count++] = next;
	}
	return count;
}

/*
 * Warning: this Dijkstra implementation is not meant to be the most efficient, but only to solve the problem for a given AOC input.
 * It is poorly written and could be improved in a lot of way : a better managment of the state space, the use of a Priority Queue to store the states to be evaluated, ...
 */
int Dijkstra(pos start, pos goal, bool p2)
{
	int D[256][256][4][11]; // well, it works...
	memset(D, 127, sizeof(D));
	node Q[32768];
	int N = 0;
	node init;
	init.pos = start;
	init.dir = 0;
	init.len = 0;
	D[start.y][start.x][0][0] = 0;
	
	Q[N++] = init;
	while (N > 0)
	{
		int best = 0;
		for (int i = 0; i < N; i++)
		{
			node current = Q[best];
			node other = Q[i];
			if (D[other.pos.y][other.pos.x][other.dir][other.len] < D[current.pos.y][current.pos.x][current.dir][current.len])
			{
				best = i;
			}
		}
		
		node current = Q[best];
		
		for (int i = best; i < N - 1; i++)
		{
			memcpy(&Q[i], &Q[i+1], sizeof(node));
		}
		N--;
		
		node neigh[4];
		int nc = neighbors(current, neigh, p2);
		for (int i = 0; i < nc; i++)
		{
			node next = neigh[i];
			int d = D[current.pos.y][current.pos.x][current.dir][current.len] + grid[next.pos.y][next.pos.x];
			if (d < D[next.pos.y][next.pos.x][next.dir][next.len])
			{
				D[next.pos.y][next.pos.x][next.dir][next.len] = d;
				bool in = false;
				for (int j = 0; j < N; j++)
				{
					if (Q[j].pos.x == next.pos.x && Q[j].pos.y == next.pos.y && Q[j].dir == next.dir && Q[j].len == next.len)
					{
						in = true;
						break;
					}
				}
				if (!in)
				{
					Q[N++] = next;
				}
			}
		}
	}
	
	int min = INT_MAX;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 1; j <= 10; j++)
		{
			min = fmin(min, D[goal.y][goal.x][i][j]);
		}
	}
	return min;
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
			grid[h][x] = line[x] - '0';
		}
		h++;
	}
	
	fclose(fp);
	
	pos start, end;
	start.x = 0;
	start.y = 0;
	end.x = w-1;
	end.y = h-1;
	
	printf("p1 : %d\n", Dijkstra(start, end, false));
	printf("p2 : %d\n", Dijkstra(start, end, true));
}

int main(int argc, const char* argv[])
{
	const char* filename = "17";
	if (argc > 1)
		filename = argv[1];
	char buf[32];
	snprintf(buf, sizeof(buf), "inputs/%s", filename);
	parse(buf);
	return 0;
}
