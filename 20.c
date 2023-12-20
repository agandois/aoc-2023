#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>

#define ll long long

ll gcd(ll a, ll b)
{
	if (b == 0)
		return a;
	return gcd(b, a%b);
}

ll lcm(int arr[], int n)
{
	ll res = arr[0];
	
	for (int i = 1; i < n; i++)
		res = (arr[i] * res) / gcd(arr[i], res);
	
	return res;
}
typedef struct module module;
struct module {
	int index;
	char type;
	char name[32];
	bool state;
	module* dest[32];
	int nd;
	module* pred[32];
	bool mem[128]; // conjunction only
	int np;
};

module* modules[128];
int N=0;


module* find_module(const char* name)
{
	for (int i = 0; i < N; i++)
		if (!strcmp(modules[i]->name, name))
			return modules[i];
	return NULL;
}

module* init_module(char name[32])
{
	module* m = malloc(sizeof(module));
	m->index = N;
	m->type = 0;
	m->state = false;
	m->nd = 0;
	m->np = 0;
	strcpy(m->name, name);
	memset(m->mem, 0, sizeof(m->mem));
	return m;
}

typedef struct pulse {
	module* from;
	module* to;
	bool state;
} pulse;

void parse(const char* filename)
{
	FILE* fp = fopen(filename, "r");
	char line[256];
	while (fgets(line, sizeof(line), fp) != NULL)
	{
		line[strlen(line)-1] = 0;
		char* name = strtok(line, "-");
		name[strlen(name)-1] = 0;
		char* dest = strtok(NULL, "-")+2;
		char type = 'b';
		if (*name == '%' || *name == '&')
		{
			type = *name;
			name++;
		}
		
		module* m = find_module(name);
		if (m == NULL)
		{
			m = init_module(name);
			modules[N++] = m;
		}
		m->type = type;
		
		char destName[32];
		int pos = 0;
		while (pos < strlen(dest))
		{
			sscanf(dest+pos, "%[^,]", destName);
			module* d = find_module(destName);
			if (d == NULL)
			{
				d = init_module(destName);
				modules[N++] = d;
			}
			
			m->dest[m->nd++] = d;
			d->pred[d->np++] = m;
			
			pos += strlen(destName)+2;
		}
	}
	
	fclose(fp);

	module* button = init_module("button");
	modules[N++] = button;
	module* broadcast = find_module("broadcaster");

	ll nh = 0, nl = 0;
	module* check[32]; // P2
	int steps[32];
	module* rx = find_module("rx");
	int nc = rx->pred[0]->np;
	for (int i = 0; i < nc; i++)
	{
		check[i] = rx->pred[0]->pred[i];
		steps[i] = 0;
	}
	int r=nc;
	
	int c=1;
	while (true)
	{
		pulse first;
		first.from = button;
		first.to = broadcast;
		first.state = false;
		
		pulse Q[128];
		int NQ = 0;
		Q[NQ++] = first;

		while (NQ > 0)
		{
			pulse curr = Q[0];
			if (c <= 1000) // P1
			{
				if (curr.state)
					nh++;
				else
					nl++;
			}
			
			for (int i = 1; i < NQ; i++)
				Q[i-1] = Q[i];
			NQ--;
			
			for (int i = 0; i < nc; i++)
			{
				if (!curr.state && !strcmp(curr.to->name, check[i]->name) && steps[i] == 0)
				{
					steps[i] = c;
					r--;
				}
			}
			
			module* o = curr.from;
			module* m = curr.to;
			switch (m->type)
			{
				case 'b':
				for (int i = 0; i < m->nd; i++)
				{
					pulse p;
					p.from = m;;
					p.state = curr.state;
					p.to = m->dest[i];
					Q[NQ++] = p;
				}
				break;
				case '%':
				if (!curr.state)
				{
					m->state = !m->state;
					for (int i = 0; i < m->nd; i++)
					{
						pulse p;
						p.from = m;
						p.state = m->state;
						p.to = m->dest[i];
						Q[NQ++] = p;
					}
				}
				break;
				case '&':
				{
					m->mem[o->index] = curr.state;
					bool next = false;
					for (int i = 0; i < m->np; i++)
					{
						if (!m->mem[m->pred[i]->index])
						{
							next = true;
							break;
						}
					}
					
					for (int i = 0; i < m->nd; i++)
					{
						pulse p;
						p.from = m;
						p.state = next;
						p.to = m->dest[i];
						Q[NQ++] = p;
					}
				}
				break;
			}
		}
		
		if (r <= 0)
			break;

		c++;
	}
	
	printf("p1 : %lld\n", nl*nh);
	printf("p2 : %lld\n", lcm(steps, nc));
	for (int i = 0; i < N; i++)
		free(modules[i]);
}

int main(int argc, const char* argv[])
{
	const char* filename = "20";
	if (argc > 1)
		filename = argv[1];
	char buf[32];
	snprintf(buf, sizeof(buf), "inputs/%s", filename);
	parse(buf);
	return 0;
}
