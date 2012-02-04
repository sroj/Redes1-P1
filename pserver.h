#ifndef PSERVER_H_INCLUDED
#define PSERVER_H_INCLUDED

int serve (int pLocal, int pRemoto, int timeout, const char hostname[]);

int processClientRequest(char in_buffer[], int p_remoto, int timeout,
                         const char hostname[]);

#endif // PSERVER_H_INCLUDED
