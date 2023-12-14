#include "../helpers/helpers.hpp"



int main()
{
    std::vector<std::string> dish = read_lines();
    int ncols = dish[0].size();
    dish.insert(dish.begin(), std::string(ncols, '#'));
    int nrows = dish.size();
    std::vector<int> slided(nrows * ncols, 0);

    for(int c = 0; c < ncols; c++)
    {
        for(int r = 0; r < nrows; r++)
        {
            if(dish[r][c] == 'O')
            {
                for(int rr = r-1; rr >= 0; rr--)
                {
                    if(dish[rr][c] == '#')
                    {
                        slided[rr * ncols + c]++;
                        break;
                    }
                }
            }
        }
    }

    int result = 0;
    for(int c = 0; c < ncols; c++)
    {
        for(int r = 0; r < nrows; r++)
        {
            if(slided[r * ncols + c] > 0)
            {
                int rr = nrows - r - 1;
                int nrocks = slided[r * ncols + c];
                int a = rr + rr - nrocks + 1;
                int b = a * nrocks / 2;
                result += b;
            }
        }
    }

    printf("%d\n", result);

    return 0;
}
