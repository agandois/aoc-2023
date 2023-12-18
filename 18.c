#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>

#define ll long long

typedef struct pos {
	ll x;
	ll y;
} pos;

ll run(const char* filename, bool p2)
{
	FILE* fp = fopen(filename, "r");
	char line[256];
	pos points[1024];
	int N = 0;
	pos current;
	current.x = 0;
	current.y = 0;
	points[N++] = current;
	ll b = 0;
	while (fgets(line, sizeof(line), fp) != NULL)
	{
		line[strlen(line)-1] = 0;
		char d;
		int amt;
		char col[8];
		sscanf(line, "%c %d (%[^)])", &d, &amt, col);
		const char* dir = "RDLU";
		if (p2)
		{
			d = dir[col[6] - '0'];
			col[6] = 0;
			amt = strtol(col+1, NULL, 16);
		}
		
		switch (d)
		{
			case 'U':
			current.y -= amt;
			break;
			case 'R':
			current.x += amt;
			break;
			case 'L':
			current.x -= amt;
			break;
			case 'D':
			current.y += amt;
			break;
		}

		b += amt;
		points[N++] = current;
	}
	fclose(fp);
	
	// Computing the area of the polygon with the Shoelace formula
	// https://en.wikipedia.org/wiki/Shoelace_formula
	ll A = 0;
	
	for (int i = 0; i < N-1; i++)
	{
		A += (points[i].y + points[i+1].y) * (points[i].x - points[i+1].x);
	}
	A /= 2.0;
	
	// Then, apply Pick's theorem to get the number of points inside the polygon
	// https://en.wikipedia.org/wiki/Pick%27s_theorem
	// A = i + b/2 -1
	// <=> i = A - b/2 + 1
	
	ll i = A - b/2 + 1;
	
	// The result is the number of points inside the polygon (i) + the number of points on the boundary (b)
	return i+b;
}

int main(int argc, const char* argv[])
{
	const char* filename = "18";
	if (argc > 1)
		filename = argv[1];
	char buf[32];
	snprintf(buf, sizeof(buf), "inputs/%s", filename);
	printf("p1 : %lld\n", run(buf, false));
	printf("p2 : %lld\n", run(buf, true));
	return 0;
}
