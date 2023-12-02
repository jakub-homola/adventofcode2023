#include <cstdio>
#include <cstring>
#include <algorithm>



int starts_with_pattern(const char * str, const char * patterns[], int n_patterns)
{
    for(int p = 0; p < n_patterns; p++)
    {
        bool matches = true;
        const char * pattern = patterns[p];
        for(int i = 0; pattern[i] != '\0' && str[i] != '\0'; i++)
        {
            if(str[i] != pattern[i])
            {
                matches = false;
                break;
            }
        }
        if(matches)
        {
            return p;
        }
    }

    return -1;
}



int main()
{
    int sum = 0;
    char str[10000];

    constexpr int n_patterns = 20;
    const char * patterns[n_patterns] = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
    const char * patterns_reverse[n_patterns] = {"orez", "eno", "owt", "eerht", "ruof", "evif", "xis", "neves", "thgie", "enin", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
    const int numbers[n_patterns] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    while(true)
    {
        int read = scanf(" %s", str);
        if(feof(stdin)) break;
        int len = strlen(str);

        int first = -1, last = -1;

        for(int start = 0; start < len; start++)
        {
            int p = starts_with_pattern(str + start, patterns, n_patterns);
            if(p >= 0)
            {
                first = numbers[p];
                break;
            }
        }

        std::reverse(str, str + len);

        for(int start = 0; start < len; start++)
        {
            int p = starts_with_pattern(str + start, patterns_reverse, n_patterns);
            if(p >= 0)
            {
                last = numbers[p];
                break;
            }
        }

        sum += 10 * first + last;
    }

    printf("%d\n", sum);

    return 0;
}
