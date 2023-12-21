#include "../helpers/helpers.hpp"



struct module
{
    const char * name;
    std::vector<const char *> connected_names;
    std::vector<module *> connected;
    int idx_in_types = -1;
    char type;
};

struct pulse
{
    module & src;
    module & dst;
    char type;
};



int main()
{
    int button_presses = 1000;
    std::vector<std::vector<char>> input;
    read_lines(input);

    std::vector<module> modules;
    {
        module & button = modules.emplace_back();
        button.name = "button";
        button.type = 'B';
        button.connected_names.push_back("broadcaster");
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
        nm.connected_names.resize(connected_str.size());
        std::transform(connected_str.begin(), connected_str.end(), nm.connected_names.begin(), [](char * cs){ return cs + 1; });
    }

    module & m_button_ptr = modules[0];
    module & m_broadcaster_ptr = *std::find_if(modules.begin(), modules.end(), [](const module & m){ return strcmp(m.name, "broadcaster") == 0; });

    for(module & m : modules)
    {
        for(const char * con_name : m.connected_names)
        {
            m.connected.push_back(&(*std::find_if(modules.begin(), modules.end(), [&](const module & m){ return strcmp(con_name, m.name) == 0; })));
        }
    }

    std::vector<bool> states_flipflop;
    std::vector<std::vector<std::pair<module *, char>>> states_conjunction;
    for(module & m : modules)
    {
        if(m.type == 'F')
        {
            m.idx_in_types = states_flipflop.size();
            states_flipflop.push_back(false);
        }
        if(m.type == 'C')
        {
            m.idx_in_types = states_conjunction.size();
            states_conjunction.emplace_back();
        }
    }
    for(module & m : modules)
    {
        for(module * mdst : m.connected)
        {
            if(mdst->type == 'C')
            {
                states_conjunction[mdst->idx_in_types].push_back({&m, 'L'});
            }
        }
    }

    long long n_pulses_hi = 0;
    long long n_pulses_lo = 0;
    for(int i = 0; i < button_presses; i++)
    {
        std::deque<pulse> pulses;
        pulses.push_back({m_button_ptr, m_broadcaster_ptr, 'L'});
        while(!pulses.empty())
        {
            pulse p = pulses.front();
            pulses.pop_front();
            if(p.type == 'L') n_pulses_lo++;
            if(p.type == 'H') n_pulses_hi++;
            module & m = p.dst;
            // printf("%12s --%c--> %12s\n", p.src.name, p.type, p.dst.name);
            
            if(m.type == 'X')
            {
                for(module * mdst : m.connected)
                {
                    pulses.push_back({m, *mdst, p.type});
                }
            }
            if(m.type == 'F')
            {
                if(p.type == 'L')
                {
                    char tosend = 'H';
                    if(states_flipflop[m.idx_in_types]) tosend = 'L';
                    for(module * mdst : m.connected)
                    {
                        pulses.push_back({m, *mdst, tosend});
                    }
                    states_flipflop[m.idx_in_types] = !states_flipflop[m.idx_in_types];
                }
            }
            if(m.type == 'C')
            {
                module & msrc = p.src;
                std::vector<std::pair<module *, char>> & state = states_conjunction[m.idx_in_types];
                std::find_if(state.begin(), state.end(), [&](const std::pair<module *, char> & p){ return &msrc == p.first; })->second = p.type;
                char tosend = 'H';
                if(std::all_of(state.begin(), state.end(), [](const std::pair<module *, char> & p){ return p.second == 'H'; })) tosend = 'L';
                for(module * mdst : m.connected)
                {
                    pulses.push_back({m, *mdst, tosend});
                }
            }
        }
    }

    printf("%lld\n", n_pulses_hi * n_pulses_lo);

    return 0;
}
