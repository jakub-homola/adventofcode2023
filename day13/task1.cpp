#include "../helpers/helpers.hpp"



bool are_rows_same(const std::vector<std::string> & pat, int row1, int row2)
{
    return strcmp(pat[row1].data(), pat[row2].data()) == 0;
}

bool are_cols_same(const std::vector<std::string> & pat, int col1, int col2)
{
    int nrows = pat.size();

    for(size_t r = 0; r < nrows; r++)
    {
        if(pat[r][col1] != pat[r][col2]) return false;
    }
    return true;
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
            bool matches = true;
            while(i_prev >= 0 && i_next < end)
            {
                bool row_refl = (!transp && are_rows_same(pat, i_prev, i_next));
                bool col_refl = ( transp && are_cols_same(pat, i_prev, i_next));
                if(!row_refl && !col_refl)
                {
                    matches = false;
                    break;
                }
                i_prev--;
                i_next++;
            }
            if(matches) result += i;
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
