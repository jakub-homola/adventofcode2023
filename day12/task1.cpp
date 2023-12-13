#include "../helpers/helpers.hpp"



struct springseq
{
    char * str;
    int str_len;
    std::vector<int> sizes;
    std::vector<int> opr;
    std::vector<int> dmg;
    std::vector<int> unk;
    int nopr_needed;
    int ndmg_needed;
};

bool matches(char * str, char * unk, std::vector<int> & sizes)
{
    int curr_seq_size = 0;
    char * curr_unk = unk;
    int curr_size_idx = 0;
    for(char * c = str; true; c++)
    {
        char curr_c = *c;
        if(curr_c == '?')
        {
            curr_c = *curr_unk;
            curr_unk++;
        }
        if(curr_c == '\0') curr_c = '.';
        if(curr_c == '#') curr_seq_size++;
        if(curr_c == '.' && curr_seq_size > 0)
        {
            if(curr_seq_size != sizes[curr_size_idx]) return false;
            curr_seq_size = 0;
            curr_size_idx++;
        }
        if(*c == '\0') return true;
    }
}



int main()
{
    std::vector<std::string> lines = read_lines();
    std::vector<springseq> springseqs(lines.size());

    for(size_t i = 0; i < lines.size(); i++)
    {
        springseq & s = springseqs[i];
        s.str = lines[i].data();
        char * currstr = strchr(s.str, ' ');
        *currstr = '\0';
        s.str_len = strlen(s.str);
        while(currstr != nullptr)
        {
            currstr++;
            int num;
            sscanf(currstr, "%d", &num);
            s.sizes.push_back(num);
            currstr = strchr(currstr, ',');
        }
    }

    for(size_t i = 0; i < springseqs.size(); i++)
    {
        springseq & s = springseqs[i];
        for(int j = 0; j < s.str_len; j++)
        {
            if(s.str[j] == '.') s.opr.push_back(j);
            if(s.str[j] == '#') s.dmg.push_back(j);
            if(s.str[j] == '?') s.unk.push_back(j);
        }
        s.ndmg_needed = std::accumulate(s.sizes.begin(), s.sizes.end(), 0);
        s.nopr_needed = s.str_len - s.ndmg_needed;
    }

    int result = 0;
    for(size_t i = 0; i < springseqs.size(); i++)
    {
        springseq & s = springseqs[i];

        std::vector<char> unk_arrangement;
        unk_arrangement.reserve(s.unk.size() + 1);
        int nopr_remaining = s.nopr_needed - s.opr.size();
        int ndmg_remaining = s.ndmg_needed - s.dmg.size();
        for(int j = 0; j < nopr_remaining; j++) unk_arrangement.push_back('.');
        for(int j = 0; j < ndmg_remaining; j++) unk_arrangement.push_back('#');
        unk_arrangement.push_back('\0');
        std::sort(unk_arrangement.begin(), unk_arrangement.end() - 1);
        do
        {
            bool m = matches(s.str, unk_arrangement.data(), s.sizes);
            if(m) result++;
        } while(std::next_permutation(unk_arrangement.begin(), unk_arrangement.end() - 1));
    }

    printf("%d\n", result);

    return 0;
}
