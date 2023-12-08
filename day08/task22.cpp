#include <cstdio>
#include <vector>
#include <cstring>



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

void print_num_str(int num)
{
    char c3 = num % 26 + 'A';
    num /= 26;
    char c2 = num % 26 + 'A';
    num /= 26;
    char c1 = num % 26 + 'A';
    printf("%c%c%c", c1, c2, c3);
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

struct leap
{
    long long nsteps = -1;
    int nodeidx = -1;
};



int main()
{
    constexpr int max_instr_len = 10000;
    constexpr int max_nodes = 26 * 26 * 26;
    char instructions[max_instr_len];
    std::vector<node> nodes(max_nodes);
    std::vector<std::vector<leap>> leaps;
    std::vector<std::vector<std::vector<leap>>> sameleaps;

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

    int instr_len = strlen(instructions);
    leaps.resize(nodes.size());
    for(size_t n = 0; n < nodes.size(); n++) if(nodes[n].left >= 0 && nodes[n].right >= 0) leaps[n].resize(instr_len);
    for(size_t n = 0; n < nodes.size(); n++)
    {
        if(nodes[n].left < 0 || nodes[n].right < 0) continue;

        for(int i = 0; i < instr_len; i++)
        {
            long long nsteps = 0;
            int instr_ptr = i;
            int currpos = 0;
            while(currpos % 26 != 25)
            {
                char instr = instructions[instr_ptr];
                if(instr == 'L') currpos = nodes[currpos].left;
                if(instr == 'R') currpos = nodes[currpos].right;
                nsteps++;
                instr_ptr++;
                if(instructions[instr_ptr] == '\0') instr_ptr = 0;
            }
            leaps[n][i].nsteps = nsteps;
            leaps[n][i].nodeidx = currpos;
        }
    }
printf("AAA\n");

    sameleaps.resize(instr_len);
    #pragma omp parallel for schedule(dynamic,1)
    for(int i = 0; i < instr_len; i++)
    {
printf("BBB %d\n", i);
        std::vector<std::vector<leap>> & currsameleaps = sameleaps[i];
        currsameleaps.resize(max_nodes);
// printf("  CCC\n");
        for(size_t n = 0; n < max_nodes; n++)
        {
// printf("  DDD %zu\n", n);
            if(leaps[n].empty()) continue;
// printf("  EEE %zu\n", n);

            currsameleaps[n].resize(max_nodes);
            for(size_t m = 0; m < max_nodes; m++)
            {
                if(leaps[m].empty()) continue;

// printf("    FFF %zu\n", m);
                // start at node n and instruction i. Go for number of steps specified in the leaps[m][i]. Where do I get?
                int node = n;
                int instrptr = i;
                long long mynsteps = leaps[m][i].nsteps;
                for(long long s = 0; s < mynsteps; s++)
                {
// printf("      GGG %lld\n", s);
                    if(instructions[instrptr] == 'L') node = nodes[node].left;
                    if(instructions[instrptr] == 'R') node = nodes[node].right;
                    instrptr = (instrptr + 1) % instr_len;
// printf("        HHH\n");
                }
                
                currsameleaps[n][m].nsteps = mynsteps;
                currsameleaps[n][m].nodeidx = node;
// printf("      WWW\n");
            }
// printf("    XXX\n");
        }
// printf("  YYY\n");
    }
printf("ZZZ\n");

    long long nsteps = 0;
    int instr_ptr = 0;
    std::vector<int> currposs;
    for(size_t i = 0; i < nodes.size(); i++) if(i % 26 == 0 && nodes[i].left >= 0 && nodes[i].right >= 0) currposs.push_back(i);
    while(true)
    {
        // printf("step %d", nsteps); for(size_t i = 0; i < currposs.size(); i++) {printf(" "); print_num_str(currposs[i]);} printf("\n");

        bool is_ok = true;
        for(size_t i = 0; i < currposs.size(); i++)
        {
            if(currposs[i] % 26 != 25)
            {
                is_ok = false;
                break;
            }
        }
        if(is_ok) break;

        int maxleapidx = 0;
        for(size_t i = 1; i < currposs.size(); i++)
        {
            if(leaps[currposs[i]][instr_ptr].nsteps > leaps[maxleapidx][instr_ptr].nsteps)
            {
                maxleapidx = i;
            }
        }

        for(size_t i = 0; i < currposs.size(); i++)
        {
            currposs[i] = sameleaps[instr_ptr][currposs[i]][maxleapidx].nodeidx;
        }

        nsteps += leaps[instr_ptr][maxleapidx].nsteps;
        instr_ptr = (instr_ptr + leaps[instr_ptr][maxleapidx].nsteps) % instr_len;
    }

    printf("%lld\n", nsteps);

    return 0;
}
