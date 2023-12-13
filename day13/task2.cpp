#include "../helpers/helpers.hpp"



int rows_diffs(const std::vector<std::string> & pat, int row1, int row2)
{
    int ncols = pat[0].size();
    int ndiffs = 0;
    for(size_t c = 0; c < ncols; c++)
    {
        if(pat[row1][c] != pat[row2][c]) ndiffs++;
    }
    return ndiffs;
}

int cols_diffs(const std::vector<std::string> & pat, int col1, int col2)
{
    int nrows = pat.size();
    int ndiffs = 0;
    for(size_t r = 0; r < nrows; r++)
    {
        if(pat[r][col1] != pat[r][col2]) ndiffs++;
    }
    return ndiffs;
}

int get_reflections_hash(const std::vector<std::vector<std::string>> & patterns, bool transp)
{
    int result = 0;

    for(size_t p = 0; p < patterns.size(); p++)
    {
        const std::vector<std::string> & pat = patterns[p];
        int nrows = pat.size();
        int ncols = pat[0].size();

        int end = (transp ? ncols : nrows);
        for(int i = 1; i < end; i++)
        {
            int i_prev = i-1;
            int i_next = i;
            int n_diffs = 0;
            while(i_prev >= 0 && i_next < end && n_diffs <= 1)
            {
                if( transp) n_diffs += cols_diffs(pat, i_prev, i_next);
                if(!transp) n_diffs += rows_diffs(pat, i_prev, i_next);
                i_prev--;
                i_next++;
            }
            if(n_diffs == 1)
            {
                result += i;
            }
        }
    }
    
    return result;
}



int main()
{
    std::vector<std::string> lines = read_lines();
    std::vector<std::vector<std::string>> patterns;

    size_t nextrow = 0;
    for(size_t r = 0; r <= lines.size(); r++)
    {
        if(r == lines.size() || lines[r][0] == '\0')
        {
            patterns.emplace_back();
            std::vector<std::string> & pat = patterns.back();
            pat.insert(pat.end(), lines.begin() + nextrow, lines.begin() + r);
            nextrow = r+1;
        }
    }

    int refl_rows = get_reflections_hash(patterns, false);
    int refl_cols = get_reflections_hash(patterns, true);
    int result = 100 * refl_rows + refl_cols;
    
    printf("%d\n", result);

    return 0;
}
