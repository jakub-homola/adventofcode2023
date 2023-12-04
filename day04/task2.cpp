#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>



int my_intersection_size(int * arrL, int countL, int * arrR, int countR)
{
    int count = 0;
    int idxL = 0;
    int idxR = 0;

    while(idxL < countL && idxR < countR)
    {
        if(arrL[idxL] == arrR[idxR])
        {
            count++;
            idxR++;
        }
        else if(arrL[idxL] < arrR[idxR])
        {
            idxL++;
        }
        else
        {
            idxR++;
        }
    }

    return count;
}



int main()
{
    int result = 0;
    char line[10000];
    std::vector<int> multipliers;
    multipliers.push_back(1);

    for(int i = 0; ; i++)
    {
        // printf("MMM "); for(size_t j = 0; j < multipliers.size(); j++) printf("%d ", multipliers[j]); printf("\n");
        while(fgetc(stdin) != ':')
        {
            if(feof(stdin))
            {
                goto theend;
            }
        }

        std::vector<int> winning;
        while(true)
        {
            int num;
            int r = scanf("%d", &num);
            if(r != 1) break;
            winning.push_back(num);
        }
        
        while(fgetc(stdin) != '|') ;

        std::vector<int> my;
        while(true)
        {
            int num;
            int r = scanf("%d", &num);
            if(r != 1) break;
            my.push_back(num);
        }

        std::sort(winning.begin(), winning.end());
        std::sort(my.begin(), my.end());

        int nmatches = my_intersection_size(winning.data(), (int)winning.size(), my.data(), (int)my.size());
        if(multipliers.size() < i + nmatches + 1) multipliers.resize(i + nmatches + 1, 1);
        for(int j = 0; j < nmatches; j++)
        {
            multipliers[i+j+1] += multipliers[i];
        }
        // if(multipliers[i] == 1 && nmatches == 0) break;
    }

    theend:

    for(auto it = multipliers.begin(); it != multipliers.end(); it++)
    {
        result += *it;
    }

    printf("%d\n", result);

    return 0;
}
