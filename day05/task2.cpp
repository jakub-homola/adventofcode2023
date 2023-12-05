#include <cstdio>
#include <vector>
#include <algorithm>



using uint = unsigned int;


struct mymap_section
{
    uint src_range_start;
    uint dst_range_start;
    uint length;
    uint src_range_end;
    uint dst_range_end;
};



struct mymap
{
    std::vector<mymap_section> sections_all;
    std::vector<mymap_section*> sections_sorted_src;
    std::vector<mymap_section*> sections_sorted_dst;
};



bool is_in_section(const mymap_section & section, uint num)
{
    return (num >= section.src_range_start && num < section.src_range_end);
}



uint map_section(const mymap_section & section, uint num)
{
    return section.dst_range_start + (num - section.src_range_start);
}



void sort_map(mymap & map)
{
    map.sections_sorted_src.resize(map.sections_all.size());
    map.sections_sorted_dst.resize(map.sections_all.size());
    std::transform(map.sections_all.begin(), map.sections_all.end(), map.sections_sorted_src.begin(), [](mymap_section & ms){ return &ms; });
    std::transform(map.sections_all.begin(), map.sections_all.end(), map.sections_sorted_dst.begin(), [](mymap_section & ms){ return &ms; });
    std::sort(map.sections_sorted_src.begin(), map.sections_sorted_src.end(), [](mymap_section * l, mymap_section * r){ return l->src_range_start < r->src_range_start; });
    std::sort(map.sections_sorted_dst.begin(), map.sections_sorted_dst.end(), [](mymap_section * l, mymap_section * r){ return l->dst_range_start < r->dst_range_start; });
}



uint do_map_one(const mymap & map, uint num)
{
    auto section_it = std::lower_bound(map.sections_sorted_src.begin(), map.sections_sorted_src.end(), num, [](mymap_section * s, uint n) { return s->src_range_end <= n; } );
    bool is_end = (section_it == map.sections_sorted_src.end());

    if(!is_end && is_in_section(**section_it, num)) return map_section(**section_it, num);
    else return num;
}



uint do_map_all(const std::vector<mymap> & maps, uint num)
{
    // printf("    %u", num);
    uint res = num;
    for(size_t i = 0; i < maps.size(); i++)
    {
        res = do_map_one(maps[i], res);
        // printf(" -> %u", res);
    }
    // printf("\n");
    return res;
}



void read_until_char(char ch, FILE * file = stdin)
{
    while(fgetc(file) != ch) ;
}



int main()
{
    std::vector<mymap> maps;
    std::vector<uint> seed_starts;
    std::vector<uint> seed_ends;

    read_until_char(':');
    while(true)
    {
        uint seed_start, length;
        int res = scanf("%u%u", &seed_start, &length);
        if(res <= 0) break;
        seed_starts.push_back(seed_start);
        seed_ends.push_back(seed_start + length);
    }

    while(true)
    {
        mymap currmap;
        read_until_char(':');

        while(true)
        {
            mymap_section section;
            int res = scanf("%u%u%u", &section.dst_range_start, &section.src_range_start, &section.length);
            if(res <= 0) break;
            section.dst_range_end = section.dst_range_start + section.length;
            section.src_range_end = section.src_range_start + section.length;
            currmap.sections_all.push_back(section);

        }

        sort_map(currmap);
        maps.push_back(std::move(currmap));

        if(feof(stdin)) break;
    }

    uint min_location_num = ~0;
    #pragma omp parallel for
    for(size_t i = 0; i < seed_starts.size(); i++)
    {
printf("Range %zu start\n", i);
        for(uint seed_num = seed_starts[i]; seed_num < seed_ends[i]; seed_num++)
        {
            uint location_num = do_map_all(maps, seed_num);
            #pragma omp critical
            min_location_num = std::min(min_location_num, location_num);
        }
printf("Range %zu finish\n", i);
    }

    printf("%u\n", min_location_num);

    return 0;
}
