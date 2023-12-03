#include <cstdio>
#include <vector>
#include <cstring>



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
        printf("KKK %zu ", line.size());
        for(size_t c = 0; c < line.size(); c++) printf("%c", line[c]);
        printf("\n");
    }
}



int main()
{
    int result = 0;
    int maxcols = 1000;
    std::vector<std::vector<char>> grid;

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

    for(size_t r = 0; r < nrows; r++)
    {
        for(int c = 0; c < ncols; c++)
        {
            if(is_symbol(grid[r][c]))
            {
                for(int dr = -1; dr <= 1; dr++)
                {
                    for(int dc = -1; dc <= 1; dc++)
                    {
                        int cr = r + dr;
                        int cc = c + dc;

                        if(is_digit(grid[cr][cc]))
                        {
                            while(is_digit(grid[cr][cc-1])) cc--;
                            int num = 0;
                            while(is_digit(grid[cr][cc]))
                            {
                                num = 10 * num + char_to_num(grid[cr][cc]);
                                grid[cr][cc] = '.';
                                cc++;
                            }
                            result += num;
                        }
                    }
                }
            }
        }
    }

    printf("%d\n", result);

    return 0;
}
