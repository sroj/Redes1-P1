#ifndef PSERVER_H_INCLUDED
#define PSERVER_H_INCLUDED

int serve (int pLocal, int pRemoto, int timeout, const char hostname[]);

int processClientRequest(int p_remoto, int timeout, const char hostname[],
                         int client_socket_fd);

#endif // PSERVER_H_INCLUDED
