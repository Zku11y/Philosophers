#include <stdio.h>
#include <unistd.h>

int main()
{
    int i;

    i = 0;
    while(i < 2000)
    {
        i += 100;
        if(i > 2000)
            usleep(i - 2000 - 100);
        else
            usleep(100);
    }
        return 0;
}