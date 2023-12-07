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
};

int card_to_val(char card)
{
    switch(card)
    {
        case 'A': return 14;
        case 'K': return 13;
        case 'Q': return 12;
        case 'J': return 11;
        case 'T': return 10;
        default: return card - '0';
    }
}

void setup_cardarray(player & p)
{
    std::fill(p.cards.begin(), p.cards.end(), 0);
    for(const char * c = p.hand; *c != '\0'; c++)
    {
        int cv = card_to_val(*c);
        p.cards[cv]++;
    }
}

void setup_handval(player & p)
{
    long val = 0;
    long currmult = 1;

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

    if(std::count_if(multiples_counts.begin(), multiples_counts.end(), [](int m){ return m == 2; }) == 1) val += (1 * currmult); // one pair
    currmult *= 10;
    if(std::count_if(multiples_counts.begin(), multiples_counts.end(), [](int m){ return m == 2; }) == 2) val += (1 * currmult); // two pair
    currmult *= 10;
    if(std::count_if(multiples_counts.begin(), multiples_counts.end(), [](int m){ return m == 3; }) == 1) val += (1 * currmult); // three of a kind
    currmult *= 10;
    if(std::count_if(multiples_counts.begin(), multiples_counts.end(), [](int m){ return m == 2; }) == 1
        && std::count_if(multiples_counts.begin(), multiples_counts.end(), [](int m){ return m == 3; }) == 1) val += (1 * currmult); // full house
    currmult *= 10;
    if(std::count_if(multiples_counts.begin(), multiples_counts.end(), [](int m){ return m == 4; }) == 1) val += (1 * currmult); // four of a kind
    currmult *= 10;
    if(std::count_if(multiples_counts.begin(), multiples_counts.end(), [](int m){ return m == 5; }) == 1) val += (1 * currmult); // five of a kind
    currmult *= 10;

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

    // printf("                        54f3p2aabbccddee\n");
    // for(size_t i = 0; i < players.size(); i++) printf("hand %s bid %4d val %16lld\n", players[i].hand, players[i].bid, players[i].handval);
    // printf("                        54f3p2aabbccddee\n");

    int result = 0;
    for(size_t i = 0; i < players.size(); i++)
    {
        int rank = players.size() - i;
        result += players[i].bid * rank;
    }

    printf("%d\n", result);

    return 0;
}
