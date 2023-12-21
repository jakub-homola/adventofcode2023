#include "../helpers/helpers.hpp"



struct coord
{
    int r;
    int c;
};



int main()
{
    int total_steps = 64;

    std::vector<std::string> garden;
    read_lines(garden);
    garden.insert(garden.begin(), std::string(garden[0].size(), '#'));
    garden.insert(garden.end(), std::string(garden[0].size(), '#'));
    for(size_t r = 0; r < garden.size(); r++) garden[r].insert(garden[r].begin(), '#');
    for(size_t r = 0; r < garden.size(); r++) garden[r].insert(garden[r].end(), '#');
    int nrows = garden.size();
    int ncols = garden[0].size();

    std::vector<int> visited_vec(nrows * ncols, -1);
    arrayview_2d<int> visited(visited_vec.data(), nrows, ncols);

    std::vector<coord> tovisit;
    for(int r = 0; r < nrows; r++) for(int c = 0; c < ncols; c++) if(garden[r][c] == 'S')
    {
        tovisit.push_back({r,c});
        visited[r][c] = -2;
        break;
    }
    int nsteps = 0;
    while(!tovisit.empty())
    {
        std::vector<coord> tovisit_next;

        while(!tovisit.empty())
        {
            coord coo = tovisit.back();
            tovisit.pop_back();
            visited[coo.r][coo.c] = nsteps;
            for(int i = 0; i < 4; i++)
            {
                coord nextcoo = coo;
                if(i == 0) nextcoo.r++;
                if(i == 1) nextcoo.r--;
                if(i == 2) nextcoo.c++;
                if(i == 3) nextcoo.c--;
                if(visited[nextcoo.r][nextcoo.c] == -1 && garden[nextcoo.r][nextcoo.c] != '#')
                {
                    visited[nextcoo.r][nextcoo.c] = -2;
                    tovisit_next.push_back(nextcoo);
                }
            }
        }

        std::swap(tovisit_next, tovisit);
        nsteps++;
    }

    int reachable = 0;
    for(int r = 0; r < nrows; r++)
    {
        for(int c = 0; c < ncols; c++)
        {
            if(garden[r][c] != '#' && visited[r][c] <= total_steps && visited[r][c] % 2 == total_steps % 2)
            {
                reachable++;
                // printf("O");
            }
            // else printf("%c", garden[r][c]);
        }
        // printf("\n");
    }

    printf("%d\n", reachable);

    return 0;
}
