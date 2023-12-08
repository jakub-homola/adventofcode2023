#include <cstdio>
#include <vector>



void read_until_char(char ch, FILE * file = stdin)
{
    while(fgetc(file) != ch) ;
}

int str_to_num(const char * str)
{
    int num = 0;
    for(const char * c = str; *c != '\0'; c++)
    {
        num = 26 * num + *c - 'A';
    }
    return num;
}

bool is_bigletter(char c)
{
    return c >= 'A' && c <= 'Z';
}

int red_bigletter_str()
{
    char c;
    char str[10];
    while(true)
    {
        c = fgetc(stdin);
        if(feof(stdin)) return -1;
        if(is_bigletter(c))
        {
            str[0] = c;
            str[1] = fgetc(stdin);
            str[2] = fgetc(stdin);
            str[3] = '\0';
            int ret = str_to_num(str);
            // printf("%s -> %d   ", str, ret);
            return ret;
        }
    }
}

struct node
{
    int left = -1;
    int right = -1;
};



int main()
{
    constexpr int max_instr_len = 10000;
    constexpr int max_nodes = 26 * 26 * 26;
    char instructions[max_instr_len];
    std::vector<node> nodes(max_nodes);

    scanf("%s", instructions);

    while(true)
    {
        int src = red_bigletter_str();
        int left = red_bigletter_str();
        int right = red_bigletter_str();
        // printf("\n");

        if(src < 0 || left < 0 || right < 0) break;

        nodes[src].left = left;
        nodes[src].right = right;
    }

    int nsteps = 0;
    int instr_ptr = 0;
    int currpos = 0;
    int finalpos = str_to_num("ZZZ");
    while(currpos != finalpos)
    {
        // printf("steps %d instr %d pos %d\n", nsteps, instr_ptr, currpos);
        char instr = instructions[instr_ptr];
        if(instr == 'L') currpos = nodes[currpos].left;
        if(instr == 'R') currpos = nodes[currpos].right;
        nsteps++;
        instr_ptr++;
        if(instructions[instr_ptr] == '\0') instr_ptr = 0;
    }

    printf("%d\n", nsteps);

    return 0;
}
