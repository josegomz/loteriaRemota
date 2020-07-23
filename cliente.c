/*gcc -o cliente cliente.c funcionesCliente.h `pkg-config --libs --cflags gtk+-3.0` -lallegro_audio -lallegro_acodec -lallegro_main -lallegro
*/
#include "funcionesCliente.h"

static void activate (GtkApplication *app,gpointer        user_data){  
  window = gtk_application_window_new (app); // Crea la ventana
  gtk_window_set_title (GTK_WINDOW (window), "Conexión al servidor"); // Asigna Titulo a la ventana 
  gtk_window_set_default_size (GTK_WINDOW (window), 350,250); // El tamaño por defaul de la ventana
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER); // La posición en la pantalla de la ventana
  gtk_window_set_resizable(GTK_WINDOW(window),FALSE); // Para desabilitar la propiedad redimensionable
  contenedor = gtk_fixed_new(); // Crea un nuevo contenedor
  fondo = gtk_image_new_from_file("Recursos/fondo1b.jpg"); 
  gtk_widget_set_size_request(window,100,100); // definir el tamaño de la ventana
  /*Crea un Radio button inicial*/
  button1 = gtk_radio_button_new(NULL);
  group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (button1)); // obtenemos el grupo del primer radio
  /*Crea el segundo radio y se asigna al mismo grupo del primero*/
  button2 = gtk_radio_button_new(group);
  //text fields de los parametros
  ip=gtk_entry_new(); // se crean las entradas de texto
  gtk_widget_set_sensitive(ip,false);//desactivamos la IP
  gtk_entry_set_text(GTK_ENTRY(ip),LOCALHOST);
  puerto=gtk_entry_new();
  gtk_entry_set_text(GTK_ENTRY(puerto),PUERTO);
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
  g_signal_connect (GTK_TOGGLE_BUTTON (button1), "toggled", G_CALLBACK (on_rb_local), window);
  g_signal_connect (GTK_TOGGLE_BUTTON (button2), "toggled", G_CALLBACK (on_rb_remoto), window);
  g_signal_connect_swapped (G_OBJECT (btnConectar), "clicked",G_CALLBACK (conectar),(gpointer) window);

  //Agrega el contenedor a la ventana y la muestra.
  gtk_container_add (GTK_CONTAINER(window),contenedor);
  gtk_widget_show_all (window);
}



int main (int argc, char **argv){
  inicializarAllegro();
  int status;
  app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}

//funion que conecta con el servidor de la loteria
void conectar(){
    //conectamos
    char buf[MAX_LINE];
    struct sockaddr_in fsock, sname;
    struct hostent *hent;
    int s, len; 
    const gchar *host;
    const gchar *port;
    const gchar *user;
    int puerto_serv;
    host =gtk_entry_get_text(GTK_ENTRY(ip));
    port =gtk_entry_get_text(GTK_ENTRY(puerto));
    user = gtk_entry_get_text(GTK_ENTRY(njugador));
    puerto_serv = atoi(port);
    if(!(hent = gethostbyname(host))){//obtirne el host del servidor
        perror("GETHOSTBYNAME:  ");
        exit(0);
    }
    //creaci��n del socket
    if((s=socket(AF_INET,SOCK_STREAM,0))<0){
        perror("SOCKET:  ");
        exit(0);
    }
    fsock.sin_family =AF_INET;
    fsock.sin_addr.s_addr = *(long *) hent->h_addr;//direccion ip de la maquina remota
    fsock.sin_port = htons(puerto_serv);
    if(connect(s,(struct sockaddr *)&fsock, sizeof (struct sockaddr_in)) == -1){
        perror("CONNECT");
        close(s);
        exit(0);
    }
    //g_print ("Conectado al servidor: %l    \n",fsock.sin_addr.s_addr);
    send(s,user,LENGTH_NAME,0);
    recv(s,tablero,3,0);
  //mostrar cliente
    mostrarVentanaJuego();
  //al_play_sample(audiowin,1,0,1,ALLEGRO_PLAYMODE_ONCE,NULL);
}

