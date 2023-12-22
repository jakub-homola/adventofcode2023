#include "../helpers/helpers.hpp"



struct brick
{
    int x1, x2;
    int y1, y2;
    int z1, z2;
    std::vector<brick*> supported_by;
    std::vector<brick*> supporting;
};

struct coord2d
{
    int x;
    int y;
    struct hasher
    {
        long long operator()(const coord2d & c) const
        {
            return ((long long)c.x << 32) | ((long long)c.y << 0);
        }
    };
    struct equaler
    {
        long long operator()(const coord2d & l, const coord2d & r) const
        {
            return (l.x == r.x && l.y == r.y);
        }
    };
};

bool is_between(int val, int lo, int hi)
{
    return val >= lo && val <= hi;
}


int main()
{
    std::vector<std::vector<char>> input;
    read_lines(input);

    std::vector<brick> bricks;
    bricks.reserve(input.size());
    for(std::vector<char> & line : input)
    {
        brick & b = bricks.emplace_back();
        sscanf(line.data(), "%d,%d,%d~%d,%d,%d", &b.x1, &b.y1, &b.z1, &b.x2, &b.y2, &b.z2);
    }

    for(brick & b : bricks)
    {
        if(b.x1 > b.x2) std::swap(b.x1, b.x2);
        if(b.y1 > b.y2) std::swap(b.y1, b.y2);
        if(b.z1 > b.z2) std::swap(b.z1, b.z2);
    }

    std::sort(bricks.begin(), bricks.end(), [](const brick & l, const brick & r){ return l.z1 < r.z1; });

    std::unordered_map<coord2d, int, coord2d::hasher, coord2d::equaler> heights;
    for(brick & b : bricks)
    {
        int max_curr_height = 0;
        for(int x = b.x1; x <= b.x2; x++) for(int y = b.y1; y <= b.y2; y++)
        {
            auto it = heights.find(coord2d{x,y});
            if(it != heights.end()) max_curr_height = std::max(max_curr_height, it->second);
        }
        int new_z1 = max_curr_height + 1;
        int dropby = b.z1 - new_z1;
        b.z1 -= dropby;
        b.z2 -= dropby;
        for(int x = b.x1; x <= b.x2; x++) for(int y = b.y1; y <= b.y2; y++) heights[coord2d{x,y}] = b.z2;
    }

    int max_z = std::max_element(bricks.begin(), bricks.end(), [](const brick & l, const brick & r){ return l.z2 < r.z2; })->z2;

    std::vector<std::vector<brick*>> bricks_at_z(max_z + 1);
    for(brick & b : bricks)
    {
        for(int z = b.z1; z <= b.z2; z++) bricks_at_z[z].push_back(&b);
    }

    for(int z = 1; z <= max_z; z++)
    {
        for(brick * bh : bricks_at_z[z])
        {
            for(brick * bl : bricks_at_z[z-1])
            {
                if(bh == bl) continue;
                bool overlap_x = is_between(bh->x1, bl->x1, bl->x2) || is_between(bh->x2, bl->x1, bl->x2) || is_between(bl->x1, bh->x1, bh->x2) || is_between(bl->x2, bh->x1, bh->x2);
                bool overlap_y = is_between(bh->y1, bl->y1, bl->y2) || is_between(bh->y2, bl->y1, bl->y2) || is_between(bl->y1, bh->y1, bh->y2) || is_between(bl->y2, bh->y1, bh->y2);
                if(overlap_x && overlap_y)
                {
                    bh->supported_by.push_back(bl);
                    bl->supporting.push_back(bh);
                }
            }
        }
    }
    
    // for(size_t i = 0; i < bricks.size(); i++)
    // {
    //     brick & b = bricks[i];
    //     printf("brick id=%3zu   X={%3d,%3d}   Y={%3d,%3d}   Z={%3d,%3d},   supported_by {", i, b.x1, b.x2, b.y1, b.y2, b.z1, b.z2);
    //     for(brick * bs : b.supported_by) printf("%zu,", bs - bricks.data());
    //     printf("},   supporting {");
    //     for(brick * bs : b.supporting) printf("%zu,", bs - bricks.data());
    //     printf("}\n");
    // }

    int n_removable = 0;
    for(brick & b : bricks)
    {
        if(std::all_of(b.supporting.begin(), b.supporting.end(), [](const brick * bs){ return bs->supported_by.size() > 1; }))
        {
            n_removable++;
        }
    }

    printf("%d\n", n_removable);

    return 0;
}
