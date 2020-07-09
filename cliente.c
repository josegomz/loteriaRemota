#include "funcionesCliente.h"


void conectar()
{
  gtk_widget_destroy(window);
  window2 = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window2), "Juego");
  gtk_widget_set_size_request(window2,1300,680);//tamaño de la ventana creada
  gtk_window_set_resizable(GTK_WINDOW(window2),FALSE);// esta linea deja estatica la ventana
  gtk_window_set_position(GTK_WINDOW(window2), GTK_WIN_POS_CENTER);
  gtk_widget_show_all(window2);// terminamo de usar la ventana
}

/*Cuando se activa un Radio Button*/
static void button_toggled_cb (GtkWidget *button,
                   gpointer   user_data)
{
  char *b_state;
  const char *button_label;

  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (button)))
          b_state = "on";
  else {
          b_state = "off";
          g_print ("\n");
  }

  button_label = gtk_button_get_label (GTK_BUTTON (button));

  g_print ("%s was turned %s\n", button_label, b_state);
}

// funcion cuando marquen local
static void on_rb_local (GtkWidget *button, gpointer user_data) 
{
  g_print ("Local fue seleccionado \n");
}

// funcion cuando marquen local
static void on_rb_remoto (GtkWidget *button,gpointer user_data) 
{
  g_print ("Remoto fue seleccionado \n");
}

static void activate (GtkApplication *app,
          gpointer        user_data)
{


  window = gtk_application_window_new (app); // Crea la ventana
  gtk_window_set_title (GTK_WINDOW (window), "Conexión al servidor"); // Asigna Titulo a la ventana 
  gtk_window_set_default_size (GTK_WINDOW (window), 350,250); // El tamaño por defaul de la ventana
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER); // La posición en la pantalla de la ventana
  gtk_window_set_resizable(GTK_WINDOW(window),FALSE); // Para desabilitar la propiedad redimensionable
  contenedor = gtk_fixed_new(); // Crea un nuevo contenedor
  fondo = gtk_image_new_from_file("fondo1b.jpg"); 
  gtk_widget_set_size_request(window,100,100); // definir el tamaño de la ventana

  /*Crea un Radio button inicial*/
  button1 = gtk_radio_button_new(NULL);

  group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (button1)); // obtenemos el grupo del primer radio
   
  /*Crea el segundo radio y se asigna al mismo grupo del primero*/
  button2 = gtk_radio_button_new(group);
                                                         
  //Crear el titulo dentro de la ventana
  titulo = gtk_label_new("<big>Conectarse al servidor</big>");
  gtk_label_set_use_markup (GTK_LABEL (titulo), TRUE);

  ip=gtk_entry_new(); // se crean las entradas de texto
  puerto=gtk_entry_new();
  njugador=gtk_entry_new();
  btnConectar = gtk_button_new_with_label("Entrar"); // creamos botones
  /* Asignamos tamaños */
  gtk_widget_set_size_request(ip, 240,45);
  gtk_widget_set_size_request(puerto, 150,45);
  gtk_widget_set_size_request(njugador, 350,45);
  gtk_widget_set_size_request(btnConectar,160,65);


  contInfoRe = gtk_fixed_new(); // se crea el Contenedor que va a contener los elementos del servidor remoto
  
  gtk_fixed_put(GTK_FIXED(contenedor), btnConectar,190,550); 
  gtk_fixed_put(GTK_FIXED(contenedor),fondo,0,0);//se coloca el fondo al contenedor

  /* Agregamos los elementos a los contenedores correspondientes en posiciones expecificas*/
  gtk_fixed_put(GTK_FIXED(contenedor),button1, 150,150);
  gtk_fixed_put(GTK_FIXED(contenedor),button2, 340,150);
  gtk_fixed_put(GTK_FIXED(contInfoRe), ip,90,190); // agrega el elemento a el contenedor que contiene la información del servidor remoto
  gtk_fixed_put(GTK_FIXED(contInfoRe), puerto,150,307);
  gtk_fixed_put(GTK_FIXED(contInfoRe), njugador,175,425);
  gtk_fixed_put(GTK_FIXED(contenedor),contInfoRe,0,0); //agregamos el contenedor contInfo a el contenedor "contenedor"
  
  /* Se setea el valor inicial de los radios button*/
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (button2), FALSE);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (button1), FALSE);

  /*manejadores de señal (si un radio es activado llama a una función)*/
  g_signal_connect (GTK_TOGGLE_BUTTON (button1), "toggled", 
                    G_CALLBACK (button_toggled_cb), window);
  g_signal_connect (GTK_TOGGLE_BUTTON (button2), "toggled", 
                    G_CALLBACK (button_toggled_cb), window);


  g_signal_connect_swapped (G_OBJECT (btnConectar), "clicked",G_CALLBACK (conectar),(gpointer) window);

  //Agrega el contenedor a la ventana y la muestra.
  gtk_container_add (GTK_CONTAINER(window),contenedor);
  gtk_widget_show_all (window);
}



int main (int argc, char **argv)
{
  int status;

  app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}