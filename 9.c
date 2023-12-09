#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>

void parse(const char* filename)
{
	FILE* fp = fopen(filename, "r");
	char line[256];
	
	int p1 = 0;
	int p2 = 0;
	
	while (fgets(line, sizeof(line), fp))
	{
		int len = strlen(line)-1;
		line[len--] = 0;
		int hist[32][32];
		int n=0;
		char* val = strtok(line, " ");
		do {
			hist[0][n++] = atoi(val);
			val = strtok(NULL, " ");
		} while (val != NULL);
		
		int k = 1;
		bool done = false;
		while (!done)
		{
			done = true;
			for (int i = 0; i < n - k; i++)
			{
				hist[k][i] = hist[k-1][i+1] - hist[k-1][i];
				if (hist[k][i] != 0)
					done = false;
			}
			k++;
		}
		int next = 0;
		int next2 = 0;
		while (k > 0)
		{
			next += hist[k-1][n-k];
			next2 = hist[k-1][0] - next2;
			k--;
		}
		p1 += next;
		p2 += next2;
	}
	
	printf("p1 : %d\n", p1);
	printf("p2 : %d\n", p2);
	
	fclose(fp);
}

int main(int argc, const char* argv[])
{
	const char* filename = "9";
	if (argc > 1)
		filename = argv[1];
	char buf[32];
	snprintf(buf, sizeof(buf), "inputs/%s", filename);
	
	parse(buf);
	
	return 0;
}