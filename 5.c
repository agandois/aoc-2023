#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>

#define ll long long

typedef struct srange {
	ll dst;
	ll src;
	ll len;
} range;

ll find(ll value, range arr[], int n)
{
	for (int i = 0; i < n; i++)
	{
		ll start = arr[i].src;
		ll end = start + arr[i].len;
		
		if (value >= start && value < end)
		{
			ll index = value - start;
			return arr[i].dst + index;
		}
	}
	
	return value;
}

int subranges(range ranges[], int Nc, range arr[], int n, range ret[])
{
	int Nr = 0;
	
	while (Nc > 0)
	{
		range current = ranges[Nc-1];
		ll as = current.src;
		ll ae = current.dst;
		bool found = false;
		for (int i = 0; i < n; i++)
		{
			ll bs = arr[i].src;
			ll be = bs + arr[i].len;
			ll bd = arr[i].dst;
			
			//  as    bs    be    ae
			if (as <= bs && be <= ae)
			{
				ret[Nr].src = bd;
				ret[Nr].dst = bd + arr[i].len;
				Nr++;
				ranges[Nc-1].src = as;
				ranges[Nc-1].dst = bs-1;
				ranges[Nc].src = be;
				ranges[Nc].dst = ae-1;
				Nc++;
				found = true;
				break;
			}
			//       bs    as    ae    be
			else if (bs <= as && ae <= be)
			{
				ret[Nr].src = bd + (as-bs);
				ret[Nr].dst = bd + (ae-bs);
				Nr++;
				Nc--;
				found = true;
				break;
			}
			//       as    bs    ae    be
			else if (as <= bs && ae >= bs && ae <= be)
			{
				ret[Nr].src = bd;
				ret[Nr].dst = bd + (ae-bs);
				Nr++;
				ranges[Nc-1].src = as;
				ranges[Nc-1].dst = bs-1;
			}
			//       bs    as    be    ae
			else if (bs <= as && be >= as && be <= ae)
			{
				ret[Nr].src = bd + (as-bs);
				ret[Nr].dst = bd + (be-bs);
				Nr++;
				ranges[Nc-1].src = be;
				ranges[Nc-1].dst = ae-1;
			}
		}
		
		if (!found)
		{
			ret[Nr].src = as;
			ret[Nr].dst = ae;
			Nr++;
			Nc-=1;
		}
	}
	
	return Nr;
}

void run(const char* filename)
{
	FILE* fp = fopen(filename, "r");

	char line[256];
	ll seeds[32], N = 0;
	memset(seeds, -1, sizeof(seeds));

	int Ntf = -1;
	range tf[32][64];
	int tfc[32];

	while (fgets(line, sizeof(line), fp) != NULL)
	{
		int len = strlen(line);
		line[len--] = '\0';
		
		if (strlen(line) == 1)
		{
			Ntf++;
			continue;
		}
		
		if (!isdigit(*line) && Ntf >= 0)
			continue;
		
		if (Ntf < 0)
		{
			strtok(line, ":");
			char* seedLine = strtok(NULL, ":")+1;
			char seed[32];
			while (sscanf(seedLine, "%s", seed) == 1)
			{
				seeds[N++] = atoll(seed);
				seedLine += strlen(seed)+1;
			}
		}
		else 
		{
			range r;
			sscanf(line, "%lld %lld %lld", &r.dst, &r.src, &r.len);
			tf[Ntf][tfc[Ntf]] = r;
			tfc[Ntf]++;
		}
	}
	
	Ntf++;
	fclose(fp);
	
	ll lowest = LLONG_MAX;
	
	for (int i = 0; i < N; i++)
	{
		ll seed = seeds[i];
		for (int j = 0; j < Ntf; j++)
		{
			seed = find(seed, tf[j], tfc[j]);
		}
		lowest = fmin(lowest, seed);
	}
	
	printf("Part 1 : %lld\n", lowest);
	
	ll p2 = LLONG_MAX;
	
	for (int i = 0; i < N; i+=2)
	{
		ll start = seeds[i];
		ll end = start+seeds[i+1];
		range prev[256];
		prev[0].src = start;
		prev[0].dst = end-1;
		range next[256];
		int rangeN = 1;
		for (int j = 0; j < Ntf; j++)
		{
			rangeN = subranges(prev, rangeN, tf[j], tfc[j], next);
			for (int k = 0; k < rangeN; k++)
			{
				prev[k] = next[k];
			}
		}
		
		for (int j = 0; j < rangeN; j++)
		{
			p2 = fmin(p2, prev[j].src);
		}
	}
	
	printf("Part 2 : %lld\n", p2);
}

int main(int argc, const char* argv[])
{
	const char* filename = "5";
	if (argc > 1)
		filename = argv[1];
	char buf[32];
	snprintf(buf, sizeof(buf), "inputs/%s", filename);
	
	run(buf);
	return 0;
}