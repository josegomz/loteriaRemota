// Servidor de la loteria remota
// Compilar: 
// Ejecutar: ./servidor
#include "funcionesServidor.h"
#define MAX_LINE 120
extern int errno;

int main()
{
  inicializarCartas();
   struct sockaddr_in lsock,fsock, sname;
   int s, ss;
   int len,i;
   char buf[MAX_LINE], resp[MAX_LINE];
   //Creación de socket
   if((s=socket(AF_INET,SOCK_STREAM,0)) < 0) {
      perror("SOCKET: ");
      exit(0);
   }
   lsock.sin_family = AF_INET;
   lsock.sin_port = htons(4400); /* puerto para dar el servicio */
   lsock.sin_addr.s_addr =0; /* direccion IP de mi maquina servidora */
   //Asignación de dirección local 
   if(bind(s,(struct sockaddr *)&lsock, sizeof(struct sockaddr_in)) < 0 ){
      perror("BIND: ");
      exit(1);
   }
   //La llamada al sistema listen()  
   if(listen(s,3)<0){
      perror("LISTEN: ");
      exit(1);
   }
    
   while(1){ 
      len = sizeof(struct sockaddr_in); /* &len: entra y sale el tamano del socket esperado */
      //La llamada al sistema accept() 
      if((ss=accept(s,(struct sockaddr *)&fsock, &len)) < 0){
         perror("ACCEPT: ");
         continue;
      }
      printf("Un cliente conectado\n");
      if (fork() == 0) {
         /* Aqui se ejecuta el proceso hijo */
         /* Cierra el socket incompleto */
         /* se dedica a atender la conexion con el socket completo */
         close(s);
         while(1){ // Transferencia de datos.
            if((len=recv(ss,buf,MAX_LINE-1,0))<=0){
               perror("RECV: "); /* Si len==0 entonces el cliente cerro la conexion */
               exit(1);
            }
            //Aqui imprimo la direccion IP del cliente y el puerto al que esta conectado
            printf("La dirección IP que tiene el cliente es: %s \n",inet_ntoa(fsock.sin_addr));
            
            printf("Cadena Recibida: %s\n",buf);
            if(strcmp(buf,"A16hg7Aez2m2E")==0){
               printf("Termina el servicio por decision del Cliente\n");
               close(ss); //Cerrar la conexión
               exit(0); /* el proceso hijo se mata */
            }
             if(strcmp(buf,"A1tC003FY2z3E")==0)
               {
                 printf("Contraseña Correcta\n");
                 strcpy(resp,"Contraseña Correcta");   
               }    
             else
                {
                 printf("Contraseña Incorrecta\n");
                 strcpy(resp,"Contraseña Incorrecta");
                }
             
            // Transferencia de datos. 
            if(send(ss,resp,strlen(resp),0) < len) /* responde al cliente */
               perror("SEND: ");
         } /*while */
      } /* if fork */
      else /* Aqui continua el proceso vigia para aceptar otra conexion */
         close(ss); /* el padre cierra el socket completo que dejo al hijo */
   } /*while*/
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
	
}
