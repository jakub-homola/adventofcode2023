#include <cstdio>
#include <vector>
#include <algorithm>
#include <array>
#include <cstring>



struct player
{
    char hand[32];
    long long handval;
    std::array<int,20> cards;
    int bid;
    int njokers;
};

int card_to_val(char card)
{
    switch(card)
    {
        case 'A': return 14;
        case 'K': return 13;
        case 'Q': return 12;
        case 'J': return 1;
        case 'T': return 10;
        default: return card - '0';
    }
}

void setup_cardarray(player & p)
{
    std::fill(p.cards.begin(), p.cards.end(), 0);
    p.njokers = 0;
    for(const char * c = p.hand; *c != '\0'; c++)
    {
        int cv = card_to_val(*c);
        if(*c == 'J') p.njokers++;
        else p.cards[cv]++;
    }
}

int jokermap[8][6] = {
    {0,0,0,0,0,0},
    {1,2,4,6,7,7}, // 1 = nothing
    {2,4,6,7,0,0}, // 2 = one pair
    {3,5,0,0,0,0}, // 3 = two pairs
    {4,6,7,0,0,0}, // 4 = 3 of
    {5,0,0,0,0,0}, // 5 = fullhouse
    {6,7,0,0,0,0}, // 6 = 4 of
    {7,0,0,0,0,0}  // 7 = 5 of
};

int get_multiplier(int handkind, int njokers)
{
    int newhandkind = jokermap[handkind][njokers];
    int mult = 1;
    for(int i = 0; i < newhandkind; i++) mult *= 10;
    return mult;
}

void setup_handval(player & p)
{
    long long val = 0;
    long long currmult = 1;

    for(int i = strlen(p.hand)-1; i >= 0; i--)
    {
        val += (card_to_val(p.hand[i]) * currmult);
        currmult *= 100;
    }

    std::vector<int> multiples_counts;
    for(size_t i = 0; i < p.cards.size(); i++)
    {
        if(p.cards[i] != 0) multiples_counts.push_back(p.cards[i]);
    }
    std::sort(multiples_counts.begin(), multiples_counts.end());

    if(std::count_if(multiples_counts.begin(), multiples_counts.end(), [](int m){ return m == 5; }) == 1) val += (1 * currmult * get_multiplier(7, p.njokers)); // five of a kind
    else if(std::count_if(multiples_counts.begin(), multiples_counts.end(), [](int m){ return m == 4; }) == 1) val += (1 * currmult * get_multiplier(6, p.njokers)); // four of a kind
    else if(std::count_if(multiples_counts.begin(), multiples_counts.end(), [](int m){ return m == 2; }) == 1
        && std::count_if(multiples_counts.begin(), multiples_counts.end(), [](int m){ return m == 3; }) == 1) val += (1 * currmult * get_multiplier(5, p.njokers)); // full house
    else if(std::count_if(multiples_counts.begin(), multiples_counts.end(), [](int m){ return m == 3; }) == 1) val += (1 * currmult * get_multiplier(4, p.njokers)); // three of a kind
    else if(std::count_if(multiples_counts.begin(), multiples_counts.end(), [](int m){ return m == 2; }) == 2) val += (1 * currmult * get_multiplier(3, p.njokers)); // two pair
    else if(std::count_if(multiples_counts.begin(), multiples_counts.end(), [](int m){ return m == 2; }) == 1) val += (1 * currmult * get_multiplier(2, p.njokers)); // one pair
    else val += (1 * currmult * get_multiplier(1, p.njokers)); // nothing

    p.handval = val;
}





int main()
{
    std::vector<player> players;

    while(true)
    {
        player np;
        int res = scanf("%s %d", np.hand, &np.bid);
        if(res < 2) break;
        setup_cardarray(np);
        setup_handval(np);
        players.push_back(np);
    }

    std::sort(players.begin(), players.end(), [](const player & l, const player & r) { return l.handval > r.handval; });

    // printf("                          54f3p2n_aabbccddee\n");
    // for(size_t i = 0; i < players.size(); i++) printf("hand %s bid %4d val %20lld\n", players[i].hand, players[i].bid, players[i].handval);
    // printf("                          54f3p2n_aabbccddee\n");

    int result = 0;
    for(size_t i = 0; i < players.size(); i++)
    {
        int rank = players.size() - i;
        result += players[i].bid * rank;
    }

    printf("%d\n", result);

    return 0;
}
