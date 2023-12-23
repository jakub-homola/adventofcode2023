#include "../helpers/helpers.hpp"



struct coord
{
    int r;
    int c;
};

struct node
{
    coord pos;
    std::vector<int> next_node_idxs;
    std::vector<int> next_dists;
};

struct hike
{
    int curr_node_idx;
    std::vector<bool> visited_nodes;
    int length;
};



int main()
{
    std::vector<std::vector<char>> terrain;
    read_lines(terrain);
    terrain.insert(terrain.begin(), std::vector<char>(terrain[0].size() - 1, '#'));
    terrain.front().push_back('\0');
    terrain.push_back(std::vector<char>(terrain[0].size() - 1, '#'));
    terrain.back().push_back('\0');
    int nrows = terrain.size();
    int ncols = terrain[1].size() - 1;

    coord start_pos = {1, static_cast<int>(strchr(terrain[1].data(), '.') - terrain[1].data())};
    coord final_pos = {nrows-2, static_cast<int>(strchr(terrain[nrows-2].data(), '.') - terrain[nrows-2].data())};

    std::vector<std::vector<direction>> possible_dirs_vec(nrows * ncols);
    arrayview_2d<std::vector<direction>> possible_dirs(possible_dirs_vec.data(), nrows, ncols);
    for(int r = 1; r < nrows-1; r++)
    {
        for(int c = 1; c < ncols-1; c++)
        {
            if(terrain[r][c] == '#') continue;

            for(int d = 0; d < 4; d++)
            {
                coord p = {r,c};
                direction dir(d);
                if(dir == directions::north) p.r--;
                if(dir == directions::south) p.r++;
                if(dir == directions::east) p.c++;
                if(dir == directions::west) p.c--;
                if(terrain[p.r][p.c] != '#') possible_dirs[r][c].push_back(dir);
            }
        }
    }

    std::vector<int> node_idx_at_pos_vec(nrows * ncols, -1);
    arrayview_2d<int> node_idx_at_pos(node_idx_at_pos_vec.data(), nrows, ncols);
    std::vector<node> nodes;
    nodes.emplace_back().pos = start_pos;
    nodes.emplace_back().pos = final_pos;
    int start_node_idx = 0;
    int final_node_idx = 1;
    node_idx_at_pos[start_pos.r][start_pos.c] = start_node_idx;
    node_idx_at_pos[final_pos.r][final_pos.c] = final_node_idx;
    for(int r = 1; r < nrows-1; r++)
    {
        for(int c = 1; c < ncols-1; c++)
        {
            if(possible_dirs[r][c].size() > 2)
            {
                node_idx_at_pos[r][c] = nodes.size();
                nodes.emplace_back().pos = {r,c};
            }
        }
    }

    for(size_t ni = 0; ni != nodes.size(); ni++)
    {
        node & ns = nodes[ni];
        for(int d = 0; d < 4; d++)
        {
            direction curr_dir(d);
            int r = ns.pos.r;
            int c = ns.pos.c;
            int curr_dist = 0;
            while(true)
            {
                if(node_idx_at_pos[r][c] != -1 && node_idx_at_pos[r][c] != ni)
                {
                    ns.next_node_idxs.push_back(node_idx_at_pos[r][c]);
                    ns.next_dists.push_back(curr_dist);
                    break;
                }
                if(terrain[r][c] == '<' && curr_dir != directions::west) break;
                if(terrain[r][c] == '>' && curr_dir != directions::east) break;
                if(terrain[r][c] == '^' && curr_dir != directions::north) break;
                if(terrain[r][c] == 'v' && curr_dir != directions::south) break;
                if(curr_dir == directions::north) r--;
                if(curr_dir == directions::south) r++;
                if(curr_dir == directions::east) c++;
                if(curr_dir == directions::west) c--;
                if(terrain[r][c] == '#') break;
                direction way_back = directions::turn_around(curr_dir);
                auto it = std::find_if(possible_dirs[r][c].begin(), possible_dirs[r][c].end(), [&](direction dd){return dd != way_back;});
                if(it != possible_dirs[r][c].end()) curr_dir = *it;
                curr_dist++;
            }
        }
    }

    // for(size_t ni = 0; ni != nodes.size(); ni++)
    // {
    //     node & n = nodes[ni];
    //     printf("node %2zu {%2d,%2d}, trails", ni, n.pos.r, n.pos.c);
    //     for(size_t i = 0; i < n.next_node_idxs.size(); i++) printf(" {%2d,%2d;%2d}", nodes[n.next_node_idxs[i]].pos.r, nodes[n.next_node_idxs[i]].pos.c, n.next_dists[i]);
    //     printf("\n");
    // }

    int max_length = 0;
    std::vector<hike> hikes;
    hike & starthike = hikes.emplace_back();
    starthike.curr_node_idx = 0;
    starthike.length = 0;
    starthike.visited_nodes.resize(nodes.size(), false);
    while(!hikes.empty())
    {
        hike h = std::move(hikes.back());
        hikes.pop_back();
        if(h.curr_node_idx == final_node_idx)
        {
            max_length = std::max(max_length, h.length);
            continue;
        }
        h.visited_nodes[h.curr_node_idx] = true;
        node & n = nodes[h.curr_node_idx];
        for(size_t i = 0; i < n.next_node_idxs.size(); i++)
        {
            int nidx = n.next_node_idxs[i];
            if(h.visited_nodes[nidx]) continue;
            hike & nh = hikes.emplace_back(h);
            nh.length += n.next_dists[i];
            nh.curr_node_idx = nidx;
        }
    }

    printf("%d\n", max_length);

    return 0;
}
