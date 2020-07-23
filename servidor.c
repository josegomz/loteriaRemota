// Servidor de la loteria remota
// Compilar: gcc -o servidor servidor.c funcionesServidor.h `pkg-config --libs --cflags gtk+-3.0` -pthread
// Ejecutar: ./servidor
#include "funcionesServidor.h"
#define MAX_LINE 120
extern int errno;
void activate (){
  window = gtk_window_new (GTK_WINDOW_TOPLEVEL); // Crea la ventana
  gtk_window_set_title (GTK_WINDOW (window), "ConexiÃ³n al servidor"); // Asigna Titulo a la ventana 
  gtk_window_set_default_size (GTK_WINDOW (window),1200,650); // El tamaÃ±o por defaul de la ventana
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER); // La posiciÃ³n en la pantalla de la ventana
  gtk_window_set_resizable(GTK_WINDOW(window),FALSE); // Para desabilitar la propiedad redimensionable

  contenedor = gtk_fixed_new(); // Crea un nuevo contenedor
  char link_carta[60];
  fondo = gtk_image_new_from_file("Recursos/fondo2.jpg"); 
  sprintf(link_carta,"Recursos/img/%d.jpg",loteriaRevolvida[carta_actual]);
  carta = gtk_image_new_from_file(link_carta);

  buttonIniciar =gtk_button_new_with_label("Iniciar Servidor"); // creamos botones
  gtk_widget_set_size_request(buttonIniciar,160,50);

  gtk_fixed_put(GTK_FIXED(contenedor),fondo,0,0);//se coloca el fondo al contenedor
  gtk_fixed_put(GTK_FIXED(contenedor),buttonIniciar,20,630);
  gtk_fixed_put(GTK_FIXED(contenedor),carta,50,30);
  //Agrega el contenedor a la ventana y la muestra.
  g_signal_connect_swapped (G_OBJECT (buttonIniciar), "clicked",G_CALLBACK (cargar_servidor),(gpointer) window);
  gtk_container_add (GTK_CONTAINER(window),contenedor);
  gtk_widget_show_all (window);
}


void cargar_servidor(){
    gtk_widget_set_sensitive(buttonIniciar,false);
    signal(SIGINT, capturar_ctrl_c_y_exit);
    // Create socket
    server_sockfd = socket(AF_INET , SOCK_STREAM , 0);
    if (server_sockfd == -1) {
        printf("Falla al crear el socket.");
        exit(EXIT_FAILURE);
    }
    // Socket information
    struct sockaddr_in server_info;
    int s_addrlen = sizeof(server_info);
    memset(&server_info, 0, s_addrlen);
    server_info.sin_family = PF_INET;
    server_info.sin_addr.s_addr = INADDR_ANY;
    server_info.sin_port = htons(8888);
    // Bind and Listen
    bind(server_sockfd, (struct sockaddr *)&server_info, s_addrlen);
    listen(server_sockfd, 6);//cuandos pueden agregarse a la lista de esper
    // Imprimir Ip del servidor
    getsockname(server_sockfd, (struct sockaddr*) &server_info, (socklen_t*) &s_addrlen);
    printf("Iniciar servidor en: %s:%d\n", inet_ntoa(server_info.sin_addr), ntohs(server_info.sin_port));
    // Initiciar listas ligadas de clientes
    root = nuevoNodo(server_sockfd, inet_ntoa(server_info.sin_addr));
    now = root;
    //agregar usuarios al 
    pthread_t hilo_cartas;
    if(pthread_create(&hilo_cartas,NULL,*repartir_cartas,NULL)){
        perror("Error al crear el hilo");
        exit(EXIT_FAILURE);
    }
    g_idle_add(cargar_jugadores,NULL);
}

gboolean cargar_jugadores(gpointer data){
  while(gtk_events_pending()) gtk_main_iteration();
  printf("Espeando jugadores ..\n");
  struct sockaddr_in client_info;
  int c_addrlen = sizeof(client_info);
  memset(&client_info, 0, c_addrlen);

    while (1) {
      while(gtk_events_pending()) gtk_main_iteration();
      if(usuarios>=2){
        iniciar_juego=1;
        continue;
      }
      client_sockfd = accept(server_sockfd, (struct sockaddr*) &client_info, (socklen_t*) &c_addrlen);//establece conexion con los clientes
        tabla[usuarios]=gtk_image_new_from_file("Recursos/tabla.jpg");
        gtk_fixed_put(GTK_FIXED(contenedor),tabla[usuarios],posiciones_tabla[usuarios][0],posiciones_tabla[usuarios][1]); 
        usuarios++;
        // Imprimie IP del cliente
        
        getpeername(client_sockfd, (struct sockaddr*) &client_info, (socklen_t*) &c_addrlen);
        //printf("Client %s:%d Bienvenido.\n", inet_ntoa(client_info.sin_addr), ntohs(client_info.sin_port));

        // Agrega el usuario a la lista de usuarios conectados
        Clientes *c = nuevoNodo(client_sockfd, inet_ntoa(client_info.sin_addr));
        c->atras = now;
        now->siguiente = c;
        now = c;

        pthread_t id;//declaramos un hilo por cada jugador
        if (pthread_create(&id, NULL, (void *)client_handler, (void *)c) != 0) {
            perror("Error al crear el hilo!\n");//error al crea rle hilo
            exit(EXIT_FAILURE);
        }

        
        gtk_widget_show_all(window);
    }
}

