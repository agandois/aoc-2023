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

ll reflect(bool flip)
{
	for (int col = 0; col < w-1; col++)
	{
		int cl = col, cr = col+1;
		
		bool valid = false;
		bool flipped = !flip;
		while (cl >= 0 && cr < w)
		{
			valid = true;
			for (int y = 0; y < h; y++)
			{
				if (grid[y][cl] != grid[y][cr])
				{
					if (flipped)
					{
						valid = false;
						break;
					}
					
					flipped = true;
				}
			}
			
			if (!valid)
				break;
			
			cl--;
			cr++;
		}

		if (valid && (!flip || flipped))
		{
			return col+1;
		}
	}
	
	for (int row = 0; row < h-1; row++)
	{
		int rl = row, rr = row+1;
		
		bool valid = false;
		bool flipped = !flip;
		while (rl >= 0 && rr < h)
		{
			valid = true;
			for (int x = 0; x < w; x++)
			{
				if (grid[rl][x] != grid[rr][x])
				{
					if (flipped)
					{
						valid = false;
						break;
					}
					
					flipped = true;
				}
			}
			
			if (!valid)
				break;
			
			rl--;
			rr++;
		}

		if (valid && (!flip || flipped))
		{
			return 100*(row+1);
		}
	}
}

void parse(const char* filename)
{
	FILE* fp = fopen(filename, "r");
	char line[256];
	ll p1 = 0, p2 = 0;
	
	while (fgets(line, sizeof(line), fp) != NULL)
	{
		line[strlen(line)-1] = 0;
		if (strlen(line) > 0)
		{
			for (int x = 0; x < strlen(line); x++)
			{
				w = fmax(w, x+1);
				grid[h][x] = line[x];
			}
			h++;
		}
		else
		{
			p1 += reflect(false);
			p2 += reflect(true);
			w = 0;
			h = 0;
		}
	}

	// last empty line
	p1 += reflect(false);
	p2 += reflect(true);
	
	printf("p1 : %lld\n", p1);
	printf("p2 : %lld\n", p2);
	
	fclose(fp);
}

int main(int argc, const char* argv[])
{
	const char* filename = "13";
	if (argc > 1)
		filename = argv[1];
	char buf[32];
	snprintf(buf, sizeof(buf), "inputs/%s", filename);
	parse(buf);
	return 0;
}