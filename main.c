#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc,char* argv[]){
  char *hostname;
  int premoto, plocal, timeout;

  if(argc == 9){

  if (!strcmp(argv[1], "-h")) {
    strcpy(hostname, argv[2]);
      if (!strcmp(argv[3], "-p")) {
        premoto = atoi(argv[4]);
        if (!strcmp(argv[5], "-l")) {
          plocal = atoi(argv[6]);
          if (!strcmp(argv[7],"-t")) {
            timeout = atoi(argv[8]);
          }
        }
      }
    }
  } else {
    perror("Se recomienda ejecutar nuevamente con el siguiente patron\nsiesta  -h <host psensor> -p <puerto psensor> -l <puerto local> -t <tiempo de ciclo>\n");
    exit(1);
  }
  exit(0);
}
