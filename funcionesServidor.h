//funciones del servidor y puede que del cliente
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <semaphore.h>

#define CARTAS 54
#define LENGTH_NAME 31
#define LENGTH_MSG 101
#define LENGTH_SEND 201

GtkApplication *app;
GtkWidget *window;
GtkWidget *contenedor;
GtkWidget *fondo;
GtkWidget *carta;
GtkWidget *tabla[6];
int posiciones_tabla[6][2]={{600,30},{830,30},{1060,30},{600,280},{830,280},{1060,280}};
GtkWidget *buttonIniciar;

//nodo para manejar al cliente
typedef struct ClienteNodo {
    int dato;
    struct ClienteNodo* atras;
    struct ClienteNodo* siguiente;
    char ip[16];
    char name[31];
} Clientes;

// variables globales
int server_sockfd = 0, client_sockfd = 0;
Clientes *root, *now;//estos serviran para identificar el primero y el ultimo

//variables para barajear las cartas
int loteriaOrdenada[CARTAS];
int loteriaRevolvida[CARTAS];
bool usado[CARTAS];
bool ganador = false;//para verificar si ya hay un ganador

int iniciar_juego =0;
int carta_actual =0;
sem_t semaforo;

void inicializarCartas();
void revolverCartas();
void imprimirCartas();
Clientes *nuevoNodo(int sockfd, char* ip);
void capturar_ctrl_c_y_exit(int sig);
void enviar_msg_a_todos(Clientes *np, char tmp_buffer[]);
void enviar_cartas_a_todos(int carta);
static void * repartir_cartas(void *arg);
void client_handler(void *p_client);
void cargar_servidor();
gboolean cargar_jugadores(gpointer data);
int plantilla[10][16] = {{1,2,3,4,10,11,12,13,19,20,21,22,28,29,30,31},
					{6,7,8,9,15,16,17,18,24,25,26,27,33,34,35,36},
					{2,3,4,5,7,8,9,10,12,13,14,15,17,18,19,20},
					{43,44,45,21,52,53,54,26,7,8,9,31,16,17,18,36},
					{22,23,24,25,27,28,29,30,32,33,34,35,37,38,39,40},
					{21,22,23,24,30,31,32,33,39,40,41,42,48,49,50,51},
					{25,26,27,41,34,35,36,46,43,44,45,51,52,53,54,32},
					{42,43,44,45,47,48,49,50,52,53,54,1,40,10,19,20},
					{41,42,37,38,50,51,46,47,5,6,1,2,14,15,10,11},
					{39,40,19,20,48,49,28,29,3,4,37,38,12,13,46,47}};




