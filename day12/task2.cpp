#include "../helpers/helpers.hpp"


struct memoization
{
    std::unordered_map<long long, long long> map;
    long long make_key(long long curr_unk, long long ndmg_todo, long long curr_sec_size)
    {
        return (curr_unk << 40) | (ndmg_todo << 20) | (curr_sec_size << 0);
    }
    bool get(int curr_unk, int ndmg_todo, int curr_sec_size, long long & val)
    {
        auto it = map.find(make_key(curr_unk, ndmg_todo, curr_sec_size));
        if(it == map.end()) return false;
        val = it->second;
        return true;
    }
    void set(int curr_unk, int ndmg_todo, int curr_sec_size, long long val)
    {
        map[make_key(curr_unk, ndmg_todo, curr_sec_size)] = val;
    }
};

bool matches(const std::string & springs, const std::string & unk, const std::vector<int> & sections, int curr_unk)
{
    int curr_seq_size = 0;
    int curr_unk_idx = 0;
    int curr_size_idx = 0;
    for(int i = 0; true; i++)
    {
        char curr_c = springs[i];
        if(curr_c == '?')
        {
            if(curr_unk_idx == curr_unk) return true;
            curr_c = unk[curr_unk_idx];
            curr_unk_idx++;
        }
        if(curr_c == '\0') curr_c = '.';
        if(curr_c == '#') curr_seq_size++;
        if(curr_c == '.' && curr_seq_size > 0)
        {
            if(curr_seq_size != sections[curr_size_idx]) return false;
            curr_seq_size = 0;
            curr_size_idx++;
        }
        if(i == springs.length()) return true;
    }
}

long long get_n_possibilities(const std::string & springs, std::string & unks, const std::vector<int> sections, int curr_unk, int curr_sec_size, int ndmg_todo, int nopr_todo, memoization & mem)
{
    // for(int i = 0; i < curr_unk; i++) printf("  "); printf("BBB %s   %s   %d %d %d %d  ", springs.c_str(), unks.c_str(), curr_unk, curr_sec_size, ndmg_todo, nopr_todo); for(size_t i = 0; i < sections.size(); i++) printf(" %d", sections[i]); printf("\n");
    long long n_possibilities;
    if(ndmg_todo < 0) n_possibilities = 0;
    else if(nopr_todo < 0) n_possibilities = 0;
    else if(!matches(springs, unks, sections, curr_unk)) n_possibilities = 0;
    else if(unks[curr_unk] == '\0') n_possibilities = 1;
    else
    {
        bool mem_hit = mem.get(curr_unk, ndmg_todo, curr_sec_size, n_possibilities);
        // if(mem_hit) { for(int i = 0; i < curr_unk; i++) printf("  "); printf("HHHIIITTT\n"); }
        if(!mem_hit)
        {
            n_possibilities = 0;
            unks[curr_unk] = '.';
            n_possibilities += get_n_possibilities(springs, unks, sections, curr_unk + 1, 0, ndmg_todo, nopr_todo - 1, mem);
            unks[curr_unk] = '#';
            n_possibilities += get_n_possibilities(springs, unks, sections, curr_unk + 1, curr_sec_size + 1, ndmg_todo - 1, nopr_todo, mem);
            unks[curr_unk] = '-';

            mem.set(curr_unk, ndmg_todo, curr_sec_size, n_possibilities);
        }
    }
    // for(int i = 0; i < curr_unk; i++) printf("  "); printf("CCC %s   %s   %d %d %d %d  ", springs.c_str(), unks.c_str(), curr_unk, curr_sec_size, ndmg_todo, nopr_todo); for(size_t i = 0; i < sections.size(); i++) printf(" %d", sections[i]); printf("\n");
    // for(int i = 0; i < curr_unk; i++) printf("  "); printf("DDD %lld\n", n_possibilities);
    return n_possibilities;
}



int main()
{
    std::vector<std::string> lines = read_lines();
    int nfolds = 5;

    long long result = 0;
    for(size_t r = 0; r < lines.size(); r++)
    {
        std::string springs;
        std::vector<int> sections;
        {
            const char * line = lines[r].c_str();
            const char * space = strchr(line, ' ');
            int spring_count_input = space - line;
            springs.reserve(nfolds * spring_count_input + nfolds - 1);
            for(int f = 0; f < nfolds; f++)
            {
                if(f != 0) springs.push_back('?');
                springs.insert(springs.end(), line, space);
            }
            std::vector<int> sections_input;
            for(const char * s = space + 1; s > line; s = strchr(s, ',') + 1)
            {
                int num;
                sscanf(s, "%d", &num);
                sections_input.push_back(num);
            }
            sections.reserve(nfolds * sections_input.size());
            for(int f = 0; f < nfolds; f++) sections.insert(sections.end(), sections_input.begin(), sections_input.end());
        }

        int ndmg_done = std::count(springs.begin(), springs.end(), '#');
        int nunk = std::count(springs.begin(), springs.end(), '?');
        int ndmg_total = std::accumulate(sections.begin(), sections.end(), 0);
        int ndmg_todo = ndmg_total - ndmg_done;
        int nopr_todo = nunk - ndmg_todo;

        std::string unks(nunk, '-');
        memoization mem;
        long long n_possibilities = get_n_possibilities(springs, unks, sections, 0, 0, ndmg_todo, nopr_todo, mem);
        result += n_possibilities;

        // printf("AAA %s  ", springs.c_str()); for(size_t i = 0; i < sections.size(); i++) printf(" %d", sections[i]); printf("\n");
        // printf("YYY  %lld\n", n_possibilities);
    }

    printf("%lld\n", result);

    return 0;
}
