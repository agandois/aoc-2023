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
	int counts[256];
	int N=0;
	memset(counts, 0, sizeof(counts)); // p2
	int res1=0, res2=0;

	while (fgets(line, sizeof(line), fp) != NULL)
	{
		N++;
		int id = atoi(strtok(line, ":")+5);
		char* data = strtok(NULL, ":");
		
		counts[id]++; // p2
		
		int w[32], nw = 0;
		int h[32], nh = 0;
		char* win = strtok(data, "|");
		char* have = strtok(NULL, "|");
		char* tokWin = strtok(win, " ");
		while (tokWin != NULL)
		{
			w[nw++] = atoi(tokWin);
			tokWin = strtok(NULL, " ");
		}
		
		char* tokHave = strtok(have, " ");
		while (tokHave != NULL)
		{
			h[nh++] = atoi(tokHave);
			tokHave = strtok(NULL, " ");
		}

		int count = 0;
		
		for (int i = 0; i < nh; i++)
		{
			for (int j = 0; j < nw; j++)
			{
				if (h[i] == w[j])
					count++;
			}
		}
		
		if (count > 0)
			res1 += pow(2, count - 1);
		
		for (int wc = 1; wc <= count; wc++) // p2
		{
			counts[id + wc] += counts[id];
		}
	}
	
	for (int i = 0; i < N; i++)
		res2 += counts[i+1];
	
	fclose(fp);
	printf("Part 1 : %d\n", res1);
	printf("Part 2 : %d\n", res2);
}

int main(int argc, const char* argv[])
{
	const char* filename = "4";
	if (argc > 1)
		filename = argv[1];
	char buf[32];
	snprintf(buf, sizeof(buf), "inputs/%s", filename);
	
	run(buf);
	return 0;
}