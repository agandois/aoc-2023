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

typedef struct {
	int x;
	int y;
	int addx;
	int addy;
} pos;

int N = 0;
pos S[512] = {0};

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
			if (grid[h][x] == '#')
			{
				S[N].x = x;
				S[N].y = h;
				N++;
			}
		}
		h++;
	}
	
	// searching for empty rows
	for (int y = 0; y < h; y++)
	{
		bool empty = true;
		for (int x = 0; x < w; x++)
		{
			if (grid[y][x] == '#')
			{
				empty = false;
				break;
			}
		}
		if (empty)
		{
			for (int i = 0; i < N; i++)
			{
				if (S[i].y > y)
					S[i].addy++;
			}
		}
	}

	// searching for empty cols
	for (int x = 0; x < w; x++)
	{
		bool empty = true;
		for (int y = 0; y < h; y++)
		{
			if (grid[y][x] == '#')
			{
				empty = false;
				break;
			}
		}
		if (empty)
		{
			for (int i = 0; i < N; i++)
			{
				if (S[i].x > x)
					S[i].addx++;
			}
		}
	}
	
	int expansion[] = {2, 10, 100, 1000000};
	for (int k = 0; k < 4; k++)
	{
		ll sum = 0;
		int mul = expansion[k] - 1;
		for (int i = 0; i < N; i++)
		{
			pos a = S[i];
			for (int j = i+1; j < N; j++)
			{
				pos b = S[j];
				sum += fabs((a.x+a.addx*mul)-(b.x+b.addx*mul)) 
					+ fabs((a.y+a.addy*mul)-(b.y+b.addy*mul));
			}
		}
		printf("%d = %lld\n", mul+1, sum);
	}
}

int main(int argc, const char* argv[])
{
	const char* filename = "10";
	if (argc > 1)
		filename = argv[1];
	char buf[32];
	snprintf(buf, sizeof(buf), "inputs/%s", filename);
	parse(buf);
	return 0;
}