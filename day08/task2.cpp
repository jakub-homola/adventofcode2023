#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
#include <numeric>



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

struct cycle
{
    int start;
    int length;
    std::vector<int> finalnodeidxsincycle;
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
    int instr_len = strlen(instructions);
    
    std::vector<int> startnodeidxs;
    for(int n = 0; n < max_nodes; n++) if(nodes[n].left >= 0 && n % 26 == 0) startnodeidxs.push_back(n);

    std::vector<cycle> cycles(startnodeidxs.size());
    for(size_t sni = 0; sni < startnodeidxs.size(); sni++)
    {
        cycle & c = cycles[sni];
        std::vector<int> visited(max_nodes * instr_len, -1);
        int currnode = startnodeidxs[sni];
        int currinstr = 0;
        int iter = 0;
        while(visited[currnode * instr_len + currinstr] < 0)
        {
            visited[currnode * instr_len + currinstr] = iter;
            if(currnode % 26 == 25)
            {
                c.finalnodeidxsincycle.push_back(iter);
            }
            if(instructions[currinstr] == 'L') currnode = nodes[currnode].left;
            if(instructions[currinstr] == 'R') currnode = nodes[currnode].right;
            currinstr = (currinstr + 1) % instr_len;
            iter++;
        }
        c.start = visited[currnode * instr_len + currinstr];
        c.length = iter - c.start;
        for(size_t i = 0; i < c.finalnodeidxsincycle.size(); i++) c.finalnodeidxsincycle[i] -= c.start;
        auto newend = std::remove_if(c.finalnodeidxsincycle.begin(), c.finalnodeidxsincycle.end(), [](int idx) { return idx < 0; });
        c.finalnodeidxsincycle.erase(newend, c.finalnodeidxsincycle.end());
    }

    // for(size_t i = 0; i < cycles.size(); i++)
    // {
    //     printf("Cycle %zu: start %d length %d finals", i, cycles[i].start, cycles[i].length);
    //     for(size_t j = 0; j < cycles[i].finalnodeidxsincycle.size(); j++) printf(" %d", cycles[i].finalnodeidxsincycle[j]);
    //     printf("\n");
    // }

    {
        // special case because the input has a weirdly simple structure, each cycle contains only a single final node, which is at index equal to the cycle length
        long long nsteps = 1;
        for(size_t i = 0; i < cycles.size(); i++) nsteps = std::lcm(nsteps, cycles[i].length);
        printf("%lld\n", nsteps);
        return 0;
    }

    long long nsteps;
    for(size_t i = 0; i < cycles.size(); i++)
    {
        long long first_final_step = cycles[i].start + cycles[i].finalnodeidxsincycle.front();
        if(i == 0 || first_final_step > nsteps) nsteps = first_final_step;
    }

    size_t n_successes = 0;
    size_t curr_cycle;
    long long iter = 0;
    while(n_successes < cycles.size())
    {
        // if(iter % 1000000 == 0) printf("iter %lld, nsteps %lld\n", iter, nsteps);
        cycle & c = cycles[curr_cycle];
        int curr_idx_in_cycle = (nsteps - c.start) % c.length;
        if(std::find(c.finalnodeidxsincycle.begin(), c.finalnodeidxsincycle.end(), curr_idx_in_cycle) != c.finalnodeidxsincycle.end())
        {
            n_successes++;
        }
        else
        {
            n_successes = 0;
            int next_idx_in_cycle = c.finalnodeidxsincycle.front() + c.length;
            for(size_t i = 0; i < c.finalnodeidxsincycle.size(); i++)
            {
                if(c.finalnodeidxsincycle[i] > curr_idx_in_cycle)
                {
                    next_idx_in_cycle = c.finalnodeidxsincycle[i];
                    break;
                }
            }
            int numsteps = next_idx_in_cycle - curr_idx_in_cycle;
            nsteps += numsteps;
        }
        curr_cycle = (curr_cycle + 1) % cycles.size();
        iter++;
    }

    printf("%lld\n", nsteps);


    return 0;
}
