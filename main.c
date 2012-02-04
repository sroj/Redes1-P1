#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pserver.h"

int main(int argc,char* argv[])
{
    char hostname[1024];
    int p_remoto, p_local, timeout;
    int n = 0;

    if(argc == 9)
    {
        if (!strcmp(argv[1], "-h"))
        {
            strcpy(hostname, argv[2]);
            if (!strcmp(argv[3], "-p"))
            {
                p_remoto = atoi(argv[4]);
                if (!strcmp(argv[5], "-l"))
                {
                    p_local = atoi(argv[6]);
                    if (!strcmp(argv[7],"-t"))
                    {
                        timeout = atoi(argv[8]);
                    }
                }
            }
        }
        n = serve(p_local, p_remoto, timeout, hostname);
    }
    else
    {
        perror("Sintaxis correcta: psiesta -h <host psensor> -p <puerto psensor> -l <puerto local> -t <tiempo de ciclo>\n");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}
