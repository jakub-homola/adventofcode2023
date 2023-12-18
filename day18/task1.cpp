#include "../helpers/helpers.hpp"



struct digstep
{
    direction dir;
    int nsteps;
};



int main()
{
    std::vector<std::string> input = read_lines();

    std::vector<digstep> digsteps(input.size());
    for(size_t i = 0; i < input.size(); i++)
    {
        char d;
        int steps;
        sscanf(input[i].c_str(), "%c %d\n", &d, &steps);
        switch(d)
        {
            case 'D': digsteps[i].dir = directions::south; break;
            case 'U': digsteps[i].dir = directions::north; break;
            case 'L': digsteps[i].dir = directions::west;  break;
            case 'R': digsteps[i].dir = directions::east;  break;
            default: digsteps[i].dir = directions::none;
        }
        digsteps[i].nsteps = steps;
    }

    int dx = 0;
    int area = 0;
    for(size_t i = 0; i < digsteps.size(); i++)
    {
        direction dir = digsteps[i].dir;
        int nsteps = digsteps[i].nsteps;
        if(dir == directions::east) dx += nsteps;
        if(dir == directions::west) dx -= nsteps;
        if(dir == directions::south) area += nsteps * dx;
        if(dir == directions::north) area -= nsteps * dx;
    }
    int area_border = 0;
    for(size_t i = 0; i < digsteps.size(); i++) area_border += digsteps[i].nsteps;
    area_border = area_border / 2 + 1;

    area += area_border;
    printf("%d\n", area);

    return 0;
}
