#include "../helpers/helpers.hpp"



struct digstep
{
    long long nsteps;
    direction dir;
};



int main()
{
    std::vector<std::string> input = read_lines();

    std::vector<digstep> digsteps(input.size());
    for(size_t i = 0; i < input.size(); i++)
    {
        std::string & str = input[i];
        int strstart = strchr(str.c_str(), '#') - str.c_str() + 1;
        char dirdigit = str[strstart + 5];
        str[strstart + 5] = '\0';
        digsteps[i].nsteps = strtoll(str.c_str() + strstart, nullptr, 16);
        switch(dirdigit)
        {
            case '0': digsteps[i].dir = directions::east; break;
            case '1': digsteps[i].dir = directions::south; break;
            case '2': digsteps[i].dir = directions::west; break;
            case '3': digsteps[i].dir = directions::north; break;
            default: digsteps[i].dir = directions::none;
        }
    }

    long long dx = 0;
    long long area = 0;
    for(size_t i = 0; i < digsteps.size(); i++)
    {
        direction dir = digsteps[i].dir;
        long long nsteps = digsteps[i].nsteps;
        if(dir == directions::east) dx += nsteps;
        if(dir == directions::west) dx -= nsteps;
        if(dir == directions::south) area += nsteps * dx;
        if(dir == directions::north) area -= nsteps * dx;
    }
    long long area_border = 0;
    for(size_t i = 0; i < digsteps.size(); i++) area_border += digsteps[i].nsteps;
    area_border = area_border / 2 + 1;

    area += area_border;
    printf("%lld\n", area);

    return 0;
}
