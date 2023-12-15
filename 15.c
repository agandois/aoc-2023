#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>

#define ll long long

typedef struct lens {
	char label[32];
	int focal;
} lens;

typedef struct box {
	lens lenses[32];
	int size;
} box;

box boxes[256] = {0};

int hash(const char* str)
{
	int h = 0;
	for (int i = 0; i < strlen(str); i++)
	{
		h += str[i];
		h *= 17;
		h %= 256;
	}
	return h;
}

int find_lens(box b, char label[32])
{
	for (int i = 0; i < b.size; i++)
	{
		lens l = b.lenses[i];
		if (!strcmp(l.label, label))
			return i;
	}
	
	return -1;
}

void parse(const char* filename)
{
	FILE* fp = fopen(filename, "r");
	char line[32768];
	fgets(line, sizeof(line), fp);
	line[strlen(line)-1] = 0;
	char* step = strtok(line, ",");
	ll p1 = 0;
	while (step != NULL)
	{
		p1 += hash(step);
		
		char label[32];
		int focal;
		char c;
		sscanf(step, "%[^-=]%c%d", label, &c, &focal);
		
		int boxid = hash(label);
		box b = boxes[boxid];
		int find = find_lens(b, label);
		
		if (c == '-')
		{
			if (find >= 0)
			{
				for (int i = find; i < b.size-1; i++)
					memcpy(&boxes[boxid].lenses[i], &boxes[boxid].lenses[i+1], sizeof(lens));
				boxes[boxid].size--;
			}
		}
		else
		{
			if (find >= 0)
			{
				boxes[boxid].lenses[find].focal = focal;
			}
			else
			{
				lens l;
				strcpy(l.label, label);
				l.focal = focal;
				boxes[boxid].lenses[b.size] = l;
				boxes[boxid].size++;
			}
		}
		
		step = strtok(NULL, ",");
	}
	printf("p1 : %lld\n", p1);
	
	ll p2 = 0;
	for (int i = 0; i < 256; i++)
	{
		box b = boxes[i];
		for (int j = 0; j < b.size; j++)
		{
			p2 += (1 + i) * (j+1) * b.lenses[j].focal;
		}
	}
	printf("p2 : %lld\n", p2);
	
	fclose(fp);
}

int main(int argc, const char* argv[])
{
	const char* filename = "15";
	if (argc > 1)
		filename = argv[1];
	char buf[32];
	snprintf(buf, sizeof(buf), "inputs/%s", filename);
	parse(buf);
	return 0;
}