void mostrarVentanaJuego(){
	gtk_widget_destroy(window);
  	window2 = gtk_application_window_new (app);
  	gtk_window_set_title (GTK_WINDOW (window2), "Loteria");
  	gtk_widget_set_size_request(window2,1200,650);//tamaño de la ventana creada
  	gtk_window_set_resizable(GTK_WINDOW(window2),FALSE);// esta linea deja estatica la ventana
  	gtk_window_set_position(GTK_WINDOW(window2), GTK_WIN_POS_CENTER);
  	contenedor = gtk_fixed_new(); // Crea un nuevo contenedor
  	fondo = gtk_image_new_from_file("Recursos/fondo2b.jpg");
	char link_tablero[60];
	sprintf(link_tablero,"Recursos/img/tablas/%s.jpg",tablero);
	tabla = gtk_image_new_from_file(link_tablero);
	carta = gtk_image_new_from_file("Recursos/img/1.jpg");
	btnloteria = gtk_button_new();
	gtk_widget_set_size_request(btnloteria,283,73);
	gtk_fixed_put(GTK_FIXED(contenedor),btnloteria,130,610);
  	construirTablero();
  	gtk_fixed_put(GTK_FIXED(contenedor),fondo,0,0);//se coloca el fondo al contenedor
  	gtk_fixed_put (GTK_FIXED(contenedor),tabla,595,23);
	gtk_fixed_put(GTK_FIXED(contenedor),carta,100,30);
  	gtk_container_add (GTK_CONTAINER(window2),contenedor);
  	gtk_widget_show_all(window2);// terminamo de usar la ventana
}


//inicializa allegro asi como sus componentes
void inicializarAllegro(){
	al_init();
	if (!al_install_audio()){//inicializa el audio
		fprintf(stderr, "failed to initialize audio!\n");
		exit(0);
	}
	if(!al_init_acodec_addon()){
        	fprintf(stderr, "failed to initialize audio codecs!\n");
        	exit(0);
	}
	if (!al_reserve_samples(1)){//reserva la cantidad de samples que pueden estar al mismo tiempo
	        fprintf(stderr, "failed to reserve samples!\n");
	        exit(0);
	}
	audioerror=al_load_sample("Recursos/sonido/error.ogg");
	audiowin=al_load_sample("Recursos/sonido/win.ogg");
}

void construirTablero(){
	int i, y = 30;
	for(i=0;i<4;i++){
		btnplanilla[i][0] = gtk_button_new();
		btnplanilla[i][1] = gtk_button_new();	
		btnplanilla[i][2] = gtk_button_new();	
		btnplanilla[i][3] = gtk_button_new();
		gtk_widget_set_size_request(btnplanilla[i][0],120,150);
		gtk_widget_set_size_request(btnplanilla[i][1],120,150);
		gtk_widget_set_size_request(btnplanilla[i][2],120,150);
		gtk_widget_set_size_request(btnplanilla[i][3],120,150);
		gtk_fixed_put (GTK_FIXED(contenedor),btnplanilla[i][0],600,y);
		gtk_fixed_put (GTK_FIXED(contenedor),btnplanilla[i][1],740,y);
		gtk_fixed_put (GTK_FIXED(contenedor),btnplanilla[i][2],880,y);
		gtk_fixed_put (GTK_FIXED(contenedor),btnplanilla[i][3],1020,y);
		y += 160;
	} 
  
}

// funcion cuando marquen local
static void on_rb_local (GtkWidget *button, gpointer user_data) {
    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(button))){
        g_print ("Local fue seleccionado \n");
        gtk_widget_set_sensitive(ip,false);
        gtk_entry_set_text(GTK_ENTRY(ip),LOCALHOST);
    }
}

// funcion cuando marquen local
static void on_rb_remoto (GtkWidget *button,gpointer user_data){
  if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(button))){
        g_print ("Remoto fue seleccionado \n");
        gtk_widget_set_sensitive(ip,true);
        gtk_entry_set_text(GTK_ENTRY(ip),"");
    }
}
