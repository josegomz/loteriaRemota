//funciones del servidor y puede que del cliente
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

//variables para barajear las cartas
int loteriaOrdenada[54];
int loteriaRevolvida[54];
bool usado[54];

void inicializarCartas()
void revolverCartas();


