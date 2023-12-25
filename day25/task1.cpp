#include "../helpers/helpers.hpp"



struct node
{
    const char * name;
    std::vector<char *> neighs_names;
    std::vector<node*> neighs;
};

struct edge
{
    node * n1;
    node * n2;
};

bool get_path(node * n1, node * n2, std::vector<node> & nodes, std::vector<edge> * path)
{
    std::deque<node*> tovisit;
    std::vector<bool> visited(nodes.size(), false);
    std::vector<node*> predecessors(nodes.size(), nullptr);
    tovisit.push_back(n1);
    visited[n1 - nodes.data()] = true;
    while(!tovisit.empty())
    {
        node & n = *tovisit.front();
        tovisit.pop_front();
        for(node * ne : n.neighs)
        {
            int ne_idx = ne - nodes.data();
            if(!visited[ne_idx])
            {
                visited[ne_idx] = true;
                tovisit.push_back(ne);
                predecessors[ne_idx] = &n;
                if(ne == n2) break;
            }
        }
    }

    if(predecessors[n2 - nodes.data()] == nullptr) return false;

    if(path != nullptr)
    {
        node * curr = n2;
        path->clear();
        while(true)
        {
            node * n2 = curr;
            curr = predecessors[curr - nodes.data()];
            if(curr == nullptr) break;
            node * n1 = curr;
            path->push_back({n1, n2});
        }
    }

    return true;
}


int main()
{
    std::vector<std::vector<char>> input;
    read_lines(input);
    std::vector<node> nodes;
    std::vector<edge> edges;

    for(std::vector<char> & line_vec : input)
    {
        node & n = nodes.emplace_back();
        char * line = line_vec.data();
        char * colon = strchr(line, ':');
        *colon = '\0';
        n.name = line;
        n.neighs_names = split_string(colon + 2, ' ');
    }

    for(node & n : nodes)
    {
        for(char * nn : n.neighs_names)
        {
            auto it = std::find_if(nodes.begin(), nodes.end(), [&](const node & nd){return strcmp(nd.name, nn) == 0;});
            int idx = it - nodes.begin();
            if(it == nodes.end()) nodes.emplace_back().name = nn;
            node * neigh = &nodes[idx];
            if(std::find(n.neighs.begin(), n.neighs.end(), neigh) == n.neighs.end()) n.neighs.push_back(&nodes[idx]);
            if(std::find(neigh->neighs.begin(), neigh->neighs.end(), &n) == neigh->neighs.end()) neigh->neighs.push_back(&n);
            if(std::find_if(edges.begin(), edges.end(), [&](const edge & e){ return (e.n1 == &n && e.n2 == neigh) || (e.n1 == neigh && e.n2 == &n);}) == edges.end()) edges.push_back({&n, neigh});
        }
    }

    // printf("nnodes=%zu nedges=%zu\n", nodes.size(), edges.size());

    node * s1;
    node * s2;
    for(edge & e1 : edges)
    {
        e1.n1->neighs.erase(std::find(e1.n1->neighs.begin(), e1.n1->neighs.end(), e1.n2));
        e1.n2->neighs.erase(std::find(e1.n2->neighs.begin(), e1.n2->neighs.end(), e1.n1));

        std::vector<edge> another_path_e1;
        bool exists_another_e1 = get_path(e1.n1, e1.n2, nodes, &another_path_e1);
        if(!exists_another_e1) printf("WEIRD: another e1 does not exist\n");
        for(edge & e2 : another_path_e1)
        {
            e2.n1->neighs.erase(std::find(e2.n1->neighs.begin(), e2.n1->neighs.end(), e2.n2));
            e2.n2->neighs.erase(std::find(e2.n2->neighs.begin(), e2.n2->neighs.end(), e2.n1));

            std::vector<edge> another_path_e2;
            bool exists_another_e2 = get_path(e2.n1, e2.n2, nodes, &another_path_e2);
            if(!exists_another_e2) printf("WEIRD: another e2 does not exist\n");
            for(edge & e3 : another_path_e2)
            {
                e3.n1->neighs.erase(std::find(e3.n1->neighs.begin(), e3.n1->neighs.end(), e3.n2));
                e3.n2->neighs.erase(std::find(e3.n2->neighs.begin(), e3.n2->neighs.end(), e3.n1));

                std::vector<edge> another_path_e3;
                bool exists_another_e3 = get_path(e3.n1, e3.n2, nodes, nullptr);
                if(!exists_another_e3)
                {
                    s1 = e3.n1;
                    s2 = e3.n2;
                    // printf("removed edges {%s,%s}, {%s,%s}, {%s,%s}\n", e1.n1->name, e1.n2->name, e2.n1->name, e2.n2->name, e3.n1->name, e3.n2->name);
                    goto theend;
                }

                e3.n1->neighs.push_back(e3.n2);
                e3.n2->neighs.push_back(e3.n1);
            }

            e2.n1->neighs.push_back(e2.n2);
            e2.n2->neighs.push_back(e2.n1);
        }

        e1.n1->neighs.push_back(e1.n2);
        e1.n2->neighs.push_back(e1.n1);
    }
    
    theend:
    int nnodes1 = 0;
    int nnodes2 = 0;
    for(int i = 0; i < 2; i++)
    {
        int & nnodes = ((i == 0) ? nnodes1 : nnodes2);
        node * s = ((i == 0) ? s1 : s2);

        std::vector<node *> tovisit;
        std::vector<bool> visited(nodes.size(), false);
        tovisit.push_back(s);
        visited[s - nodes.data()] = true;
        nnodes = 1;
        while(!tovisit.empty())
        {
            node & n = *tovisit.back();
            tovisit.pop_back();
            for(node * ne : n.neighs)
            {
                int ne_idx = ne - nodes.data();
                if(!visited[ne_idx])
                {
                    tovisit.push_back(ne);
                    visited[ne_idx] = true;
                    nnodes++;
                }
            }
        }
    }

    int result = nnodes1 * nnodes2;
    // printf("%d %d\n", nnodes1, nnodes2);
    printf("%d\n", result);

    return 0;
}
