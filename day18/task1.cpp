#include "../helpers/helpers.hpp"



struct direction
{
    int val;
    constexpr direction() : val(-1) {}
    constexpr direction(int v) : val{v} {}
    constexpr operator int() const { return val; }
    constexpr bool operator==(const direction & other) const { return other.val == val; }
};

struct directions
{
    constexpr static direction none = direction(0);
    constexpr static direction north = direction(1);
    constexpr static direction east  = direction(2);
    constexpr static direction south = direction(3);
    constexpr static direction west  = direction(4);
};

char dir_to_char(direction d)
{
    switch(d)
    {
        case directions::north: return 'N';
        case directions::west:  return 'W';
        case directions::south: return 'S';
        case directions::east:  return 'E';
        default: return 'O';
    }
}

direction turn_left(direction d)
{
    switch(d)
    {
        case directions::north: return directions::west;
        case directions::west:  return directions::south;
        case directions::south: return directions::east;
        case directions::east:  return directions::north;
        default: return directions::none;
    }
}

direction turn_right(direction d)
{
    switch(d)
    {
        case directions::north: return directions::east;
        case directions::east:  return directions::south;
        case directions::south: return directions::west;
        case directions::west:  return directions::north;
        default: return directions::none;
    }
}

direction turn_around(direction d)
{
    switch(d)
    {
        case directions::north: return directions::south;
        case directions::south: return directions::north;
        case directions::east:  return directions::west;
        case directions::west:  return directions::east;
        default: return directions::none;
    }
}

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
