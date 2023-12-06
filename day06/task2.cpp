#include <cstdio>
#include <vector>
#include <cmath>



void read_until_char(char ch, FILE * file = stdin)
{
    while(fgetc(file) != ch) ;
}

int get_distance(int race_time, int push_time, int acceleration = 1)
{
    int speed = push_time * acceleration;
    int run_time = race_time - push_time;
    int distance = run_time * speed;
    return distance;
}

void strip_whitespace(char * str)
{
    int curr = 0;
    int left = 0;

    while(str[curr] != '\0')
    {
        if(str[curr] == ' ' || str[curr] == '\n')
        {
            curr++;
        }
        else
        {
            str[left] = str[curr];
            curr++;
            left++;
        }
    }

    str[left] = '\0';
}





int main()
{
    char times[1000];
    char distances[1000];

    read_until_char(':');
    fgets(times, 1000, stdin);
    strip_whitespace(times);
    read_until_char(':');
    fgets(distances, 1000, stdin);
    strip_whitespace(distances);

    long long ti;
    long long di;
    sscanf(times, "%lld", &ti);
    sscanf(distances, "%lld", &di);
    printf("%lld %lld\n", ti, di);

    double t = ti;
    double d = di;

    double D = t * t - 4 * d;
    long long nways_to_win;
    if(D < 0)
    {
        nways_to_win = 0;
    }
    if(D == 0)
    {
        nways_to_win = 1;
    }
    if(D > 0)
    {
        double p1 = (t - std::sqrt(D)) / 2;
        double p2 = (t + std::sqrt(D)) / 2;

        long long p1i = (long long)(p1 + 1e-6) + 1;
        long long p2i = (long long)(p2 - 1e-6);

        nways_to_win = p2i - p1i + 1;
    }

    printf("%lld\n", nways_to_win);

    return 0;
}
