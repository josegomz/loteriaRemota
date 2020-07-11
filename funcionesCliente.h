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

#define LOCALHOST "localhost"
#define PUERTO "4400"
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

ALLEGRO_SAMPLE *audiocarta =NULL;
ALLEGRO_SAMPLE *audioerror =NULL;
ALLEGRO_SAMPLE *audiowin =NULL;
ALLEGRO_SAMPLE *audioclic =NULL;

//estructura del paquete


void conectar();
void inicializarAllegro();
static void on_rb_local (GtkWidget *button, gpointer user_data);
static void on_rb_remoto (GtkWidget *button,gpointer user_data);