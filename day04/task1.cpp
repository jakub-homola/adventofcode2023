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

    while(true)
    {
        while(fgetc(stdin) != ':')
        {
            if(feof(stdin))
            {
                printf("%d\n", result);
                return 0;
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
        int price = (nmatches == 0) ? (0) : (1u << (nmatches - 1));
        result += price;
    }

    return 0;
}
