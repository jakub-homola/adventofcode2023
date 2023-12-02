#include <cstdio>
#include <cstring>
#include <algorithm>



int main()
{
    int result = 0;
    char line[10000];

    for(int gid = 1; ; gid++)
    {
        if(fgets(line, sizeof(line), stdin) == nullptr) break;
        if(feof(stdin)) break;

        char * currstr = line;
        currstr = strchr(currstr, ':') + 1;
            
        int max_r = 0;
        int max_g = 0;
        int max_b = 0;

        while(true)
        {
            char * semicolon_ptr = strchr(currstr, ';');
            if(semicolon_ptr == nullptr) semicolon_ptr = strchr(currstr, '\n');

            while(true)
            {
                int num;
                char color;
                sscanf(currstr, "%d %c", &num, &color);
                if(color == 'r') max_r = std::max(max_r, num);
                if(color == 'g') max_g = std::max(max_g, num);
                if(color == 'b') max_b = std::max(max_b, num);
                currstr = strchr(currstr, ',');
                if(currstr == nullptr || currstr >= semicolon_ptr) break;
                currstr++;
            }

            if(*semicolon_ptr == '\n') break;

            currstr = semicolon_ptr + 1;
        }

        result += max_r * max_g * max_b;
    }

    printf("%d\n", result);

    return 0;
}
