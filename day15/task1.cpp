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



int main()
{
    char str[1000000];
    scanf("%s", str);

    int result = 0;
    char * curr_str = str - 1;
    while(curr_str != nullptr)
    {
        curr_str++;
        char * commapos = strchr(curr_str, ',');
        if(commapos != nullptr) *commapos = '\0';
        result += hash(curr_str);
        curr_str = commapos;
    }

    printf("%d\n", result);

    return 0;
}
