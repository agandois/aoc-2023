#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>

int grid[512][32][32];
typedef struct brick {
	int i;
	int x,y,z;
	int dx,dy,dz;
} brick;

brick bricks[2048];
int N=0;

int fall(brick b)
{
	int count = 0;
	int z = b.z;
	int diff=0;
	
	bool stop=false;
	while (z-1 > 0)
	{
		for (int y = b.y; y <= b.dy; y++)
		{
			for (int x = b.x; x <= b.dx; x++)
			{
				if (grid[z-1][y][x] > 0)
					count += fall(bricks[grid[z-1][y][x]-1]);
				
				if (grid[z-1][y][x] != 0)
				{
					stop=true;
					break;
				}
			}
			if (stop)
				break;
		}
		if (stop)
			break;
		
		z--;
		diff++;
	}
	
	for (int x = b.x; x <= b.dx; x++)
	{
		for (int y = b.y; y <= b.dy; y++)
		{
			for (int z2 = b.z; z2 <= b.dz; z2++)
			{
				grid[z2][y][x] = 0;
				grid[z2-diff][y][x] = b.i;
			}
		}
	}
	
	bricks[b.i-1].z-=diff;
	bricks[b.i-1].dz-=diff;
	if (diff > 0)
		return 1+count;
	return count;
}

void parse(const char* filename)
{
	memset(grid, 0, sizeof(grid));
	memset(grid[0], -1, sizeof(grid[0]));
	FILE* fp = fopen(filename, "r");
	char line[256];
	while (fgets(line, sizeof(line), fp) != NULL)
	{
		line[strlen(line)-1] = 0;
		brick b;
		sscanf(line, "%d,%d,%d~%d,%d,%d", &b.x, &b.y, &b.z, &b.dx, &b.dy, &b.dz);
		b.i = N+1;
		bricks[N++] = b;
		for (int x = b.x; x <= b.dx; x++)
			for (int y = b.y; y <= b.dy; y++)
				for (int z = b.z; z <= b.dz; z++)
					grid[z][y][x] = b.i;
	}
	fclose(fp);
	
	for (int i = 0; i < N; i++)
		fall(bricks[i]);
	
	int p1 = 0;
	int p2 = 0;
	int tmp[512][32][32];
	brick tmp2[2048];
	memcpy(&tmp, &grid, sizeof(grid));
	memcpy(&tmp2, &bricks, sizeof(bricks));
	for (int i = 0; i < N; i++)
	{
		brick b = bricks[i];
		for (int x = b.x; x <= b.dx; x++)
			for (int y = b.y; y <= b.dy; y++)
				for (int z = b.z; z <= b.dz; z++)
					grid[z][y][x] = 0;

		bool safe=true;
		int chain = 0;
		for (int j = 0; j < N; j++)
		{
			if (i==j)
				continue;
			
			int count = fall(bricks[j]);
			chain += count;
			if (count > 0)
				safe=false;
		}
		
		if (safe)
			p1++;
		
		p2 += chain;
		
		memcpy(&grid, &tmp, sizeof(tmp));
		memcpy(&bricks, &tmp2, sizeof(tmp2));
	}
	printf("p1 : %d\n", p1);
	printf("p2 : %d\n", p2);
}

int main(int argc, const char* argv[])
{
	const char* filename = "22";
	if (argc > 1)
		filename = argv[1];
	char buf[32];
	snprintf(buf, sizeof(buf), "inputs/%s", filename);
	parse(buf);
	return 0;
}
