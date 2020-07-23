#include <gtk/gtk.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <sys/errno.h>
#include <netdb.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#define LOCALHOST "localhost"
#define PUERTO "8888"
#define MAX_LINE 120
  GtkApplication *app;
    //Elementos de la pantalla
  GtkWidget *grid;
  GtkWidget *window;
  GtkWidget *window2;
  GtkWidget *contenedor;
  GtkWidget *contInfoRe;
  GtkWidget *fondo;
  GtkWidget *titulo;
  GtkWidget *ip;
  GtkWidget *puerto;
  GtkWidget *njugador;
  GtkWidget *btnConectar;

  GtkWidget *button1;
  GtkWidget *button2;
  GSList *group;
   
 // Tabla y Botones 
  GtkWidget *tabla;
  GtkWidget *btnplanilla[4][4];
  GtkWidget *carta;
  GtkWidget *btnloteria;

ALLEGRO_SAMPLE *audiocarta =NULL;
ALLEGRO_SAMPLE *audioerror =NULL;
ALLEGRO_SAMPLE *audiowin =NULL;
ALLEGRO_SAMPLE *audioclic =NULL;

#define LENGTH_NAME 31
#define LENGTH_MSG 101
#define LENGTH_SEND 201

int count_beans = 0; 
char tablero[3];
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

//estructura del paquete


void conectar();
void inicializarAllegro();
void mostrarVentanaJuego();
void construirTablero();
static void on_rb_local (GtkWidget *button, gpointer user_data);
static void on_rb_remoto (GtkWidget *button,gpointer user_data);
