#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>

#define ll long long

const char* rates = "xmas";

typedef struct {
	int rate;
	char op;
	int value;
} cond;

typedef struct {
	cond pre;
	char conc[8];
} rule;

typedef struct {
	char name[8];
	rule rules[32];
	int n;
	char def[8];
} workflow;

typedef struct {
	int rates[4];
} part;

workflow wfs[1024];
int N=0;

part parts[1024];
int Np=0;

int find_wf(const char* name)
{
	for (int i = 0; i < N; i++)
	{
		if (!strcmp(wfs[i].name, name))
			return i;
	}
	return -1;
}

bool accept(part p, const char* wfName)
{
	if (!strcmp(wfName, "A"))
		return true;
	if (!strcmp(wfName, "R"))
		return false;
	
	int k = find_wf(wfName);
	workflow wf = wfs[k];
	bool found = false;
	char new[8];
	for (int i = 0; i < wf.n; i++)
	{
		rule r = wf.rules[i];
		int rate = r.pre.rate;
		switch (r.pre.op)
		{
			case '>':
				if (p.rates[rate] > r.pre.value)
					found = true;
			break;
			case '<':
				if (p.rates[rate] < r.pre.value)
					found = true;
			break;
		}
		
		if (found)
		{
			strcpy(new, r.conc);
			break;
		}
	}

	if (!found)
		strcpy(new, wf.def);
	
	return accept(p, new);
}

typedef struct {
	ll min;
	ll max;
} range;

typedef struct {
	range rates[4];
} combi;

ll count(const char* wfName, combi c)
{
	if (!strcmp(wfName, "A"))
	{
		ll prod = 1;
		for (int i = 0; i < strlen(rates); i++)
		{
			prod *= c.rates[i].max - c.rates[i].min + 1;
		}
		return prod;
	}
	
	if (!strcmp(wfName, "R"))
		return 0;

	int k = find_wf(wfName);
	workflow wf = wfs[k];
	ll accepted = 0;

	for (int i = 0; i < wf.n; i++)
	{
		rule r = wf.rules[i];
		combi next;
		memcpy(&next, &c, sizeof(combi));
		int rate = r.pre.rate;
		ll val = r.pre.value;
		switch (r.pre.op)
		{
			case '>':
				if (c.rates[rate].min <= val)
				{
					next.rates[rate].min = val+1;
					c.rates[rate].max = val;
					accepted += count(r.conc, next);
				}
				break;
			case '<':
				if (c.rates[rate].max >= val)
				{
					next.rates[rate].max = val-1;
					c.rates[rate].min = val;
					accepted += count(r.conc, next);
				}
				break;
		}
	}
	
	return accepted + count(wf.def, c);
}

void parse(const char* filename)
{
	FILE* fp = fopen(filename, "r");
	char line[256];
	int state=0;
	while (fgets(line, sizeof(line), fp) != NULL)
	{
		line[strlen(line)-1] = 0;
		int pos=0;
		if (strlen(line) == 0)
		{
			state++;
			continue;
		}
		if (state == 0)
		{
			workflow wf;
			wf.n = 0;
			sscanf(line+pos, "%[^{]", wf.name);
			pos += strlen(wf.name)+1;
			rule r;
			char tmpVal[8];
			char tmpRate[8];
			while (pos < strlen(line))
			{
				int m = sscanf(line+pos, "%[^><}]%c%[^:]:%[^,]", tmpRate,&r.pre.op,tmpVal,r.conc);
				if (m == 2)
				{
					strcpy(wf.def, tmpRate);
				}
				else
				{
					r.pre.rate = strchr(rates, *tmpRate) - rates;
					r.pre.value = atoi(tmpVal);
					wf.rules[wf.n++] = r;
				}
				pos += 1+1+strlen(tmpVal)+1+strlen(r.conc)+1;
			}
			wfs[N++] = wf;
		}
		else
		{
			part p;
			sscanf(line, "{x=%d,m=%d,a=%d,s=%d}", &p.rates[0], &p.rates[1], &p.rates[2], &p.rates[3]);
			parts[Np++] = p;
		}
	}
	fclose(fp);
	
	ll p1 = 0;
	for (int i = 0; i < Np; i++)
	{
		if (accept(parts[i], "in"))
		{
			for (int j = 0; j < strlen(rates); j++)
			{
				p1 += parts[i].rates[j];
			}
		}
	}
	printf("p1 : %lld\n", p1);
	combi start;
	for (int i = 0; i < strlen(rates); i++)
	{
		start.rates[i].min = 1;
		start.rates[i].max = 4000;
	}

	printf("p2 : %lld\n", count("in", start));
}

int main(int argc, const char* argv[])
{
	const char* filename = "19";
	if (argc > 1)
		filename = argv[1];
	char buf[32];
	snprintf(buf, sizeof(buf), "inputs/%s", filename);
	parse(buf);
	return 0;
}
