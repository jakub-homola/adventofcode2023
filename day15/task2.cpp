#include "../helpers/helpers.hpp"

// Determine the ASCII code for the current character of the string.
// Increase the current value by the ASCII code you just determined.
// Set the current value to itself multiplied by 17.
// Set the current value to the remainder of dividing itself by 256.
unsigned char hash(const char * s)
{
    unsigned char h = 0;
    for(int i = 0; s[i] != '\0'; i++)
    {
        h = (h + s[i]) * 17;
    }
    return h;
}

struct lens
{
    char * str;
    int focal_length;
    unsigned char hash;
};



int main()
{
    char str[1000000];
    scanf("%s", str);
    char * str_end = strchr(str, '\0');
    if(str_end[-1] == '\r')
    {
        str_end--;
        *str_end = '\0';
    }

    std::array<std::vector<lens>,256> boxes;
    char * curr_str = str;
    while(curr_str < str_end)
    {
        char * curr_str_end = strchr(curr_str, ',');
        if(curr_str_end == nullptr) curr_str_end = strchr(curr_str, '\0');
        *curr_str_end = '\0';
        int len = curr_str_end - curr_str;
        lens l;
        l.str = curr_str;
        l.focal_length = l.str[len-1] - '0';
        bool do_add = (l.str[len-2] == '=');
        if(do_add) l.str[len-2] = '\0';
        else l.str[len-1] = '\0';
        l.hash = hash(l.str);
        std::vector<lens> & box = boxes[l.hash];
        auto it = std::find_if(box.begin(), box.end(), [&l](lens l2){ return (strcmp(l2.str, l.str) == 0); });
        if(do_add)
        {
            if(it == box.end()) box.push_back(l);
            else *it = l;
        }
        else
        {
            if(it != box.end()) box.erase(it);
        }
        curr_str = curr_str_end + 1;
    }

    long long result = 0;
    for(size_t i = 0; i < boxes.size(); i++)
    {
        std::vector<lens> & box = boxes[i];
        for(size_t j = 0; j < box.size(); j++)
        {
            int fp = (i+1) * (j+1) * box[j].focal_length;
            result += fp;
        }
    }
    printf("%lld\n", result);

    return 0;
}
