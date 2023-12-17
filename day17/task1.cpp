#include "../helpers/helpers.hpp"



struct direction
{
    int val;
    direction() : val(-1) {}
    constexpr direction(int v) : val{v} {}
    operator int() const { return val; }
    bool operator==(const direction & other) const { return other.val == val; }
};

struct directions
{
    constexpr static direction north = direction(0);
    constexpr static direction east  = direction(1);
    constexpr static direction south = direction(2);
    constexpr static direction west  = direction(3);
};

struct state
{
    int r;
    int c;
    int heatloss;
    int stepsindir;
    direction dir;
    struct comparator
    {
        bool operator()(const state & s1, const state & s2) const
        {
            if(s1.heatloss != s2.heatloss) return s1.heatloss < s2.heatloss;
            if(s1.stepsindir != s2.stepsindir) return s1.stepsindir < s2.stepsindir;
            if(s1.r != s2.r) return s1.r < s2.r;
            if(s1.c != s2.c) return s1.c < s2.c;
            return (int)s1.dir < (int)s2.dir;
        }
    };
};


int main()
{
    int max_steps_in_dir = 3;
    std::vector<std::string> input = read_lines();
    int nrows = input.size();
    int ncols = input[0].size();
    std::vector<int> heatloss_vec(nrows * ncols);
    arrayview_2d<int> heatloss(heatloss_vec.data(), nrows, ncols);
    for(int r = 0; r < nrows; r++) for(int c = 0; c < ncols; c++) heatloss[r][c] = input[r][c] - '0';

    bool * visited_vec = new bool[nrows * ncols * 4 * max_steps_in_dir];
    std::fill(visited_vec, visited_vec + nrows * ncols * 4 * max_steps_in_dir, false);
    size_t sizes[] = {static_cast<size_t>(nrows), static_cast<size_t>(ncols), static_cast<size_t>(4), static_cast<size_t>(max_steps_in_dir)};
    arrayview_nd<bool,4> visited(visited_vec, sizes);
    std::set<state, state::comparator> pathends;
    pathends.insert({0,0,0,0,directions::north});

    state laststate;
    while(pathends.size() > 0)
    {
        auto it = pathends.begin();
        state currstate = *it;
        pathends.erase(it);

        if(currstate.r == nrows-1 && currstate.c == ncols-1)
        {
            laststate = currstate;
            break;
        }

        for(int d = 0; d < 4; d++)
        {
            direction dir = static_cast<direction>(d);
            state nextstate = currstate;
            if(((int)dir + 2) % 4 == currstate.dir) continue;

            if(dir == directions::north) nextstate.r--;
            if(dir == directions::south) nextstate.r++;
            if(dir == directions::east)  nextstate.c++;
            if(dir == directions::west)  nextstate.c--;
            if(nextstate.r < 0 || nextstate.r >= nrows || nextstate.c < 0 || nextstate.c >= ncols) continue;

            if(dir == nextstate.dir) nextstate.stepsindir++;
            else nextstate.stepsindir = 1;
            nextstate.dir = dir;
            if(nextstate.stepsindir > max_steps_in_dir) continue;
            
            auto sid = visited[nextstate.r][nextstate.c][nextstate.dir];
            if(sid[nextstate.stepsindir-1]) continue;
            for(int s = nextstate.stepsindir; s <= max_steps_in_dir; s++) sid[s-1] = true;

            nextstate.heatloss += heatloss[nextstate.r][nextstate.c];
            pathends.insert(nextstate);
        }
    }

    delete[] visited_vec;

    printf("%d\n", laststate.heatloss);

    return 0;
}
