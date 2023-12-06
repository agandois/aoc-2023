#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>

#define ll long long

int n=0;
ll data[32][2];

void parse(const char* filename)
{
	FILE* fp = fopen(filename, "r");
    char lines[2][256];
    int i=0;
    while (fgets(lines[i], sizeof(lines[i]), fp))
    {
        lines[i][strlen(lines[i])] = '\0';
        n = 0;
        char* tok = strtok(lines[i], " ");
        while (tok != NULL)
        {
            if (isdigit(*tok))
            {
                data[n++][i] = atoll(tok);
            }

            tok = strtok(NULL, " ");
        }

        i++;
    }

    fclose(fp);
}

ll solve(ll data[2])
{
    ll t = data[0];
    ll d = data[1];
    // f(x) = (t-x) * x
    //      = -x^2 + tx
    // -x^2 + tx = d
    // -x^2 + tx - d = 0
    ll a = -1, b = t, c = -d;
    ll delta = pow(b, 2) - 4 * a * c;
    ll x1 = (-b-sqrt(delta))/2*a;
    ll x2 = (-b+sqrt(delta))/2*a;
    return abs(x1-x2);
}

int main(int argc, const char* argv[])
{
	const char* filename = "6";
	if (argc > 1)
		filename = argv[1];
	char buf[32];
	snprintf(buf, sizeof(buf), "inputs/%s", filename);
	
	parse(buf);
    ll p1=1;
    for (int i = 0; i < n; i++)
    {
        p1 *= solve(data[i]);
    }

    printf("part 1 : %lld\n", p1);

    ll join[2];
    char tmp1[32], tmp2[32];
    memset(tmp1, 0, sizeof(tmp1));
    memset(tmp2, 0, sizeof(tmp2));
    for (int i = 0; i < n; i++)
    {
        snprintf(tmp1+strlen(tmp1), sizeof(tmp1)-strlen(tmp1), "%lld", data[i][0]);
        snprintf(tmp2+strlen(tmp2), sizeof(tmp2)-strlen(tmp2), "%lld", data[i][1]);
    }
    join[0] = atoll(tmp1);
    join[1] = atoll(tmp2);

    printf("part 2 : %lld\n", solve(join));

	return 0;
}