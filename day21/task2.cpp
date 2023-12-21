#include "../helpers/helpers.hpp"



struct coord
{
    int r;
    int c;
};



int main()
{
    int total_steps = 26501365;

    std::vector<std::string> garden;
    read_lines(garden);
    garden.insert(garden.begin(), std::string(garden[0].size(), '#'));
    garden.insert(garden.end(), std::string(garden[0].size(), '#'));
    for(size_t r = 0; r < garden.size(); r++) garden[r].insert(garden[r].begin(), '#');
    for(size_t r = 0; r < garden.size(); r++) garden[r].insert(garden[r].end(), '#');
    int nrows = garden.size();
    int ncols = garden[0].size();

    std::vector<std::vector<int>> visited_vecs(9, std::vector<int>(nrows * ncols, -1));
    std::vector<arrayview_2d<int>> visited;
    for(size_t i = 0; i < visited_vecs.size(); i++) visited.emplace_back(visited_vecs[i].data(), nrows, ncols);
    coord start_coord;
    for(int r = 0; r < nrows; r++) for(int c = 0; c < ncols; c++) if(garden[r][c] == 'S')
    {
        start_coord.r = r;
        start_coord.c = c;
        break;
    }

    for(int d = 0; d < 9; d++)
    {
        std::vector<coord> tovisit;
        coord begin_coord;
        if(d == 0) begin_coord = start_coord;
        if(d == 1) begin_coord = coord{nrows - 2, start_coord.c};
        if(d == 2) begin_coord = coord{1, start_coord.c};
        if(d == 3) begin_coord = coord{start_coord.r, ncols - 2};
        if(d == 4) begin_coord = coord{start_coord.r, 1};
        if(d == 5) begin_coord = coord{1, 1};
        if(d == 6) begin_coord = coord{1, ncols - 2};
        if(d == 7) begin_coord = coord{nrows - 2, 1};
        if(d == 8) begin_coord = coord{nrows - 2, ncols - 2};
        tovisit.push_back(begin_coord);
        visited[d][begin_coord.r][begin_coord.c] = -2;
        int nsteps = 0;
        while(!tovisit.empty())
        {
            std::vector<coord> tovisit_next;

            while(!tovisit.empty())
            {
                coord coo = tovisit.back();
                tovisit.pop_back();
                visited[d][coo.r][coo.c] = nsteps;
                for(int i = 0; i < 4; i++)
                {
                    coord nextcoo = coo;
                    if(i == 0) nextcoo.r++;
                    if(i == 1) nextcoo.r--;
                    if(i == 2) nextcoo.c++;
                    if(i == 3) nextcoo.c--;
                    if(visited[d][nextcoo.r][nextcoo.c] == -1 && garden[nextcoo.r][nextcoo.c] != '#')
                    {
                        visited[d][nextcoo.r][nextcoo.c] = -2;
                        tovisit_next.push_back(nextcoo);
                    }
                }
            }

            std::swap(tovisit_next, tovisit);
            nsteps++;
        }
    }

    std::vector<int> reachable_even(9, 0);
    std::vector<int> reachable_odd(9, 0);

    for(int d = 0; d < 9; d++)
    {
        long long reachable = 0;
        for(int r = 0; r < nrows; r++)
        {
            for(int c = 0; c < ncols; c++)
            {
                if(garden[r][c] != '#' && visited[d][r][c] >= 0)
                {
                    if(visited[d][r][c] % 2 == 0) reachable_even[d]++;
                    else reachable_odd[d]++;
                }
            }
        }
    }

    long long gardensize = nrows - 2;
    long long gardensize_half = (gardensize - 1) / 2 + 1;
    long long whole_gardens_radius = total_steps / gardensize - 1;
    long long any_garden_radius = total_steps / gardensize + 1;
    long long n_whole_gardens = whole_gardens_radius * (whole_gardens_radius + 1) / 2;
    long long n_gardens_outer_edge = whole_gardens_radius + 1;
    long long n_gardens_inner_edge = whole_gardens_radius;

    // the interior
    long long reachable_interior = 0;
    if(gardensize % 2 == 0) reachable_interior += n_whole_gardens * ((total_steps % 2 == 0) ? reachable_even[0] : reachable_odd[0]);
    else
    {
        long long odd = ((whole_gardens_radius % 2 == 0) ? whole_gardens_radius - 1 : whole_gardens_radius);
        long long even = ((whole_gardens_radius % 2 == 0) ? whole_gardens_radius : whole_gardens_radius - 1);
        long long n_whole_gardens_a = (odd + 1) * (odd / 2 + 1) / 2;
        long long n_whole_gardens_b = (even + 2) * (even / 2) / 2;
        if(total_steps % 2 == 0) reachable_interior += n_whole_gardens_a * reachable_odd[0] + n_whole_gardens_b * reachable_even[0];
        else reachable_interior += n_whole_gardens_a * reachable_even[0] + n_whole_gardens_b * reachable_odd[0];
    }

    // the ends
    long long steps_left = total_steps - whole_gardens_radius * gardensize - gardensize - gardensize_half;
    long long reachable_ends = 0;
    for(int d = 1; d <= 4; d++)
    {
        for(int r = 0; r < nrows; r++)
        {
            for(int c = 0; c < ncols; c++)
            {
                if(garden[r][c] != '#' && visited[d][r][c] >= 0 && visited[d][r][c] <= steps_left && visited[d][r][c] % 2 == steps_left % 2)
                {
                    reachable_ends++;
                }
            }
        }
    }

    // the almost ends
    steps_left = total_steps - whole_gardens_radius * gardensize - gardensize_half;
    long long reachable_almostends = 0;
    for(int d = 1; d <= 4; d++)
    {
        for(int r = 0; r < nrows; r++)
        {
            for(int c = 0; c < ncols; c++)
            {
                if(garden[r][c] != '#' && visited[d][r][c] >= 0 && visited[d][r][c] <= steps_left && visited[d][r][c] % 2 == steps_left % 2)
                {
                    reachable_almostends++;
                }
            }
        }
    }

    // the outer edges
    steps_left = total_steps - 2 * gardensize_half - whole_gardens_radius * gardensize;
    long long reachable_edgeo = 0;
    for(int d = 5; d < 9; d++)
    {
        for(int r = 0; r < nrows; r++)
        {
            for(int c = 0; c < ncols; c++)
            {
                if(garden[r][c] != '#' && visited[d][r][c] >= 0 && visited[d][r][c] <= steps_left && visited[d][r][c] % 2 == steps_left % 2)
                {
                    reachable_edgeo += n_gardens_outer_edge;
                }
            }
        }
    }

    // the inner edges
    steps_left = total_steps - 2 * gardensize_half - (whole_gardens_radius - 1) * gardensize;
    long long reachable_edgei = 0;
    for(int d = 5; d < 9; d++)
    {
        for(int r = 0; r < nrows; r++)
        {
            for(int c = 0; c < ncols; c++)
            {
                if(garden[r][c] != '#' && visited[d][r][c] >= 0 && visited[d][r][c] <= steps_left && visited[d][r][c] % 2 == steps_left % 2)
                {
                    reachable_edgei += n_gardens_inner_edge;
                }
            }
        }
    }


    // the center garden
    long long reachable_center = ((total_steps % 2 == 0) ? reachable_even[0] : reachable_odd[0]);

    long long reachable_total = 4 * reachable_interior + reachable_ends + reachable_almostends + reachable_edgeo + reachable_edgei + reachable_center;

    printf("%lld\n", reachable_total);

    return 0;
}
