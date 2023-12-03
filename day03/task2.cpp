#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>



bool is_symbol(char ch)
{
    return ch != '.' && (ch < '0' || ch > '9');
}

bool is_digit(char ch)
{
    return ch >= '0' && ch <= '9';
}

int char_to_num(char ch)
{
    return ch - '0';
}

void print_grid(std::vector<std::vector<char>> & grid)
{
    for(size_t r = 0; r < grid.size(); r++)
    {
        std::vector<char> & line = grid[r];
        for(size_t c = 0; c < line.size(); c++) printf("%c", line[c]);
        printf("\n");
    }
}

void print_grid(std::vector<std::vector<int>> & grid)
{
    for(size_t r = 0; r < grid.size(); r++)
    {
        std::vector<int> & line = grid[r];
        for(size_t c = 0; c < line.size(); c++) printf("%3d ", line[c]);
        printf("\n");
    }
}



int main()
{
    int result = 0;
    int maxcols = 1000;
    std::vector<std::vector<char>> grid;
    std::vector<std::vector<int>> grid_nums;
    std::vector<std::vector<int>> grid_pids;

    grid.resize(1); grid.back().resize(maxcols);
    for(int l = 0; ; l++)
    {
        grid.resize(grid.size() + 1);
        grid.back().resize(maxcols);
        grid.back()[0] = '.';
        scanf(" %s", grid.back().data() + 1);
        grid.back()[strlen(grid.back().data())] = '.';
        if(feof(stdin)) break;
    }
    
    int nrows = (int)grid.size();
    int ncols = strlen(grid[1].data());
    memset(grid.front().data(), '.', ncols);
    memset(grid.back().data(), '.', ncols);

    grid_nums.resize(nrows);
    for(int r = 0; r < nrows; r++) grid_nums[r].resize(ncols, -1);
    grid_pids.resize(nrows);
    for(int r = 0; r < nrows; r++) grid_pids[r].resize(ncols, -1);

    int curr_pid = 1;
    for(size_t r = 0; r < nrows; r++)
    {
        for(int c = 0; c < ncols; c++)
        {
            int cr = r;
            int cc = c;

            if(is_digit(grid[cr][cc]))
            {
                int num = 0;
                while(is_digit(grid[cr][cc]))
                {
                    num = 10 * num + char_to_num(grid[cr][cc]);
                    cc++;
                }
                cc--;
                while(is_digit(grid[cr][cc]))
                {
                    grid_nums[cr][cc] = num;
                    grid_pids[cr][cc] = curr_pid;
                    cc--;
                }
                cc++;
                while(is_digit(grid[cr][cc]))
                {
                    grid[cr][cc] = '.';
                    cc++;
                }
                curr_pid++;
            }
        }
    }
    
    for(size_t r = 0; r < nrows; r++)
    {
        for(int c = 0; c < ncols; c++)
        {
            if(grid[r][c] == '*')
            {
                std::vector<int> adj_pids;
                std::vector<int> adj_rs;
                std::vector<int> adj_cs;
                for(int dr = -1; dr <= 1; dr++)
                {
                    for(int dc = -1; dc <= 1; dc++)
                    {
                        int cr = r + dr;
                        int cc = c + dc;
                        if(grid_pids[cr][cc] > 0)
                        {
                            if(std::find(adj_pids.begin(), adj_pids.end(), grid_pids[cr][cc]) == adj_pids.end())
                            {
                                adj_pids.push_back(grid_pids[cr][cc]);
                                adj_rs.push_back(cr);
                                adj_cs.push_back(cc);
                            }
                        }
                    }
                }

                if(adj_pids.size() == 2)
                {
                    int num0 = grid_nums[adj_rs[0]][adj_cs[0]];
                    int num1 = grid_nums[adj_rs[1]][adj_cs[1]];
                    result += num0 * num1;
                }
            }
        }
    }

    printf("%d\n", result);

    return 0;
}
