#include <cstdio>
#include <vector>


int main()
{
    std::vector<std::vector<int>> values;

    while(true)
    {
        char line[10000];
        fgets(line, sizeof(line), stdin);
        if(feof(stdin)) break;
        values.push_back(std::vector<int>());
        std::vector<int> & vals = values.back();
        char * l = line;
        while(true)
        {
            int v;
            int res = sscanf(l, "%d", &v);
            vals.push_back(v);
            while(*l != ' ' && *l != '\0') l++;
            if(*l == '\0') break;
            l++;
        }
    }

    long long result = 0;

    for(size_t i = 0; i < values.size(); i++)
    {
        std::vector<int> & vals = values[i];
        size_t curr_n = vals.size() - 1;
        bool is_all_zero = false;
        while(!is_all_zero)
        {
            is_all_zero = true;
            for(size_t j = 0; j < curr_n; j++)
            {
                vals[j] = vals[j+1] - vals[j];
                if(vals[j] != 0) is_all_zero = false;
            }
            curr_n--;
        }

        for(size_t j = curr_n; j < vals.size(); j++) result += vals[j];
    }

    printf("%lld\n", result);

    return 0;
}
