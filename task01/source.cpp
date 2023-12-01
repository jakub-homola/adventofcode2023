#include <cstdio>
#include <cstring>
#include <algorithm>

int main()
{
    int sum = 0;
    char str[10000];

    while(true)
    {
        int read = scanf(" %s", str);
        if(feof(stdin)) break;
        int len = strlen(str);

        char first = *std::find_if(str, str + len, [](char c){ return c >= '0' && c <= '9'; });
        char last = *std::find_if(std::reverse_iterator<const char *>(str + len), std::reverse_iterator<const char *>(str), [](char c){ return c >= '0' && c <= '9'; });

        int first_num = first - '0';
        int last_num = last - '0';

        sum += 10 * first_num + last_num;
    }

    printf("%d\n", sum);

    return 0;
}

