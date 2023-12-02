#include <cstdio>
#include <cstring>
#include <algorithm>



int main()
{
    int result = 0;
    int max_r = 12;
    int max_g = 13;
    int max_b = 14;
    char line[10000];

    for(int gid = 1; ; gid++)
    {
        if(fgets(line, sizeof(line), stdin) == nullptr) break;
        if(feof(stdin)) break;

        bool is_possible = true;
        char * currstr = line;
        currstr = strchr(currstr, ':') + 1;

        while(true)
        {
            char * semicolon_ptr = strchr(currstr, ';');
            if(semicolon_ptr == nullptr) semicolon_ptr = strchr(currstr, '\n');
            
            int r = 0, g = 0, b = 0;

            while(true)
            {
                int num;
                char color;
                sscanf(currstr, "%d %c", &num, &color);
                if(color == 'r') r = std::max(r, num);
                if(color == 'g') g = std::max(g, num);
                if(color == 'b') b = std::max(b, num);
                currstr = strchr(currstr, ',');
                if(currstr == nullptr || currstr >= semicolon_ptr) break;
                currstr++;
            }

            if(r > max_r || g > max_g || b > max_b)
            {
                is_possible = false;
                break;
            }

            if(*semicolon_ptr == '\n') break;

            currstr = semicolon_ptr + 1;
        }

        if(is_possible) result += gid;
    }

    printf("%d\n", result);

    return 0;
}
