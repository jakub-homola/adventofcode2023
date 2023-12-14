#include "../helpers/helpers.hpp"



void print_dish(std::vector<std::string> & dish)
{
    for(size_t j = 0; j < dish.size(); j++)
    {
        printf("%s\n", dish[j].c_str());
    }
}

void do_slide_n(std::vector<std::string> & dish)
{
    int nrows = dish.size();
    int ncols = dish[0].length();
    for(int c = 0; c < ncols; c++)
    {
        int next_slidepos = 0;
        for(int r = 0; r < nrows; r++)
        {
            if(dish[r][c] == '#')
            {
                next_slidepos = r + 1;
            }
            if(dish[r][c] == 'O')
            {
                std::swap(dish[next_slidepos][c], dish[r][c]);
                next_slidepos++;
            }
        }
    }
}

void do_slide_s(std::vector<std::string> & dish)
{
    int nrows = dish.size();
    int ncols = dish[0].length();
    for(int c = 0; c < ncols; c++)
    {
        int next_slidepos = nrows - 1;
        for(int r = nrows - 1; r >= 0; r--)
        {
            if(dish[r][c] == '#')
            {
                next_slidepos = r - 1;
            }
            if(dish[r][c] == 'O')
            {
                std::swap(dish[next_slidepos][c], dish[r][c]);
                next_slidepos--;
            }
        }
    }
}

void do_slide_w(std::vector<std::string> & dish)
{
    int nrows = dish.size();
    int ncols = dish[0].length();
    for(int r = 0; r < nrows; r++)
    {
        int next_slidepos = 0;
        for(int c = 0; c < ncols; c++)
        {
            if(dish[r][c] == '#')
            {
                next_slidepos = c + 1;
            }
            if(dish[r][c] == 'O')
            {
                std::swap(dish[r][next_slidepos], dish[r][c]);
                next_slidepos++;
            }
        }
    }
}

void do_slide_e(std::vector<std::string> & dish)
{
    int nrows = dish.size();
    int ncols = dish[0].length();
    for(int r = 0; r < nrows; r++)
    {
        int next_slidepos = ncols - 1;
        for(int c = ncols - 1; c >= 0; c--)
        {
            if(dish[r][c] == '#')
            {
                next_slidepos = c - 1;
            }
            if(dish[r][c] == 'O')
            {
                std::swap(dish[r][next_slidepos], dish[r][c]);
                next_slidepos--;
            }
        }
    }
}

void do_slide_cycle(std::vector<std::string> & dish)
{
    do_slide_n(dish);
    do_slide_w(dish);
    do_slide_s(dish);
    do_slide_e(dish);
}

int get_weight(std::vector<std::string> & dish)
{
    int nrows = dish.size();
    int ncols = dish[0].length();
    int result = 0;
    for(int c = 0; c < ncols; c++)
    {
        for(int r = 0; r < nrows; r++)
        {
            if(dish[r][c] == 'O')
            {
                int w = nrows - r;
                result += w;
            }
        }
    }
    return result;
}


int main()
{
    std::vector<std::string> dish = read_lines();
    int nrows = dish.size();
    int ncols = dish[0].size();
    int ncycles = 1000000000;

    
    std::vector<std::vector<std::string>> dish_past;
    for(int i = 0; i < ncycles; i++)
    {
        dish_past.push_back(dish);
        do_slide_cycle(dish);
        auto it = std::find(dish_past.begin(), dish_past.end(), dish);
        if(it != dish_past.end())
        {
            int period = dish_past.end() - it;
            int rem_cycles = ncycles - i - 1;
            int whole_periods = rem_cycles / period;
            int todo_cycles = rem_cycles - whole_periods * period;
            for(int j = 0; j < todo_cycles; j++)
            {
                do_slide_cycle(dish);
            }
            // printf("last cycle %d period %d todo %d\n", i, period, todo_cycles);
            break;
        }
    }

    int result = get_weight(dish);
    printf("%d\n", result);

    return 0;
}
