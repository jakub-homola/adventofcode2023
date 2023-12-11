#pragma once

#include <cstdio>
#include <vector>
#include <cstring>



std::vector<std::vector<char>> read_lines()
{
    std::vector<std::vector<char>> ret;
    constexpr size_t max_line_len = 100000;
    char line[max_line_len];
    while(true)
    {
        char * s = fgets(line, max_line_len, stdin);
        if(feof(stdin)) break;
        size_t len = strlen(line);
        line[len-1] = '\0';
        std::vector<char> linevec(len+1);
        strcpy(linevec.data(), line);
        ret.push_back(std::move(linevec));
    }
    return ret;
}



template<typename I>
std::vector<I> read_ints()
{
    std::vector<I> ret;
    while(true)
    {
        I num;
        int r;
        if constexpr(std::is_same_v<I,int>)                r = scanf("%d", &num);
        if constexpr(std::is_same_v<I,unsigned int>)       r = scanf("%u", &num);
        if constexpr(std::is_same_v<I,long long>)          r = scanf("%lld", &num);
        if constexpr(std::is_same_v<I,unsigned long long>) r = scanf("%llu", &num);
        if(feof(stdin)) break;
        ret.push_back(num);
    }
    return ret;
}



void read_until_char(char ch)
{
    while(getchar() != ch) ;
}