int main(int argc, char **argv)
{
  inicializarCartas();
  revolverCartas();
  //imprimirCartas();
  gtk_init(&argc, &argv);
  activate();
  gtk_widget_show(window);
  gtk_main();
  return 0;
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

//funcion que retorna un nuevo nodo
Clientes *nuevoNodo(int sockfd, char* ip) {
    Clientes *np = (Clientes *)malloc( sizeof(Clientes) );
    np->dato = sockfd;
    np->atras = NULL;
    np->siguiente = NULL;
    strncpy(np->ip, ip, 16);
    strncpy(np->name, "NULL", 5);
    return np;
}

//captura si el usuario se sale del sistema
void capturar_ctrl_c_y_exit(int sig) {
    Clientes *tmp;
    while (root != NULL) {
        printf("\nCerrar socketfd: %d\n", root->dato);
        close(root->dato); // close all socket include server_sockfd
        tmp = root;
        root = root->siguiente;
        free(tmp);
    }
    exit(EXIT_SUCCESS);
}

//funcion que envia un mensaje a todos
void enviar_msg_a_todos(Clientes *np, char tmp_buffer[]) {
    Clientes *tmp = root->siguiente;
    while (tmp != NULL) {
        if (np->dato != tmp->dato) { //excepto el cliente np
            printf("Enviar al cliente %d: \"%s\" \n", tmp->dato, tmp_buffer);
            send(tmp->dato, tmp_buffer, LENGTH_SEND, 0);
        }
        tmp = tmp->siguiente;
    }
}

//envia la carta que se esta haciendo presente
void enviar_cartas_a_todos(int carta){
	char carta_char[LENGTH_SEND];
	sprintf(carta_char,"%d",carta);
	Clientes *tmp = root->siguiente;
	while (tmp != NULL) {
            printf("Enviar carta a %d: \"%d\" \n", tmp->dato,carta);
            send(tmp->dato, carta_char, LENGTH_SEND, 0);
        tmp = tmp->siguiente;
    }
    sleep(5);
}

//hilo que se encarga de repartir las cartas 
static void * repartir_cartas(void *arg){
    while(carta_actual<CARTAS){
      while(gtk_events_pending()) gtk_main_iteration();
        if(iniciar_juego ==1 && ganador == false){
            //sem_wait(&semaforo);            
            enviar_cartas_a_todos(loteriaRevolvida[carta_actual]);
            gtk_widget_destroy(carta);
            char link_carta[60];
            sprintf(link_carta,"Recursos/img/%d.jpg",loteriaRevolvida[carta_actual+1]);
            carta = gtk_image_new_from_file(link_carta);
            printf("%s\n", link_carta);
            gtk_fixed_put(GTK_FIXED(contenedor),carta,50,30);
            gtk_widget_show_all(window);
            carta_actual++;
            //sem_post(&semaforo);   
        }
        else if(ganador==true){
            printf("Ya hay un ganador\n");
            exit(EXIT_SUCCESS);
        }
    }
    printf("Cartas repartidas\n");
}

//hilo que se encarga de la interaccion con cada cliente
void client_handler(void *p_client) {
    int leave_flag = 0;
    char nickname[LENGTH_NAME] = {};
    char recv_buffer[LENGTH_MSG] = {};//texto que recive
    char send_buffer[LENGTH_SEND] = {};//texto que ser¨¢ enviado a todos
    Clientes *np = (Clientes *)p_client;

    // Recibir el nomre del cliente
    if (recv(np->dato, nickname, LENGTH_NAME, 0) <= 0 || strlen(nickname) < 2 || strlen(nickname) >= LENGTH_NAME-1) {
        printf("%s No escribio el nombre.\n", np->ip);
        leave_flag = 1;
    } else {
        strncpy(np->name, nickname, LENGTH_NAME);
        printf("%s(%s)(%d) Bienvenido al juego.\n", np->name, np->ip, np->dato);
        sprintf(send_buffer, "%s(%s) Bienvenido al juego.", np->name, np->ip);//se almacena en send_buffer
        enviar_msg_a_todos(np, send_buffer);
    }
    char tablero_send[3];
    sprintf(tablero_send,"%d",tablero[usuarios]);
    send(np->dato,tablero_send,3,0);

    // Conversation
    while (1) {
        if (leave_flag) {
            break;
        }   
        int receive = recv(np->dato, recv_buffer, LENGTH_MSG, 0);
        if (receive > 0) {
            if (strlen(recv_buffer) == 0) {
                continue;
            }
            sprintf(send_buffer, "%s£º%s ", np->name, recv_buffer);
        } else if (receive == 0 || strcmp(recv_buffer, "exit") == 0) {
            printf("%s(%s)(%d) lSe salió\n", np->name, np->ip, np->dato);
            sprintf(send_buffer, "%s(%s) Se salió", np->name, np->ip);
            leave_flag = 1;
        } else {
            printf("Fatal Error: -1\n");
            leave_flag = 1;
        }
        if(strcmp(recv_buffer, "loteria") == 0){
            ganador=true;
            sprintf(send_buffer, "El ganador es %s ", np->name);
        }
        enviar_msg_a_todos(np, send_buffer);
        
    }

    // Remove Node
    close(np->dato);
    if (np == now) { // remove an edge node
        now = np->atras;
        now->siguiente = NULL;
    } else { // remove a middle node
        np->atras->siguiente = np->siguiente;
        np->siguiente->atras = np->atras;
    }
    free(np);
}
