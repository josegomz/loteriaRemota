// Servidor de la loteria remota
// Compilar: gcc -o servidor servidor.c funcionesServidor.h `pkg-config --libs --cflags gtk+-3.0` 
// Ejecutar: ./servidor
#include "funcionesServidor.h"
#define MAX_LINE 120
extern int errno;

static void activate (GtkApplication *app,
          gpointer        user_data)
{
  window = gtk_application_window_new (app); // Crea la ventana
  gtk_window_set_title (GTK_WINDOW (window), "Conexión al servidor"); // Asigna Titulo a la ventana 
  gtk_window_set_default_size (GTK_WINDOW (window),1200,650); // El tamaño por defaul de la ventana
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER); // La posición en la pantalla de la ventana
  gtk_window_set_resizable(GTK_WINDOW(window),FALSE); // Para desabilitar la propiedad redimensionable
  contenedor = gtk_fixed_new(); // Crea un nuevo contenedor
  fondo = gtk_image_new_from_file("Recursos/fondo2.jpg"); 
  carta = gtk_image_new_from_file("Recursos/img/1.jpg");
  cartaC1 = gtk_image_new_from_file("Recursos/tabla.jpg");
  cartaC2 = gtk_image_new_from_file("Recursos/tabla.jpg");
  cartaC3 = gtk_image_new_from_file("Recursos/tabla.jpg");
  cartaC4 = gtk_image_new_from_file("Recursos/tabla.jpg");
  cartaC5 = gtk_image_new_from_file("Recursos/tabla.jpg");
  cartaC6 = gtk_image_new_from_file("Recursos/tabla.jpg");

  gtk_fixed_put(GTK_FIXED(contenedor),fondo,0,0);//se coloca el fondo al contenedor
  gtk_fixed_put(GTK_FIXED(contenedor),carta,50,30);
  gtk_fixed_put(GTK_FIXED(contenedor),cartaC1,600,30);
  gtk_fixed_put(GTK_FIXED(contenedor),cartaC2,830,30);
  gtk_fixed_put(GTK_FIXED(contenedor),cartaC3,1060,30);
  gtk_fixed_put(GTK_FIXED(contenedor),cartaC4,600,280);
  gtk_fixed_put(GTK_FIXED(contenedor),cartaC5,830,280);
  gtk_fixed_put(GTK_FIXED(contenedor),cartaC6,1060,280);
  
  

  //Agrega el contenedor a la ventana y la muestra.
  gtk_container_add (GTK_CONTAINER(window),contenedor);
  gtk_widget_show_all (window);
}

int main(int argc, char **argv)
{
  inicializarCartas();
  revolverCartas();
  imprimirCartas();
  int status;
  app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}

void inicializarCartas(){
  for (int i = 0; i < CARTAS; ++i)
  {
    loteriaOrdenada[i]=i+1;
    usado[i]=false;
  }
}

void revolverCartas(){
  int index=0;  
  srand(time(NULL)); 
  for (int i=0; i < CARTAS; i++){
    do{
      index = (rand() % CARTAS);
    }
    while (usado[index] == true);
    loteriaRevolvida[i] = loteriaOrdenada[index];
    usado[index]=true;
  }
}

void imprimirCartas(){
  for (int i = 0; i < CARTAS; ++i)
  {
    printf("%d \n",loteriaRevolvida[i]);
  }
}
