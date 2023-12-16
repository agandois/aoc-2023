#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>

#define ll long long

char grid[256][256];
bool energy[256][256] = {0};
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

bool visited[256][256][4] = {0};
void move(pos current, int dir)
{
	pos n;
	pos d = directions[dir];
	n.x = current.x + d.x;
	n.y = current.y + d.y;
	
	if (n.x < 0 || n.x > w-1 || n.y < 0 || n.y > h-1)
		return;
	
	if (visited[n.y][n.x][dir])
		return;
	visited[n.y][n.x][dir] = true;
	
	energy[n.y][n.x] = true;
	char c = grid[n.y][n.x];
	
	switch (c)
	{
		case '.':
			move(n, dir);
			break;
		case '-':
			switch (dir)
			{
				case RIGHT:
				case LEFT:
					move(n, dir);
					break;
				default:
					move(n, RIGHT);
					move(n, LEFT);
					break;
			}
			break;
		case '|':
			switch (dir)
			{
				case UP:
				case DOWN:
					move(n, dir);
					break;
				default:
					move(n, UP);
					move(n, DOWN);
					break;
			}
			break;
		case '/':
			switch (dir)
			{
				case UP:
					move(n, RIGHT);
					break;
				case RIGHT:
					move(n, UP);
					break;
				case DOWN:
					move(n, LEFT);
					break;
				case LEFT:
					move(n, DOWN);
					break;
			}
			break;
		case '\\':
			switch (dir)
			{
				case UP:
					move(n, LEFT);
					break;
				case RIGHT:
					move(n, DOWN);
					break;
				case DOWN:
					move(n, RIGHT);
					break;
				case LEFT:
					move(n, UP);
					break;
			}
			break;
	}
}

ll score()
{
	ll ret = 0;
	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			if (energy[y][x])
				ret++;
		}
	}
	return ret;
}

ll run(int x, int y, int dir)
{
	memset(energy, 0, sizeof(energy));
	memset(visited, 0, sizeof(visited));
	pos start;
	start.x = x;
	start.y = y;
	move(start, dir);
	return score();
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
		}
		h++;
	}

	ll p1 = run(-1, 0, RIGHT);
	ll p2 = INT_MIN;
	for (int y = -1; y <= h; y++)
	{
		p2 = fmax(p2, run(0, y, RIGHT));
		p2 = fmax(p2, run(w, y, LEFT));
	}
	
	for (int x = -1; x <= w; x++)
	{
		p2 = fmax(p2, run(x, 0, DOWN));
		p2 = fmax(p2, run(x, h, UP));
	}
	
	printf("p1 : %lld\n", p1);
	printf("p2 : %lld\n", p2);
	fclose(fp);
}

int main(int argc, const char* argv[])
{
	const char* filename = "16";
	if (argc > 1)
		filename = argv[1];
	char buf[32];
	snprintf(buf, sizeof(buf), "inputs/%s", filename);
	parse(buf);
	return 0;
}
