#include "../helpers/helpers.hpp"



struct workflow;

struct rule
{
    const char * my_str = "";
    const char * next_str_true = nullptr;
    const char * next_str_false = nullptr;
    int next_idx_true = -1;
    int next_idx_false = -1;
    int compareto;
    char param;
    char comparison;
    bool is_end_accept = false;
    bool is_end_reject = false;
};

struct part_state
{
    // start inclusive, end inclusive
    int rule_idx;
    std::array<int,4> vals_starts = {1,1,1,1};
    std::array<int,4> vals_ends = {4000,4000,4000,4000};
    part_state() {}
    explicit part_state(int r) : rule_idx(r) {}
    long long n_states()
    {
        long long result = 1;
        #pragma unroll
        for(size_t i = 0; i < vals_starts.size(); i++)
        {
            result *= vals_ends[i] - vals_starts[i] + 1;
        }
        return result;
    }
};

int letter_to_idx(char c)
{
    switch(c)
    {
        case 'x': return 0;
        case 'm': return 1;
        case 'a': return 2;
        case 's': return 3;
    }
    return -1;
}



int main()
{
    std::vector<std::vector<char>> input;
    read_lines(input);

    std::vector<rule> rules;
    {
        rule rule_accept;
        rule_accept.next_str_true = "A";
        rule_accept.next_str_false = "A";
        rule_accept.next_idx_true = rules.size();
        rule_accept.next_idx_false = rules.size();
        rule_accept.my_str = "A";
        rule_accept.is_end_accept = true;
        rule_accept.param = '_';
        rule_accept.comparison = '_';
        rule_accept.compareto = -1;
        rules.push_back(rule_accept);
    }
    {
        rule rule_reject;
        rule_reject.next_str_true = "R";
        rule_reject.next_str_false = "R";
        rule_reject.next_idx_true = rules.size();
        rule_reject.next_idx_false = rules.size();
        rule_reject.my_str = "R";
        rule_reject.is_end_reject = true;
        rule_reject.param = '_';
        rule_reject.comparison = '_';
        rule_reject.compareto = -1;
        rules.push_back(rule_reject);
    }
    for(size_t l = 0; l < input.size(); l++)
    {
        char * str = input[l].data();
        if(str[0] == '\0') break;
        char * nameend = strchr(str, '{');
        char * lineend = strchr(str, '}');
        *nameend = '\0';
        *lineend = '\0';
        const char * firstname = str;
        str = nameend + 1;
        std::vector<char *> rules_str = split_string(str, ',');
        for(char * rule_str : rules_str)
        {
            char * colon = strchr(rule_str, ':');
            rule & r = rules.emplace_back();
            r.my_str = firstname;
            firstname = "";
            if(colon == nullptr)
            {
                r.next_str_true = rule_str;
                r.next_idx_false = -1;
                r.param = 'x';
                r.comparison = '>';
                r.compareto = 0;
            }
            else
            {
                sscanf(rule_str, "%c%c%d", &r.param, &r.comparison, &r.compareto);
                r.next_str_true = colon + 1;
                r.next_idx_false = rules.size();
            }
        }
    }

    for(rule & r : rules)
    {
        auto it = std::find_if(rules.begin(), rules.end(), [&](const rule & rr){ return strcmp(rr.my_str, r.next_str_true) == 0; });
        r.next_idx_true = it - rules.begin();
    }

    int rule_start_idx = std::find_if(rules.begin(), rules.end(), [](const rule & rr){ return strcmp(rr.my_str, "in") == 0; }) - rules.begin();

    long long result = 0;
    std::vector<part_state> states;
    states.emplace_back(rule_start_idx);
    while(!states.empty())
    {
        // printf("%lld\n", result);
        // for(part_state & s : states)
        // {
        //     rule & r = rules[s.rule_idx];
        //     printf("state: rule %4d '%4s' {%c %c %4d ? %4d : %4d}, state vals x=<%4d,%4d> m=<%4d,%4d> a=<%4d,%4d> s=<%4d,%4d>\n", s.rule_idx, r.my_str, r.param, r.comparison, r.compareto, r.next_idx_true, r.next_idx_false, s.vals_starts[0], s.vals_ends[0], s.vals_starts[1], s.vals_ends[1], s.vals_starts[2], s.vals_ends[2], s.vals_starts[3], s.vals_ends[3]);
        // }
        // printf("\n");

        part_state state = states.back();
        states.pop_back();
        rule & r = rules[state.rule_idx];

        if(r.is_end_accept)
        {
            result += state.n_states();
            continue;
        }
        if(r.is_end_reject)
        {
            continue;
        }
        int validx = letter_to_idx(r.param);
        {
            part_state state_true = state;
            if(r.comparison == '<') state_true.vals_ends[validx] = std::min(state_true.vals_ends[validx], r.compareto - 1);
            if(r.comparison == '>') state_true.vals_starts[validx] = std::max(state_true.vals_starts[validx], r.compareto + 1);
            state_true.rule_idx = r.next_idx_true;
            if(state_true.vals_starts[validx] <= state_true.vals_ends[validx])
            {
                states.push_back(state_true);
            }
        }
        {
            part_state state_false = state;
            if(r.comparison == '<') state_false.vals_starts[validx] = std::max(state_false.vals_starts[validx], r.compareto);
            if(r.comparison == '>') state_false.vals_ends[validx] = std::min(state_false.vals_ends[validx], r.compareto);
            state_false.rule_idx = r.next_idx_false;
            if(state_false.vals_starts[validx] <= state_false.vals_ends[validx])
            {
                states.push_back(state_false);
            }
        }
    }

    printf("%lld\n", result);

    return 0;
}
