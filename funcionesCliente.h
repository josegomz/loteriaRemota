#include <gtk/gtk.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

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


void conectar();
void inicializarAllegro();
