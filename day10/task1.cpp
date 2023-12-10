#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>



enum struct pipe : int
{
    empty = 0,
    n = 1,
    e = 2,
    s = 4,
    w = 8,
    start = 16
};

pipe operator|(pipe p1, pipe p2)
{
    return static_cast<pipe>(static_cast<int>(p1) | static_cast<int>(p2));
}

pipe operator&(pipe p1, pipe p2)
{
    return static_cast<pipe>(static_cast<int>(p1) & static_cast<int>(p2));
}

pipe operator~(pipe p)
{
    return static_cast<pipe>(~static_cast<int>(p));
}

pipe opposite(pipe p)
{
    if(p == pipe::n) return pipe::s;
    if(p == pipe::s) return pipe::n;
    if(p == pipe::e) return pipe::w;
    if(p == pipe::w) return pipe::e;
    return pipe::empty;
}

bool pc(pipe p1, pipe p2)
{
    return (p1 & p2) != pipe::empty;
}

char pipe_to_char(pipe p)
{
    if(p == pipe::start) return 'S';
    if(p == pipe::n) return 'n';
    if(p == pipe::s) return 's';
    if(p == pipe::e) return 'e';
    if(p == pipe::w) return 'w';
    if(p == pipe::empty) return '.';
    if( pc(p,pipe::n) &&  pc(p,pipe::e) && !pc(p,pipe::s) && !pc(p,pipe::w)) return 'L';
    if(!pc(p,pipe::n) &&  pc(p,pipe::e) &&  pc(p,pipe::s) && !pc(p,pipe::w)) return 'F';
    if(!pc(p,pipe::n) && !pc(p,pipe::e) &&  pc(p,pipe::s) &&  pc(p,pipe::w)) return '7';
    if( pc(p,pipe::n) && !pc(p,pipe::e) && !pc(p,pipe::s) &&  pc(p,pipe::w)) return 'J';
    if( pc(p,pipe::n) && !pc(p,pipe::e) &&  pc(p,pipe::s) && !pc(p,pipe::w)) return '|';
    if(!pc(p,pipe::n) &&  pc(p,pipe::e) && !pc(p,pipe::s) &&  pc(p,pipe::w)) return '-';
    return 'X';
}

pipe char_to_pipe(char c)
{
    if(c == 'L') return pipe::n | pipe::e;
    if(c == 'F') return pipe::e | pipe::s;
    if(c == '7') return pipe::s | pipe::w;
    if(c == 'J') return pipe::w | pipe::n;
    if(c == '|') return pipe::s | pipe::n;
    if(c == '-') return pipe::w | pipe::e;
    if(c == 'S') return pipe::start;
    return pipe::empty;
}



int main()
{
    std::vector<char> pipes_char;

    int ncols;
    int nrows = 0;
    while(true)
    {
        char line[10000];
        scanf("%s", line);
        if(feof(stdin)) break;
        ncols = strlen(line);
        if(nrows == 0) pipes_char.insert(pipes_char.end(), ncols+2, '.');
        nrows++;
        pipes_char.push_back('.');
        pipes_char.insert(pipes_char.end(), line, line + ncols);
        pipes_char.push_back('.');
    }
    pipes_char.insert(pipes_char.end(), ncols+2, '.');
    nrows += 2;
    ncols += 2;

    std::vector<pipe> pipes;
    pipes.resize(pipes_char.size());
    std::transform(pipes_char.begin(), pipes_char.end(), pipes.begin(), char_to_pipe);


    int start_r, start_c;
    pipe curr_direction;
    for(int r = 0; r < nrows; r++) for(int c = 0; c < ncols; c++)
    {
        if(pipes[r * ncols + c] == pipe::start)
        {
            pipe thispipe = pipe::empty;
            if(pc(pipes[(r+1) * ncols + c],pipe::n)) { thispipe = thispipe | pipe::s; curr_direction = pipe::s; };
            if(pc(pipes[(r-1) * ncols + c],pipe::s)) { thispipe = thispipe | pipe::n; curr_direction = pipe::n; };
            if(pc(pipes[r * ncols + (c+1)],pipe::w)) { thispipe = thispipe | pipe::e; curr_direction = pipe::e; };
            if(pc(pipes[r * ncols + (c-1)],pipe::e)) { thispipe = thispipe | pipe::w; curr_direction = pipe::w; };
            start_r = r;
            start_c = c;
            pipes[r * ncols + c] = thispipe;
            break;
        }
    }

    // for(int r = 0; r < nrows; r++)
    // {
    //     for(int c = 0; c < ncols; c++) printf("%c", pipe_to_char(pipes[r * ncols + c]));
    //     printf("\n");
    // }
    // printf("start %3d %3d   %c %c\n", start_r, start_c, pipe_to_char(pipes[start_r * ncols + start_c]), pipe_to_char(curr_direction));

    int loopsize = 0;
    int curr_r = start_r, curr_c = start_c;
    do
    {
        // printf("%3d %3d   %c %c  %d %d\n", curr_r, curr_c, pipe_to_char(pipes[curr_r * ncols + curr_c]), pipe_to_char(curr_direction), start_r, start_c);
        if(curr_direction == pipe::n) curr_r = curr_r - 1;
        if(curr_direction == pipe::s) curr_r = curr_r + 1;
        if(curr_direction == pipe::w) curr_c = curr_c - 1;
        if(curr_direction == pipe::e) curr_c = curr_c + 1;
        curr_direction = pipes[curr_r * ncols + curr_c] & ~opposite(curr_direction);
        loopsize++;
    } while(curr_r != start_r || curr_c != start_c);

    int nsteps = (loopsize - 1) / 2 + 1;
    printf("%d\n", nsteps);

    return 0;
}
