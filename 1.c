#include "stdio.h"
#include "stdlib.h"
#include "string.h"

const char* file = "inputs/1";

void part1()
{
    FILE* fp = fopen(file, "r");

    char word[256];
    int sum = 0;

    while (fscanf(fp, "%s", word) != EOF)
    {
        char pair[3] = {};
        
        for (int i = 0; i < strlen(word); i++)
        {
            char c = word[i];
            if (c >= '0' && c <= '9')
            {
                if (!pair[0])
                {
                    pair[0] = c;
                }

                pair[1] = c;
            }
        }

        sum += atoi(pair);
    }

    fclose(fp);

    printf("Part 1 : %d\n", sum);
}

void part2()
{
    const char* digits[] = {
        NULL, "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"
    };

    FILE* fp = fopen(file, "r");

    char word[256];
    int sum = 0;

    while (fscanf(fp, "%s", word) != EOF)
    {
        char pair[3] = {};
        for (int i = 0; i < strlen(word); i++)
        {
            char c = word[i];
            if (c >= '0' && c <= '9')
            {
                if (!pair[0])
                {
                    pair[0] = c;
                }

                pair[1] = c;
            }
            else
            {
                for (int d = 1; d < 10; d++)
                {
                    char substr[10];
                    snprintf(substr, strlen(digits[d])+1, "%s", word+i);
                    if (!strcmp(substr, digits[d]))
                    {
                        if (!pair[0])
                        {
                            pair[0] = d + '0';
                        }

                        pair[1] = d + '0';
                        break;
                    }
                }
            }
        }

        sum += atoi(pair);
    }

    fclose(fp);

    printf("Part 2 : %d\n", sum);
}

int main()
{

    part1();
    part2();
    return 0;
}