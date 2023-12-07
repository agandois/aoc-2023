#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>

int N = 0;
typedef struct {
	char hand[5+1];
	int bid;
	int type;
	int type2; // P2
} hand;
hand hands[1024];

char* order = "AKQJT98765432";
char* order2 = "AKQT98765432J";
#define Nlabel 13

int find(const char* str, char c)
{
	char* pos = strchr(str, c);
	if (pos != NULL)
		return pos-str;
	
	return -1;
}

void count(hand h, const char* order, int ret[])
{
	for (int i = 0; i < 5; i++)
	{
		int c = find(order, h.hand[i]);
		ret[c]++;
	}
}

int type(hand h, const char* order)
{
	int counter[Nlabel] = {0};
	count(h, order, counter);
	int len[6] = {0};
	for (int i = 0; i < Nlabel; i++)
	{
		for (int j = 5; j > 0; j--)
		{
			if (counter[i] == j)
			{
				len[j]++;
			}
		}
	}
	
	if (len[5] > 0)
		return 6;
	if (len[4] > 0)
		return 5;
	if (len[3] > 0 && len[2] > 0)
		return 4;
	if (len[3] > 0)
		return 3;
	if (len[2] == 2)
		return 2;
	if (len[2] > 0)
		return 1;
	return 0;
}

int best_type(hand h, const char* order)
{
	int best = 0;
	for (int i = 0; i < Nlabel; i++)
	{
		char newj = order[i];
		hand new;
		strcpy(new.hand, h.hand);
		for (int j = 0; j < 5; j++)
		{
			if (new.hand[j] == 'J')
				new.hand[j] = newj;
			
			int score = type(new, order);
			best = fmax(best, score);
		}
	}
	
	return best;
}

void parse(const char* filename)
{
	FILE* fp = fopen(filename, "r");
	char line[256];
	while (fgets(line, sizeof(line), fp))
	{
		int len = strlen(line)-1;
		sscanf(line, "%s %d", hands[N].hand, &hands[N].bid);
		hands[N].type = type(hands[N], order);
		hands[N].type2 = best_type(hands[N], order2);
		N++;
	}

	fclose(fp);
}

int cmp(const void* a, const void* b)
{
	hand h1 = *(hand*)a;
	hand h2 = *(hand*)b;
	if (h1.type > h2.type)
		return 1;
	if (h1.type < h2.type)
		return -1;
	
	for (int i = 0; i < 5; i++)
	{
		int pos1 = find(order, h1.hand[i]);
		int pos2 = find(order, h2.hand[i]);

		if (pos1 < pos2)
			return 1;
		if (pos1 > pos2)
			return -1;
	}
	
	return 0;
}

int cmp2(const void* a, const void* b)
{
	hand h1 = *(hand*)a;
	hand h2 = *(hand*)b;
	if (h1.type2 > h2.type2)
		return 1;
	if (h1.type2 < h2.type2)
		return -1;
	
	for (int i = 0; i < 5; i++)
	{
		int pos1 = find(order2, h1.hand[i]);
		int pos2 = find(order2, h2.hand[i]);

		if (pos1 < pos2)
			return 1;
		if (pos1 > pos2)
			return -1;
	}
	
	return 0;
}

int main(int argc, const char* argv[])
{
	const char* filename = "6";
	if (argc > 1)
		filename = argv[1];
	char buf[32];
	snprintf(buf, sizeof(buf), "inputs/%s", filename);
	
	parse(buf);
	
	qsort(hands, N, sizeof(hand), cmp);
	int p1 = 0;
	for (int i = 0; i < N; i++)
	{
		p1 += (i+1) * hands[i].bid;
	}
	
	qsort(hands, N, sizeof(hand), cmp2);
	int p2 = 0;
	for (int i = 0; i < N; i++)
	{
		p2 += (i+1) * hands[i].bid;
	}
	
	printf("part 1 : %d\n", p1);
	printf("part 2 : %d\n", p2);
}