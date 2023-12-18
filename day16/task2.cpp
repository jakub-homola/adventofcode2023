#include "../helpers/helpers.hpp"



struct tile
{
    char object;
    bool beams[4] = {false,false,false,false};
};

struct beam_end
{
    int r, c;
    direction d;
};

void move_beam(beam_end & be)
{
    if(be.d == directions::north) be.r--;
    if(be.d == directions::south) be.r++;
    if(be.d == directions::east) be.c++;
    if(be.d == directions::west) be.c--;
}

void print_grid(const arrayview_2d<tile> & grid)
{
    for(int r = 0; r < grid.nrows; r++)
    {
        for(int c = 0; c < grid.ncols; c++)
        {
            bool printed = false;
            #pragma unroll
            for(int i = 0; i < 4; i++)
            {
                if(grid[r][c].beams[i])
                {
                    printf("#");
                    printed = true;
                    break;
                }
            }
            if(!printed) printf("%c", grid[r][c].object);
        }
        printf("\n");
    }
}

void simulate_beams(arrayview_2d<tile> & grid, const std::vector<beam_end> & start_beamsends)
{
    std::vector<beam_end> beamsends(start_beamsends);
    
    for(int r = 0; r < grid.nrows; r++)
    {
        for(int c = 0; c < grid.ncols; c++)
        {
            for(int i = 0; i < 4; i++) grid[r][c].beams[i] = false;
        }
    }
    for(size_t i = 0; i < start_beamsends.size(); i++)
    {
        const beam_end & be = start_beamsends[i];
        grid[be.r][be.c].beams[be.d] = true;
    }

    while(!beamsends.empty())
    {
        beam_end be = beamsends.back();
        beamsends.pop_back();
        tile & t = grid[be.r][be.c];

        // print_grid(grid);
        // printf("\n");

        if(t.object == '.')
        {
            move_beam(be);
            if(!grid[be.r][be.c].beams[be.d])
            {
                grid[be.r][be.c].beams[be.d] = true;
                beamsends.push_back(be);
            }
        }
        if(t.object == '\\')
        {
            if(be.d == directions::north) be.d = directions::west;
            else if(be.d == directions::west ) be.d = directions::north;
            else if(be.d == directions::south) be.d = directions::east;
            else if(be.d == directions::east ) be.d = directions::south;
            move_beam(be);
            if(!grid[be.r][be.c].beams[be.d])
            {
                grid[be.r][be.c].beams[be.d] = true;
                beamsends.push_back(be);
            }
        }
        if(t.object == '/')
        {
            if(be.d == directions::north) be.d = directions::east;
            else if(be.d == directions::east ) be.d = directions::north;
            else if(be.d == directions::south) be.d = directions::west;
            else if(be.d == directions::west ) be.d = directions::south;
            move_beam(be);
            if(!grid[be.r][be.c].beams[be.d])
            {
                grid[be.r][be.c].beams[be.d] = true;
                beamsends.push_back(be);
            }
        }
        if(t.object == '-')
        {
            if(be.d == directions::east || be.d == directions::west)
            {
                move_beam(be);
                if(!grid[be.r][be.c].beams[be.d])
                {
                    grid[be.r][be.c].beams[be.d] = true;
                    beamsends.push_back(be);
                }
            }
            if(be.d == directions::north || be.d == directions::south)
            {
                beam_end be1 = be, be2 = be;
                be1.d = directions::east;
                be2.d = directions::west;
                move_beam(be1);
                move_beam(be2);
                if(!grid[be1.r][be1.c].beams[be1.d])
                {
                    grid[be1.r][be1.c].beams[be1.d] = true;
                    beamsends.push_back(be1);
                }
                if(!grid[be2.r][be2.c].beams[be2.d])
                {
                    grid[be2.r][be2.c].beams[be2.d] = true;
                    beamsends.push_back(be2);
                }
            }
        }
        if(t.object == '|')
        {
            if(be.d == directions::north || be.d == directions::south)
            {
                move_beam(be);
                if(!grid[be.r][be.c].beams[be.d])
                {
                    grid[be.r][be.c].beams[be.d] = true;
                    beamsends.push_back(be);
                }
            }
            if(be.d == directions::east || be.d == directions::west)
            {
                beam_end be1 = be, be2 = be;
                be1.d = directions::north;
                be2.d = directions::south;
                move_beam(be1);
                move_beam(be2);
                if(!grid[be1.r][be1.c].beams[be1.d])
                {
                    grid[be1.r][be1.c].beams[be1.d] = true;
                    beamsends.push_back(be1);
                }
                if(!grid[be2.r][be2.c].beams[be2.d])
                {
                    grid[be2.r][be2.c].beams[be2.d] = true;
                    beamsends.push_back(be2);
                }
            }
        }
    }
}

int get_n_energized(const arrayview_2d<tile> & grid)
{
    int n_energized = 0;
    for(int r = 1; r < grid.nrows-1; r++)
    {
        for(int c = 1; c < grid.ncols-1; c++)
        {
            #pragma unroll
            for(int i = 0; i < 4; i++)
            {
                if(grid[r][c].beams[i])
                {
                    n_energized++;
                    break;
                }
            }
        }
    }
    return n_energized;
}



int main()
{
    std::vector<std::string> grid_input = read_lines();
    int nrows = grid_input.size() + 2;
    int ncols = grid_input[0].size() + 2;
    std::vector<tile> grid_data(nrows * ncols);
    arrayview_2d<tile> grid(grid_data.data(), nrows, ncols);

    for(int r = 1; r < nrows-1; r++)
    {
        for(int c = 1; c < ncols-1; c++)
        {
            grid[r][c].object = grid_input[r-1][c-1];
        }
    }
    for(int c = 0; c < ncols; c++) grid[0][c].object = 'X';
    for(int c = 0; c < ncols; c++) grid[nrows-1][c].object = 'X';
    for(int r = 0; r < nrows; r++) grid[r][0].object = 'X';
    for(int r = 0; r < nrows; r++) grid[r][ncols-1].object = 'X';

    int max_energized = 0;
    for(int i = 0; i < 2 * (nrows + ncols) - 4; i++)
    {
        beam_end be;
        int in = i, is = i - ncols, iw = i - 2 * ncols, ie = i - 2 * ncols - (nrows - 2);
        if(in >= 0 && in < ncols) { be.r = 0; be.c = in; be.d = directions::south; }
        if(is >= 0 && is < ncols) { be.r = nrows-1; be.c = is; be.d = directions::north; }
        if(iw >= 0 && iw < (nrows-2)) { be.r = iw + 1; be.c = 0; be.d = directions::east; }
        if(ie >= 0 && ie < (nrows-2)) { be.r = ie + 1; be.c = ncols-1; be.d = directions::west; }
        move_beam(be);

        simulate_beams(grid, {be});

        int n_energized = get_n_energized(grid);
        max_energized = std::max(max_energized, n_energized);
    }
    printf("%d\n", max_energized);

    return 0;
}
