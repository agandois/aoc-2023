#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

void part1(const char* filename)
{
	FILE* fp = fopen(filename, "r");

	char line[256];
	int sum = 0;
	int r=12,g=13,b=14;

	while (fgets(line, sizeof(line), fp) != NULL)
	{
		bool possible=true;
		char *pos1, *pos2;
		int id = atoi(strtok(line, ":")+5);
		char* data = strtok(NULL, ":");
		char* set = strtok_r(data, ";", &pos1);
		while (set != NULL)
		{
			char* cube = strtok_r(set, ",", &pos2);
			while (cube != NULL)
			{
				int value;
				char color[32];
				sscanf(cube, "%d %s", &value, color);
				
				if ((*color == 'r' && value > r) ||
					(*color == 'g' && value > g) || 
					(*color == 'b' && value > b))
				{
					possible = false;
					break;
				}
				
				cube = strtok_r(NULL, ",", &pos2);
			}
			
			set = strtok_r(NULL, ";", &pos1);
			
			if (!possible)
				break;
		}
		
		if (possible)
			sum += id;
	}

	fclose(fp);
	printf("Part 1 : %d\n", sum);
}

void part2(const char* filename)
{
	FILE* fp = fopen(filename, "r");

	char line[256];
	int sum = 0;

	while (fgets(line, sizeof(line), fp) != NULL)
	{
		int mr=0,mg=0,mb=0;
		char *pos1, *pos2;
		int id = atoi(strtok(line, ":")+5);
		char* data = strtok(NULL, ":");
		char* set = strtok_r(data, ";", &pos1);
		while (set != NULL)
		{
			char* cube = strtok_r(set, ",", &pos2);
			while (cube != NULL)
			{
				int value;
				char color[32];
				sscanf(cube, "%d %s", &value, color);
				
				if (*color == 'r')
					mr = fmax(mr, value);
				
				if (*color == 'g')
					mg = fmax(mg, value);
				
				if (*color == 'b')
					mb = fmax(mb, value);
				
				cube = strtok_r(NULL, ",", &pos2);
			}
			
			set = strtok_r(NULL, ";", &pos1);
		}
		
		sum += mr * mg * mb;
	}

	fclose(fp);
	printf("Part 2 : %d\n", sum);
}

int main(int argc, const char* argv[])
{
	const char* filename = "2";
	if (argc > 1)
		filename = argv[1];
	char buf[32];
	snprintf(buf, sizeof(buf), "inputs/%s", filename);
	
	part1(buf);
	part2(buf);
	return 0;
}