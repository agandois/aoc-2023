#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>

void run(const char* filename)
{
	FILE* fp = fopen(filename, "r");

	char line[256];
	int p1 = 0, p2 = 0;

	char grid[256][256];
	memset(grid, '.', sizeof(grid));

	int w=0,h=0;
	while (fgets(line, sizeof(line), fp) != NULL)
	{
		for (int x = 0; x < strlen(line) - 1; x++)
		{
			w = fmax(w, x+1);
			grid[h][x] = line[x];
		}
		h++;
	}
	
	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			char c = grid[y][x];
			if (c == '.')
				continue;
			
			if (isdigit(c))
				continue;
			
			// p2
			int gr = 1;
			int gc = 0;
			
			for (int ny = y-1; ny <= y+1; ny++)
			{
				for (int nx = x-1; nx <= x+1; nx++)
				{
					if (nx == x && ny == y)
						continue;
					
					char nc = grid[ny][nx];
					
					if (!isdigit(nc))
						continue;
					
					char tmp[10];
					memset(tmp, 0, sizeof(tmp));
					int start = nx;
					while (start > 0 && isdigit(grid[ny][start-1]))
					{
						start--;
					}
					int end = start;
					while (end < w && isdigit(grid[ny][end]))
					{
						tmp[end-start] = grid[ny][end];
						end++;
					}
					nx = end;
					
					p1 += atoi(tmp);
					
					if (c == '*') // p2
					{
						gr *= atoi(tmp);
						gc++;
					}
				}
			}
				
			if (gc == 2) // p2
				p2 += gr;
		}
	}

	fclose(fp);
	printf("Part 1 : %d\n", p1);
	printf("Part 1 : %d\n", p2);
}

int main(int argc, const char* argv[])
{
	const char* filename = "3";
	if (argc > 1)
		filename = argv[1];
	char buf[32];
	snprintf(buf, sizeof(buf), "inputs/%s", filename);
	
	run(buf);
	return 0;
}