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

template<typename T>
coord<T> operator-(const coord<T> & c)
{
    coord<T> ret;
    ret.x = -c.x;
    ret.y = -c.y;
    ret.z = -c.z;
    return ret;
}

template<typename T>
coord<T> operator+(const coord<T> & l, const coord<T> & r)
{
    coord<T> ret;
    ret.x = l.x + r.x;
    ret.y = l.y + r.y;
    ret.z = l.z + r.z;
    return ret;
}

template<typename T>
coord<T> operator-(const coord<T> & l, const coord<T> & r)
{
    coord<T> ret;
    ret.x = l.x - r.x;
    ret.y = l.y - r.y;
    ret.z = l.z - r.z;
    return ret;
}

template<typename T>
T calc_error(const std::vector<particle<T>> & particles, const particle<T> & stone)
{
    double error = 0;
    for(auto part : particles)
    {
        coord<double> dp = stone.p - part.p;
        coord<double> dv = stone.v - part.v;
        double t = - (dp.x*dv.x + dp.y*dv.y + dp.z*dv.z) / (dv.x*dv.x + dv.y*dv.y + dv.z*dv.z);
        double ex = dp.x + dv.x * t;
        double ey = dp.y + dv.y * t;
        double ez = dp.z + dv.z * t;
        error += ex*ex + ey*ey + ez*ez;
    }
    return error;
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

    double max_pos = 0;
    double max_vel = 0;
    for(auto & p : particles_flt)
    {
        if(std::abs(p.p.x) > max_pos) max_pos = std::abs(p.p.x);
        if(std::abs(p.p.y) > max_pos) max_pos = std::abs(p.p.y);
        if(std::abs(p.p.z) > max_pos) max_pos = std::abs(p.p.z);
        if(std::abs(p.v.x) > max_vel) max_vel = std::abs(p.v.x);
        if(std::abs(p.v.y) > max_vel) max_vel = std::abs(p.v.y);
        if(std::abs(p.v.z) > max_vel) max_vel = std::abs(p.v.z);
    }

    particle<double> curr_stone = {coord<double>{0,0,0}, coord<double>{1,1,1}};
    double curr_err = calc_error(particles_flt, curr_stone);
    double heat = 1;
    double rel_error_to_converge = 1e-18;
    long long iters = 0;
    while(true)
    {
        iters++;
        particle<double> new_stone = curr_stone;
        int dir = rand();
        if((dir & 0b000001) != 0) new_stone.p.x += ((2.0 * rand()) / RAND_MAX - 1) * max_pos * heat;
        if((dir & 0b000010) != 0) new_stone.p.y += ((2.0 * rand()) / RAND_MAX - 1) * max_pos * heat;
        if((dir & 0b000100) != 0) new_stone.p.z += ((2.0 * rand()) / RAND_MAX - 1) * max_pos * heat;
        if((dir & 0b001000) != 0) new_stone.v.x += ((2.0 * rand()) / RAND_MAX - 1) * max_vel * heat;
        if((dir & 0b010000) != 0) new_stone.v.y += ((2.0 * rand()) / RAND_MAX - 1) * max_vel * heat;
        if((dir & 0b100000) != 0) new_stone.v.z += ((2.0 * rand()) / RAND_MAX - 1) * max_vel * heat;
        double new_err = calc_error(particles_flt, new_stone);
        double rel_err = new_err / max_pos / particles_flt.size();
        if(new_err <= curr_err)
        {
            curr_stone = new_stone;
            curr_err = new_err;
        }
        heat *= 0.999999;
        if(iters % 100000 == 0)
        {
            printf("iter %lld, heat %+12.3e, rel_err %+12.3e\n", iters, heat, rel_err);
            printf("  new:  error %+12.3e   %+12.3e %+12.3e %+12.3e   %+12.3e %+12.3e %+12.3e\n", new_err, new_stone.p.x, new_stone.p.y, new_stone.p.z, new_stone.v.x, new_stone.v.y, new_stone.v.z);
            printf("  curr: error %+12.3e   %+12.3e %+12.3e %+12.3e   %+12.3e %+12.3e %+12.3e\n", curr_err, curr_stone.p.x, curr_stone.p.y, curr_stone.p.z, curr_stone.v.x, curr_stone.v.y, curr_stone.v.z);
        }
        // std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if(rel_err < rel_error_to_converge) break;
    }

    // 24,13,10 -3,1,2
    long long rx = static_cast<long long>(std::round(curr_stone.p.x));
    long long ry = static_cast<long long>(std::round(curr_stone.p.y));
    long long rz = static_cast<long long>(std::round(curr_stone.p.z));
    long long result = rx + ry + rz;
    
    printf("iterations: %lld\n", iters);
    printf("error:      %+12.3e\n", curr_err);
    printf("position:   %+12.3e %+12.3e %+12.3e\n", curr_stone.p.x, curr_stone.p.y, curr_stone.p.z);
    printf("velocity:   %+12.3e %+12.3e %+12.3e\n", curr_stone.v.x, curr_stone.v.y, curr_stone.v.z);
    printf("\n");
    printf("result: %lld\n", result);
    // 1025127405449117

    return 0;
}
