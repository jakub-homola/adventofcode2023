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





int main()
{
    std::vector<int> times;
    std::vector<int> distances;

    read_until_char(':');
    while(true)
    {
        int time;
        int res = scanf("%d", &time);
        if(res != 1) break;
        times.push_back(time);
    }
    read_until_char(':');
    while(true)
    {
        int distance;
        int res = scanf("%d", &distance);
        if(res != 1) break;
        distances.push_back(distance);
    }

    int result = 1;
    for(size_t i = 0; i < times.size(); i++)
    {
        double t = times[i];
        double d = distances[i];

        double D = t * t - 4 * d;
        int nways_to_win;
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

            int p1i = (int)(p1 + 1e-6) + 1;
            int p2i = (int)(p2 - 1e-6);

            nways_to_win = p2i - p1i + 1;
        }
        result *= nways_to_win;
    }

    printf("%d\n", result);

    return 0;
}
