#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>

#define ll long long

ll memo[256][64][256];
ll solve(const char* row, int i, int group, int gsize, int cond[], int nc)
{
	if (memo[i][group][gsize] >= 0)
		return memo[i][group][gsize];

	if (i >= strlen(row))
	{
		if (group != nc)
			return 0;
		
		if (gsize > 0 && gsize != cond[nc-1])
			return 0;
		
		return 1;
	}
	
	char* next = "#.";
	ll count = 0;
	for (int k = 0; k < strlen(next); k++)
	{
		if (row[i] != '?' && next[k] != row[i])
			continue;
		
		int nextG = group;
		int nextS = gsize;
		if (next[k] == '.')
		{
			if (nextS > 0)
			{
				if (nextS != cond[group-1])
					continue;
			}
			
			nextS = 0;
		}
		else
		{
			if (gsize == 0)
			{
				nextG++;
				
				if (nextG > nc)
					continue;
			}
			
			nextS++;
			if (nextS > cond[group-1])
				continue;
		}
		
		count += solve(row, i+1, nextG, nextS, cond, nc);
	}
	
	memo[i][group][gsize] = count;
	return count;
}

void parse(const char* filename)
{
	FILE* fp = fopen(filename, "r");
	char line[256];
	ll p1 = 0;
	ll p2 = 0;
	while (fgets(line, sizeof(line), fp) != NULL)
	{
		memset(memo, -1, sizeof(memo));
		line[strlen(line)-1] = 0;
		char* row = strtok(line, " ");
		char* conditions = strtok(NULL, " ");
		int cond[32];
		int nc = 0;
		char* tok = strtok(conditions, ",");
		while (tok != NULL)
		{
			cond[nc++] = atoi(tok);
			tok = strtok(NULL, ",");
		}

		p1 += solve(row, 0, 0, 0, cond, nc);
		memset(memo, -1, sizeof(memo));
		
		char row2[256] = {0};
		int cond2[256];
		int nc2 = 0;
		for (int i = 0; i < 5; i++)
		{
			strcpy(row2+strlen(row2), row);
			row2[strlen(row2)] = '?';
			for (int j = 0; j < nc; j++)
			{
				cond2[nc2++] = cond[j];
			}
		}
		row2[strlen(row2)-1] = 0;
		p2 += solve(row2, 0, 0, 0, cond2, nc2);
	}
	
	printf("p1 : %lld\n", p1);
	printf("p2 : %lld\n", p2);
	fclose(fp);
}

int main(int argc, const char* argv[])
{
	const char* filename = "12";
	if (argc > 1)
		filename = argv[1];
	char buf[32];
	snprintf(buf, sizeof(buf), "inputs/%s", filename);
	parse(buf);
	return 0;
}