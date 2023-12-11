#include <cstdio>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstring>

#include "helpers.hpp"



struct pos
{
    int r;
    int c;
    pos(){}
    pos(int r_, int c_) : r{r_}, c{c_} {}
};


int main()
{
    std::vector<std::vector<char>> map = read_lines();
    int nrows = map.size();
    int ncols = map[0].size();

    std::vector<pos> galaxies;
    for(int r = 0; r < nrows; r++)
    {
        for(int c = 0; c < ncols; c++)
        {
            if(map[r][c] == '#') galaxies.push_back(pos(r,c));
        }
    }

    std::vector<bool> row_contains_galaxy(nrows, false);
    std::vector<bool> col_contains_galaxy(ncols, false);

    for(size_t i = 0; i < galaxies.size(); i++)
    {
        row_contains_galaxy[galaxies[i].r] = true;
        col_contains_galaxy[galaxies[i].c] = true;
    }

    int nrows_new = nrows + std::count(row_contains_galaxy.begin(), row_contains_galaxy.end(), true);
    int ncols_new = ncols + std::count(col_contains_galaxy.begin(), col_contains_galaxy.end(), true);

    std::vector<int> new_row_poss(nrows);
    std::vector<int> new_col_poss(ncols);

    int emptyrows = 0;
    for(int r = 0; r < nrows; r++)
    {
        new_row_poss[r] = r + emptyrows;
        if(!row_contains_galaxy[r]) emptyrows++;
    }
    int emptycols = 0;
    for(int c = 0; c < ncols; c++)
    {
        new_col_poss[c] = c + emptycols;
        if(!col_contains_galaxy[c]) emptycols++;
    }

    long long sum = 0;
    for(size_t i = 0; i < galaxies.size(); i++)
    {
        for(size_t j = i+1; j < galaxies.size(); j++)
        {
            int dr = std::abs(new_row_poss[galaxies[i].r] - new_row_poss[galaxies[j].r]);
            int dc = std::abs(new_col_poss[galaxies[i].c] - new_col_poss[galaxies[j].c]);
            int d = dr + dc;
            sum += d;
        }
    }

    printf("%lld\n", sum);

    return 0;
}
