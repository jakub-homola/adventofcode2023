#include "../helpers/helpers.hpp"



struct module
{
    const char * name;
    std::vector<const char *> outputs_names;
    std::vector<module *> outputs;
    std::vector<module *> inputs;
    int idx_in_types = -1;
    char type;
};

struct pulse
{
    module & src;
    module & dst;
    char type;
};

struct range
{
    long long first;
    long long last;
};

struct weird_cycle
{
    module * conj = nullptr;
    std::vector<module *> flipflops;
    long long period;
};

void invert_pulse_type_inplace(char & p)
{
    if(p == 'H') p = 'L';
    else if(p == 'L') p = 'H';
}



// some specially crafted input data again ... ... ...
int main()
{
    std::vector<std::vector<char>> input;
    read_lines(input);

    std::vector<module> modules;
    {
        module & button = modules.emplace_back();
        button.name = "button";
        button.type = 'B';
        button.outputs_names.push_back("broadcaster");
    }

    for(std::vector<char> & linevec : input)
    {
        module & nm = modules.emplace_back();
        char * str = linevec.data();

        if(str[0] == '%')
        {
            nm.type = 'F';
            str++;
        }
        else if(str[0] == '&')
        {
            nm.type = 'C';
            str++;
        }
        else
        {
            nm.type = 'X';
        }

        char * space = strchr(str, ' ');
        *space = '\0';
        nm.name = str;
        str = space + 3;

        std::vector<char *> connected_str = split_string(str, ',');
        nm.outputs_names.resize(connected_str.size());
        std::transform(connected_str.begin(), connected_str.end(), nm.outputs_names.begin(), [](char * cs){ return cs + 1; });
    }

    module & m_button = modules[0];
    module & m_broadcaster = *std::find_if(modules.begin(), modules.end(), [](const module & m){ return strcmp(m.name, "broadcaster") == 0; });
    module & m_rx = *std::find_if(modules.begin(), modules.end(), [](const module & m){ return strcmp(m.name, "rx") == 0; });

    for(module & m : modules)
    {
        for(const char * con_name : m.outputs_names)
        {
            auto it = std::find_if(modules.begin(), modules.end(), [&](const module & m){ return strcmp(con_name, m.name) == 0; });
            if(it == modules.end())
            {
                module & nm = modules.emplace_back();
                nm.name = con_name;
                nm.type = '_';
                it = modules.end() - 1;
            }
            m.outputs.push_back(&(*it));
        }
    }

    for(module & m : modules)
    {
        for(module * mdst : m.outputs)
        {
            mdst->inputs.push_back(&m);
        }
    }

    std::vector<weird_cycle> weird_cycles;
    for(size_t i = 0; i < m_broadcaster.outputs.size(); i++)
    {
        weird_cycle & wc = weird_cycles.emplace_back();
        module * curr_flipflop = m_broadcaster.outputs[i];
        while(curr_flipflop != nullptr)
        {
            wc.flipflops.push_back(curr_flipflop);
            module * m1 = (curr_flipflop->outputs.size() >= 1) ? curr_flipflop->outputs[0] : nullptr;
            module * m2 = (curr_flipflop->outputs.size() >= 2) ? curr_flipflop->outputs[1] : nullptr;
            curr_flipflop = nullptr;
            if(m1 != nullptr && m1->type == 'F') curr_flipflop = m1;
            if(m2 != nullptr && m2->type == 'F') curr_flipflop = m2;
            if(m1 != nullptr && m1->type == 'C') wc.conj = m1;
            if(m2 != nullptr && m2->type == 'C') wc.conj = m2;
        }
    }

    for(weird_cycle & wc : weird_cycles)
    {
        long long num = 0;
        for(size_t i = 0; i < wc.flipflops.size(); i++)
        {
            module * ff = wc.flipflops[i];
            if(std::count_if(ff->outputs.begin(), ff->outputs.end(), [](const module * m){return m->type == 'C';}) > 0) num |= 1ll << i;
        }
        wc.period = num;
    }

    // for(weird_cycle & wc : weird_cycles)
    // {
    //     printf("conj %s flipflops", wc.conj->name);
    //     for(module * m : wc.flipflops) printf(" %s", m->name);
    //     printf("   toconj ");
    //     for(module * m : wc.flipflops) printf("%zu", std::count_if(m->outputs.begin(), m->outputs.end(), [](const module * m){return m->type == 'C';}));
    //     printf("   fromconj ");
    //     for(module * m : wc.flipflops) printf("%zu", std::count_if(m->inputs.begin(), m->inputs.end(), [](const module * m){return m->type == 'C';}));
    //     printf("   period %lld", wc.period);
    //     printf("\n");
    // }

    long long result = 1;
    for(weird_cycle & wc : weird_cycles) result = std::lcm(result, wc.period);

    printf("%lld\n", result);

    return 0;
}
