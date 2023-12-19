#include "../helpers/helpers.hpp"



struct workflow;

struct rule
{
    workflow * next = nullptr;
    const char * next_str;
    int compareto;
    char param;
    char comparison;
    bool is_end_accept = false;
    bool is_end_reject = false;
};

struct workflow
{
    std::vector<rule> rules;
    const char * name;
};

struct part
{
    int x, m, a, s;
};

bool part_satisfies_rule(rule & r, part & p)
{
    if(r.comparison == '_') return true;

    int val = -1;
    if(r.param == 'x') val = p.x;
    if(r.param == 'm') val = p.m;
    if(r.param == 'a') val = p.a;
    if(r.param == 's') val = p.s;

    if(r.comparison == '<') return (val < r.compareto);
    if(r.comparison == '>') return (val > r.compareto);
    return false;
}



int main()
{
    std::vector<std::vector<char>> input;
    read_lines(input);

    std::vector<workflow> workflows;
    std::vector<part> parts;
    size_t l = 0;
    for(; l < input.size(); l++)
    {
        char * str = input[l].data();
        if(str[0] == '\0') break;
        workflow & w = workflows.emplace_back();
        char * nameend = strchr(str, '{');
        char * lineend = strchr(str, '}');
        *nameend = '\0';
        *lineend = '\0';
        w.name = str;
        str = nameend + 1;
        std::vector<char *> rules_str = split_string(str, ',');
        for(char * rule_str : rules_str)
        {
            rule & r = w.rules.emplace_back();
            char * colon = strchr(rule_str, ':');
            if(colon == nullptr)
            {
                r.next_str = rule_str;
                r.param = '_';
                r.comparison = '_';
                r.compareto = 0;
            }
            else
            {
                sscanf(rule_str, "%c%c%d", &r.param, &r.comparison, &r.compareto);
                r.next_str = colon + 1;
            }
        }
    }
    l++;
    for(; l < input.size(); l++)
    {
        char * str = input[l].data();
        part & p = parts.emplace_back();

        char * linestart = strchr(str, '{');
        char * lineend = strchr(str, '}');
        *linestart = '\0';
        *lineend = '\0';
        str = linestart + 1;
        std::vector<char *> props = split_string(str, ',');
        for(char * prop : props)
        {
            char prop_name = prop[0];
            int prop_val;
            sscanf(prop + 2, "%d", &prop_val);
            if(prop_name == 'x') p.x = prop_val;
            if(prop_name == 'm') p.m = prop_val;
            if(prop_name == 'a') p.a = prop_val;
            if(prop_name == 's') p.s = prop_val;
        }
    }

    for(workflow & w : workflows)
    {
        for(rule & r : w.rules)
        {
            if(strcmp(r.next_str,"R") == 0) { r.is_end_reject = true; continue; }
            if(strcmp(r.next_str,"A") == 0) { r.is_end_accept = true; continue; }

            auto it = std::find_if(workflows.begin(), workflows.end(), [&](const workflow & wf){ return strcmp(wf.name, r.next_str) == 0; });
            r.next = &(*it);
        }
    }

    workflow * workflow_start = &(*std::find_if(workflows.begin(), workflows.end(), [](const workflow & wf){ return strcmp(wf.name, "in") == 0; }));

    int result = 0;
    for(size_t ip = 0; ip < parts.size(); ip++)
    {
        part & p = parts[ip];
        workflow * w = workflow_start;
        while(true)
        {
            for(size_t ir = 0; ir < w->rules.size(); ir++)
            {
                rule & r = w->rules[ir];
                if(part_satisfies_rule(r, p))
                {
                    if(r.is_end_accept)
                    {
                        result += p.x + p.m + p.a + p.s;
                        goto nextpart;
                    }
                    if(r.is_end_reject)
                    {
                        goto nextpart;
                    }
                    w = r.next;
                    break;
                }
            }
        }
        nextpart:
        ;
    }

    printf("%d\n", result);

    return 0;
}
