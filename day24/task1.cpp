#include "../helpers/helpers.hpp"


template<typename T>
struct coord
{
    T x;
    T y;
    T z;
};

template<typename T>
struct particle
{
    coord<T> p;
    coord<T> v;
};

template<typename T, typename U>
coord<T> cast(const coord<U> & ci)
{
    coord<T> co;
    co.x = static_cast<U>(ci.x);
    co.y = static_cast<U>(ci.y);
    co.z = static_cast<U>(ci.z);
    return co;
}

template<typename T, typename U>
particle<T> cast(const particle<U> & pi)
{
    particle<T> po;
    po.p = cast<T>(pi.p);
    po.v = cast<T>(pi.v);
    return po;
}

template<typename T>
coord<T> move(const coord<T> & p, const coord<T> & v, T t)
{
    coord<T> ret;
    ret.x = p.x + t * v.x;
    ret.y = p.y + t * v.y;
    ret.z = p.z + t * v.z;
    return ret;
}



int main()
{
    std::vector<std::string> input = read_lines();

    std::vector<particle<long long>> particles_int(input.size());
    for(size_t i = 0; i < input.size(); i++)
    {
        const char * s = input[i].c_str();
        particle<long long> & p = particles_int[i];
        sscanf(s, "%lld, %lld, %lld @ %lld, %lld, %lld", &p.p.x, &p.p.y, &p.p.z, &p.v.x, &p.v.y, &p.v.z);
    }

    std::vector<particle<double>> particles_flt(particles_int.size());
    std::transform(particles_int.begin(), particles_int.end(), particles_flt.begin(), [](const particle<long long> & p){return cast<double>(p);});

    double area_begin = 200000000000000.0;
    double area_end = 400000000000000.0;

    long long collisions_inside = 0;
    for(size_t i = 0; i < particles_flt.size(); i++)
    {
        for(size_t j = i+1; j < particles_flt.size(); j++)
        {
            particle<double> & p1 = particles_flt[i];
            particle<double> & p2 = particles_flt[j];
            double dpx = p2.p.x - p1.p.x;
            double dpy = p2.p.y - p1.p.y;
            double det_A = - (p1.v.x * p2.v.y) + (p1.v.y * p2.v.x);
            if(det_A == 0) continue;
            double det_1 = - (dpx    * p2.v.y) + (dpy    * p2.v.x);
            double det_2 =   (p1.v.x * dpy)    - (p1.v.y * dpx);
            double t1 = det_1 / det_A;
            double t2 = det_2 / det_A;

            coord cp = move(p1.p, p1.v, t1);
            
            if(t1 >= 0 && t2 >= 0 && cp.x > area_begin && cp.x < area_end && cp.y > area_begin && cp.y < area_end)
            {
                collisions_inside++;
            }
        }
    }

    printf("%lld\n", collisions_inside);

    return 0;
}
