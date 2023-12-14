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

void move(int x, int y, int dx, int dy)
{
	int nx = x;
	int ny = y;

	while (ny+dy >= 0 && ny+dy < h && nx+dx >= 0 && nx+dx < w)
	{
		if (grid[ny+dy][nx+dx] == 'O')
			move(nx+dx, ny+dy, dx, dy);
		if (grid[ny+dy][nx+dx] != '.')
			break;
		
		nx = nx+dx;
		ny = ny+dy;
	}
	
	grid[y][x] = '.';
	grid[ny][nx] = 'O';
}

void tilt(int dx, int dy)
{
	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			if (grid[y][x] == 'O')
				move(x, y, dx, dy);
		}
	}
}

ll load()
{
	ll res = 0;
	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			if (grid[y][x] == 'O')
				res += h - y;
		}
	}
	return res;
}

void serialize(char res[256*256])
{
	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			res[y*w+x] = grid[y][x];
		}
	}
}

char grids[256][256*256];
int steps[256] = {0};
int Ns = 0;

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
	
	tilt(0, -1);
	printf("p1 : %lld\n", load());
	
	int cyclesN = 1000000000;
	for (int i = 1; i <= cyclesN; i++)
	{
		tilt(0, -1);
		tilt(-1, 0);
		tilt(0, 1);
		tilt(1, 0);
		
		char gs[256*256] = {0};
		serialize(gs);
		bool found = false;
		for (int k = 0; k < Ns; k++)
		{
			if (!strcmp(grids[k], gs))
			{
				int prev = steps[k];
				int len = i - prev;
				int remain = (cyclesN - i) / len;
				i += (remain * len);
				found = true;
				break;
			}
		}
		
		if (!found)
		{
			strcpy(grids[Ns], gs);
			steps[Ns] = i;
			Ns++;
		}
	}

	printf("p2 : %lld\n", load());
	fclose(fp);
}

int main(int argc, const char* argv[])
{
	const char* filename = "14";
	if (argc > 1)
		filename = argv[1];
	char buf[32];
	snprintf(buf, sizeof(buf), "inputs/%s", filename);
	parse(buf);
	return 0;
}