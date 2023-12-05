#include <cstdio>
#include <vector>
#include <algorithm>
#include <limits>
#include <memory>



using myuint = unsigned long long;


struct mymap_section
{
    myuint src_range_start;
    myuint dst_range_start;
    myuint length;
    myuint src_range_end;
    myuint dst_range_end;
};



struct mymap
{
    std::vector<std::unique_ptr<mymap_section>> sections;
};



void print_map(const mymap & map)
{
    printf("Map of %zu sections:\n", map.sections.size());
    for(size_t i = 0; i < map.sections.size(); i++)
    {
        const mymap_section & s = *map.sections[i];
        printf("  %10llu - %10llu   ->   %10llu - %10llu   // %10llu\n", s.src_range_start, s.src_range_end, s.dst_range_start, s.dst_range_end, s.length);
    }
}



bool is_in_section(const mymap_section & section, myuint num)
{
    return (num >= section.src_range_start && num < section.src_range_end);
}



myuint map_section(const mymap_section & section, myuint num)
{
    return section.dst_range_start + (num - section.src_range_start);
}



void map_sort(mymap & map)
{
    std::sort(map.sections.begin(), map.sections.end(), [](std::unique_ptr<mymap_section> & l, std::unique_ptr<mymap_section> & r){ return l->src_range_start < r->src_range_start; });
}



void map_add_implicit_sections(mymap & map)
{
    myuint curr_num = 0;

    while(true)
    {
        auto section_it = std::lower_bound(map.sections.begin(), map.sections.end(), curr_num, [](const std::unique_ptr<mymap_section> & s, myuint n) { return s->src_range_end <= n; } );
        bool is_end = (section_it == map.sections.end());
        if(is_end)
        {
            const mymap_section & section = *map.sections.back();
            std::unique_ptr<mymap_section> new_section = std::make_unique<mymap_section>();
            new_section->src_range_start = section.src_range_end;
            new_section->src_range_end = std::numeric_limits<myuint>::max();
            new_section->dst_range_start = new_section->src_range_start;
            new_section->dst_range_end = new_section->src_range_end;
            new_section->length = new_section->src_range_end - new_section->src_range_start;
            map.sections.push_back(std::move(new_section));
            break;
        }

        const mymap_section & section = **section_it;
        if(!is_in_section(section, curr_num))
        {
            std::unique_ptr<mymap_section> new_section = std::make_unique<mymap_section>();
            new_section->src_range_start = curr_num;
            new_section->src_range_end = section.src_range_start;
            new_section->dst_range_start = new_section->src_range_start;
            new_section->dst_range_end = new_section->src_range_end;
            new_section->length = new_section->src_range_end - new_section->src_range_start;
            map.sections.insert(section_it, std::move(new_section));
        }
        curr_num = section.src_range_end;
    }
}



mymap merge_maps(const mymap & msrc, const mymap & mdst)
{
    mymap result;
    myuint curr_num = 0;

    while(curr_num < std::numeric_limits<myuint>::max())
    {
        auto section_src_it = std::lower_bound(msrc.sections.begin(), msrc.sections.end(), curr_num, [](const std::unique_ptr<mymap_section> & s, myuint n) { return s->src_range_end <= n; } );
        const mymap_section & section_src = **section_src_it;
        myuint offset_src = curr_num - section_src.src_range_start;
        myuint mapped1_num = section_src.dst_range_start + offset_src;

        auto section_dst_it = std::lower_bound(mdst.sections.begin(), mdst.sections.end(), mapped1_num, [](const std::unique_ptr<mymap_section> & s, myuint n) { return s->src_range_end <= n; } );
        const mymap_section & section_dst = **section_dst_it;
        myuint offset_dst = mapped1_num - section_dst.src_range_start;
        myuint mapped2_num = section_dst.dst_range_start + offset_dst;

        std::unique_ptr<mymap_section> new_section = std::make_unique<mymap_section>();
        myuint length = std::min(section_src.length - offset_src, section_dst.length - offset_dst);
        new_section->length = length;
        new_section->src_range_start = curr_num;
        new_section->src_range_end = new_section->src_range_start + new_section->length;
        new_section->dst_range_start = mapped2_num;
        new_section->dst_range_end = new_section->dst_range_start + new_section->length;
        result.sections.push_back(std::move(new_section));

        curr_num += length;
    }

    return result;
}



void discard_higher(mymap & map, myuint upperbound)
{
    for(size_t i = 0; i < map.sections.size(); i++)
    {
        mymap_section & s = *map.sections[i];
        if(s.src_range_end > upperbound)
        {
            myuint shortened_length = upperbound - s.src_range_start;

            s.length = shortened_length;
            s.src_range_end = s.src_range_start + s.length;
            s.dst_range_end = s.dst_range_start + s.length;
            
            map.sections.resize(i+2);
            if(i+1 < map.sections.size())
            {
                mymap_section & s = *map.sections[i+1];
                s.src_range_start = upperbound;
                s.src_range_end = std::numeric_limits<myuint>::max();
                s.dst_range_start = s.src_range_start;
                s.dst_range_end = s.src_range_end;
                s.length = s.src_range_end - s.src_range_start;
            }

            return;
        }
    }
}



void read_until_char(char ch, FILE * file = stdin)
{
    while(fgetc(file) != ch) ;
}



int main()
{
    mymap allmap;
    myuint nseeds = 0;

    read_until_char(':');
    while(true)
    {
        myuint seed_start, length;
        int res = scanf("%llu%llu", &seed_start, &length);
        if(res <= 0) break;
        std::unique_ptr<mymap_section> new_section = std::make_unique<mymap_section>();
        new_section->src_range_start = nseeds;
        new_section->src_range_end = new_section->src_range_start + length;
        new_section->dst_range_start = seed_start;
        new_section->dst_range_end = new_section->dst_range_start + length;
        new_section->length = length;
        allmap.sections.push_back(std::move(new_section));
        nseeds += length;
    }
    map_sort(allmap);
    map_add_implicit_sections(allmap);

    while(true)
    {
        mymap currmap;
        read_until_char(':');

        while(true)
        {
            std::unique_ptr<mymap_section> new_section = std::make_unique<mymap_section>();
            int res = scanf("%llu%llu%llu", &new_section->dst_range_start, &new_section->src_range_start, &new_section->length);
            if(res <= 0) break;
            new_section->dst_range_end = new_section->dst_range_start + new_section->length;
            new_section->src_range_end = new_section->src_range_start + new_section->length;
            currmap.sections.push_back(std::move(new_section));
        }

        map_sort(currmap);
        map_add_implicit_sections(currmap);
        mymap newmap = merge_maps(allmap, currmap);
        discard_higher(newmap, nseeds);
        std::swap(allmap, newmap);

        if(feof(stdin)) break;
    }

    // print_map(allmap);

    myuint min_location_num = ~0;
    for(size_t i = 0; i < allmap.sections.size(); i++)
    {
        if(allmap.sections[i]->src_range_start >= nseeds) break;
        myuint loc_num = allmap.sections[i]->dst_range_start;
        min_location_num = std::min(min_location_num, loc_num);
    }

    printf("%llu\n", min_location_num);

    return 0;
}
