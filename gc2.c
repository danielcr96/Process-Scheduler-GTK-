#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <glib.h>
#include "tay.h"
#include "parser.h"
#include <signal.h>
#include <sys/time.h>
#include <gdk/gdkkeysyms-compat.h>



// GLOBAL VALUES
GThread *ejecucion;
GtkWidget *window,*label, *grid; //declaración de variables
GMutex mutex;
gchar *pi;
int burst_T=0;
int currentProc;
bool setTimeOut = FALSE;
int sjfordenado=0;
int psordenado=0;
int mqsOrdenado=0;
int idTemp=0;
int temp[32];
int temp2[32];
int rand_num;
int numero=12;
int anteriorProc;
int seguro=1;


/////////////////////////////////////////////////////////////////////////////////////

gboolean
on_key_press (GtkWidget *widget, GdkEventKey *event, gpointer user_data);

/////////////////////////////////////////////////////////////////////////////////////


typedef struct nodoTicket{
	struct nodoTicket *sig;
	int idTicket;
	int idP;
}nodoTicket;

typedef struct ColaTicket{
	nodoTicket* primero;
	nodoTicket* ultimo;
}ColaTicket;

struct Proc {
	int complete; //1=completed ; 0=not completed ; 
	double currentPi;
	int currentIteration;
	double factorialAnterior;
	double fraction;
	double carga;
	int t_llegada;
	int prioridad,idProcess;
	GtkWidget *progressBar;
}Proc,process[30];


void newProc(int arrayPos){ //ingresa nuevo proceso en 
	//el arreglo de estructuras
	process[arrayPos].complete = 0;
	process[arrayPos].currentPi = 1.0;
	process[arrayPos].fraction = 0.0;
	process[arrayPos].currentIteration=1;
	process[arrayPos].factorialAnterior=1;
	process[arrayPos].carga = workload[arrayPos];
	process[arrayPos].t_llegada = arriveTime[arrayPos];
	process[arrayPos].prioridad = priority[arrayPos];
	process[arrayPos].idProcess=arrayPos;
}



/*static gboolean timeOut(gpointer data){
	setTimeOut = TRUE;
}*/




///////////timer/////////////

void sigalrm_handler(int sig){
   signal (sig, sigalrm_handler);
   printf("Este proyecto es sobrehumanamente cool\n");
	 printf("Numero presionado %d \n",numero);
   setTimeOut = TRUE;
}

void  timer(){
    signal(SIGALRM, sigalrm_handler);   //install the handler
	/* Set an alarm to go off in a little while. */
    struct itimerval it_val;  /* for setting itimer */
    it_val.it_value.tv_sec =    quantum/1000;
    it_val.it_value.tv_usec =    (quantum*1000) % 1000000;
    it_val.it_interval = it_val.it_value;
    setitimer(ITIMER_REAL, &it_val, NULL);
	
	//while (1);
}



///////////////////////

ColaTicket* crearCola(){ //crea cola de procesos
	struct ColaTicket* cola1 = malloc(sizeof(ColaTicket));
	cola1->primero=cola1->ultimo=NULL;
	return cola1;
}
struct nodoTicket* auxiliar2;
struct ColaTicket* auxiliarColaNodosTicket;

void push_ticket(int p_ID, int g_offset,ColaTicket* auxiliarColaNodosTicket){
//creción de nodo
	struct nodoTicket* _nodoTicket=malloc(sizeof(_nodoTicket));
	_nodoTicket->sig=NULL;
	_nodoTicket->idTicket=g_offset;
	_nodoTicket->idP=p_ID;
	
//encolar
	if (auxiliarColaNodosTicket->primero==NULL){
		auxiliarColaNodosTicket->primero=auxiliarColaNodosTicket->ultimo=_nodoTicket;
	}
	else{
		auxiliarColaNodosTicket->ultimo->sig=_nodoTicket;
		auxiliarColaNodosTicket->ultimo=_nodoTicket;
	}
}


void fillTickets(){			
	ColaTicket* colaNodosTicket=crearCola();
	auxiliarColaNodosTicket=colaNodosTicket;

	int globalTickets[tickets_num];			//se llena globalT {0,1,2,3,4,5,6...
	for(int i = 0; i<tickets_num; i++){
		globalTickets[i] = i;
	}
	int offset = 0;
	for(int p=0; p<process_num; p++){
		int nTickets = tickets[p];
		int g = 0;
		for(g; g < nTickets; g++){
			push_ticket(p, globalTickets[g+offset], auxiliarColaNodosTicket); //p=proceso, g+Offset = dato	
		}
		offset = g + offset;	
	}

	/*auxiliar2=auxiliarColaNodosTicket->primero;
	while(auxiliar2!=NULL){
		printf("Numero de proceso %d numero de ticket %d \n",auxiliar2->idP,auxiliar2->idTicket);
		auxiliar2=auxiliar2->sig;
	}*/

}



///////////////////////


static void activateGTK(){
	int i;
	
	
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL); //crea una ventana
	gtk_window_set_default_size (GTK_WINDOW (window), 500, 40); //se le asigna un tamaño
	gtk_window_set_title (GTK_WINDOW (window), "Estado actual del proceso");//un titulo
	g_signal_connect(window,"delete_event",G_CALLBACK(gtk_main_quit),NULL);	
	
  	grid = gtk_grid_new ();
   	gtk_container_add (GTK_CONTAINER (window), grid);
	
	//gdouble fraction = 0.5;


// Odenando procesos en el GRID

	label= gtk_label_new ("CALENDARIZADOR:");
  	gtk_grid_attach (GTK_GRID (grid), label, 0, -1, 1, 1);
	label= gtk_label_new ("Procesos");
  	gtk_grid_attach (GTK_GRID (grid), label, 0, 0, 1, 1);
	label= gtk_label_new ("Porcentaje");
  	gtk_grid_attach (GTK_GRID (grid), label, 1, 0, 1, 1);
	label= gtk_label_new ("Valor estimado");
  	gtk_grid_attach (GTK_GRID (grid), label, 2, 0, 1, 1);
	label= gtk_label_new ("Carga");
  	gtk_grid_attach (GTK_GRID (grid), label, 3, 0, 1, 1);
	label= gtk_label_new ("Prioridad");
  	gtk_grid_attach (GTK_GRID (grid), label, 4, 0, 1, 1);

	if(idcalendarizador == 0){
		label= gtk_label_new ("FCFS");
  		gtk_grid_attach (GTK_GRID (grid), label, 1, -1, 1, 1);
	}
	else if(idcalendarizador == 1){
		label= gtk_label_new ("SJF");
  		gtk_grid_attach (GTK_GRID (grid), label, 1, -1, 1, 1);
	}
	else if(idcalendarizador == 2){
		label= gtk_label_new ("RR");
  		gtk_grid_attach (GTK_GRID (grid), label, 1, -1, 1, 1);
	}
	else if(idcalendarizador == 3){
		label= gtk_label_new ("PS");
  		gtk_grid_attach (GTK_GRID (grid), label, 1, -1, 1, 1);
	}
	else if(idcalendarizador == 4){
		label= gtk_label_new ("PS_RR");
  		gtk_grid_attach (GTK_GRID (grid), label, 1, -1, 1, 1);
	}
	else if(idcalendarizador == 5){
		label= gtk_label_new ("MQS");
  		gtk_grid_attach (GTK_GRID (grid), label, 1, -1, 1, 1);
	}
	else if(idcalendarizador == 6){
		label= gtk_label_new ("MFQS");
  		gtk_grid_attach (GTK_GRID (grid), label, 1, -1, 1, 1);
	}
	else if(idcalendarizador == 7){
		label= gtk_label_new ("LOTTERY");
  		gtk_grid_attach (GTK_GRID (grid), label, 1, -1, 1, 1);
	}
	
	if(modo == 0){
		label= gtk_label_new ("MODO:  NO EXPROPIATIVO");
  		gtk_grid_attach (GTK_GRID (grid), label, 2, -1, 1, 1);
	}
	if(modo == 1){
		label= gtk_label_new ("MODO:  EXPROPIATIVO");
  		gtk_grid_attach (GTK_GRID (grid), label, 2, -1, 1, 1);
	}
	
	gchar *num_label;
	
// Ciclo que genera las barras y acomodandolas
	fillTickets();

	num_label = g_strdup_printf("QUANTUM:  %d",quantum);
	label= gtk_label_new (num_label);
	gtk_grid_attach (GTK_GRID (grid), label, 4, -1, 1, 1);

	for(int i = 0; i < process_num; i++){
		newProc(i);	 //nuevo proceso en el arreglo
		process[i].progressBar = gtk_progress_bar_new();
		gtk_grid_attach (GTK_GRID (grid), process[i].progressBar, 1, i+1, 1, 1);// # columna, fila , cantidad total de columnas, cantidad total de filas
		num_label = g_strdup_printf("P# %d",i);
	  	label= gtk_label_new (num_label);
	  	gtk_grid_attach (GTK_GRID (grid), label, 0, i+1, 1, 1);

		num_label = g_strdup_printf("%f", process[i].currentPi);
	  	label= gtk_label_new (num_label);
	  	gtk_grid_attach (GTK_GRID (grid), label, 2, i+1, 1, 1);

		num_label = g_strdup_printf("%0.0f", process[i].carga);
	  	label= gtk_label_new (num_label);
	  	gtk_grid_attach (GTK_GRID (grid), label, 3, i+1, 1, 1);
		
		num_label = g_strdup_printf("%d", process[i].prioridad);
	  	label= gtk_label_new (num_label);
	  	gtk_grid_attach (GTK_GRID (grid), label, 4, i+1, 1, 1);

		gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(process[i].progressBar), process[i].fraction);
		gtk_progress_bar_set_show_text(GTK_PROGRESS_BAR(process[i].progressBar), 100);
	}
	

	gtk_container_set_border_width(GTK_CONTAINER(window), 60);
   	gtk_widget_show_all(window); //se muestran todos los widgets
	//gtk_main(); //inicia el loop de main()
	
}
		//while(gtk_events_pending())
			//gtk_main_iteration();

static void calculaTaylor(int prosnum){
			process[prosnum].currentPi= 	taylor(&process[prosnum].currentIteration,process[prosnum].currentPi,&process[prosnum].factorialAnterior);
			
			printf("iteracion actual: %d ",process[prosnum].currentIteration);
			
			
			printf("calculo actual: %f\n",2*process[prosnum].currentPi);
			process[prosnum].fraction = (process[prosnum].currentIteration) / (process[prosnum].carga);

}
////////////////////////////////////////////////////FCFS///////////////////////
void fcfs(int proceso){	
	modo=0;	
	if(process[proceso].fraction >= 1){
		process[proceso].complete = 1;
		currentProc++;
	}	
}

///////////////////////////////////////////////////////
void ordenaCarga(){
	sjfordenado=1;
	int t;

	for(int i = 0 ; i < 32; i++){
		temp[i] = workload[i];
	}

  	for(int i = 0 ; i < ( process_num - 1 ); i++){
    	for(int j = 0 ; j < process_num - i - 1; j++){
      		if(temp[j]> temp[j+1]){
        		t = temp[j];
        		temp[j] = temp[j+1];
        		temp[j+1] = t;
      		}
    	}
  	}

}

//////////////////////////////////////////////////////SJF/////////////////////////////////////////

void sjf(){		//return la posición del valor mas pequeño
int aux; //auxiliar
inicioSJF:
		aux=currentProc+1;			
		idTemp++;// lo suma despues de completar un proceso, para la primera vez lo setea en 0


		if((modo==1) && (idTemp==process_num)){ // si no se ha llegado al final del array
			idTemp=0;                      // regrese a la primera posicion
		}


		if(sjfordenado==0){ //sino esta ordenado llama la funcion que lo ordena, solo se hace una vez
			ordenaCarga();
			idTemp=0; //inicia en la primera pos del temp
		}


		if(sjfordenado==0){ //sino esta ordenado llama la funcion que lo ordena, solo se hace una vez
		}

		else {
		
			
			if(temp[idTemp]==temp[idTemp-1]){//verifica sí algunas cargas son iguales, aqui actua cuando lo son
				for(aux;aux<process_num; aux++){
					if (temp[idTemp]==workload[aux]){
						currentProc = aux;	//actualiza valor del siguiente proceso
						break;
					}
				}
			}

			else if( temp[idTemp] != temp[idTemp-1]){//actua cuando las cargas son diferentes
				for(int p = 0 ; p < process_num; p++){
					if (temp[idTemp]==workload[p]){
						currentProc = p;	//actualiza valor del siguiente proceso
						break;}
				}
			
			}
		}


		if(process[currentProc].complete==1){

			goto inicioSJF;
		}
}

///////////////////////////////////////RR//////////////////////////////////////


void rr(){
modo=1;
iniciorr:
	idTemp++;
	if(process[0].fraction<=0){ //asegura que inicie en proceso 0
		idTemp=0;
	}

	if(idTemp==process_num){ //si llega al final el array, vuelve al inicio
		idTemp=0;
	}

	if(process[idTemp].complete==1){ //si el siguiente proceso esta listo entonces escoje otro
		goto iniciorr;		
	}
	currentProc=idTemp;
}

////////////////////////////////////////////////////////////////////////
void ordenaPrioridad(){
	psordenado=1;
	int t;

	for(int i = 0 ; i < 32; i++){
		temp[i] = priority[i];
	}

  	for(int i = 0 ; i < ( process_num - 1 ); i++){
    	for(int j = 0 ; j < process_num - i - 1; j++){
      		if(temp[j]> temp[j+1]){
        		t = temp[j];
        		temp[j] = temp[j+1];
        		temp[j+1] = t;
      		}
    	}
  	}


}

/////////////////////////PS//////////////////////////////////////////
void ps(int proceso){
		modo=0;
		int aux=proceso+1;
		idTemp++;// lo suma despues de completar un proceso, para la primera vez lo setea en 0
		if(psordenado==0){ //sino esta ordenado llama la funcion que lo ordena, solo se hace una vez
			ordenaPrioridad();
			idTemp=0; //inicia en la primera pos del temp
			}

		if(temp[idTemp]==temp[idTemp-1]){//verifica sí son iguales
			for(aux;aux<process_num; aux++){
				if (temp[idTemp]==priority[aux]){
					currentProc = aux;	//actualiza valor del siguiente proceso
					break;
				}
			}
		}

		else if( temp[idTemp] != temp[idTemp-1]){
			for(int p = 0 ; p < process_num; p++){
				if (temp[idTemp]==priority[p]){
					currentProc = p;	//actualiza valor del siguiente proceso
					break;
				}
			}
		}
}
///////////////////////////PS_RR/////////////////////////////////////
void ps_rr(){
modo=1;
int aux;
inicioPS:	
		aux=currentProc+1;	
		idTemp++;// lo suma despues de completar un proceso, para la primera vez lo setea en 0
		if(psordenado==0){ //sino esta ordenado llama la funcion que lo ordena, solo se hace una vez
			ordenaPrioridad();
			idTemp=0; //inicia en la primera pos del temp
			}

		if(modo==1 && idTemp==process_num){
			idTemp=0;
		}

		if(psordenado==0){ //sino esta ordenado llama la funcion que lo ordena, solo se hace una vez
			}




		else{
			if(temp[idTemp]==temp[idTemp-1]){//verifica sí son iguales
				for(aux;aux<process_num; aux++){
					if (temp[idTemp]==priority[aux]){
						currentProc = aux;	//actualiza valor del siguiente proceso
						break;
					}
				}
			}

			else if( temp[idTemp] != temp[idTemp-1]){
				for(int p = 0 ; p < process_num; p++){
					if (temp[idTemp]==priority[p]){
						currentProc = p;	//actualiza valor del siguiente proceso
						break;
					}
				}
			}
		}

		if(process[currentProc].complete == 1){

			goto inicioPS;}
}


////////////////////////////MQS//////////////////////////////////////////////////////


typedef struct nodoProceso{
	struct nodoProceso* siguiente;
	struct Proc _proceso;
}nodoProceso;


typedef struct nodoPrioridad{
	struct nodoPrioridad* siguiente;
	int numPrioridad;
	nodoProceso* nododeproceso;
	
}nodoPrioridad;


typedef struct ColaPrioridad {
	nodoPrioridad* primero;
	nodoPrioridad* ultimo;
}ColaPrioridad;

typedef struct ColaProceso {
	nodoProceso* primero;
	nodoProceso* ultimo;
}ColaProceso;


///cola1 cola de prioridad, cola2 cola de procesos
ColaPrioridad* crearColas(){ //crea cola prioridad
	struct ColaPrioridad* cola1 = malloc(sizeof(ColaPrioridad));
	cola1->primero=cola1->ultimo=NULL;
	return cola1;
}

///colas2 es colas de procesos
ColaProceso* crearColas2(){ //crea cola de procesos
	struct ColaProceso* cola1 = malloc(sizeof(ColaProceso));
	cola1->primero=cola1->ultimo=NULL;
	return cola1;
}


nodoProceso* crearNodoProceso(struct Proc procesonodo){ //funcion que crea un nodoProceso, recibe el proceso #.
	struct nodoProceso* _nodoProceso=malloc(sizeof(nodoProceso));
	_nodoProceso->siguiente=NULL;
	_nodoProceso->_proceso=procesonodo;
}

nodoPrioridad* crearNodoPrioridad(int proridadNUM){ //funcion que crea un nodo prioridad, 
	struct nodoPrioridad* _nodoPrioridad=malloc(sizeof(_nodoPrioridad));
	_nodoPrioridad->siguiente=NULL;
	_nodoPrioridad->numPrioridad=proridadNUM;
	_nodoPrioridad->nododeproceso=NULL;
}

nodoPrioridad* extraerNodoPriridad(nodoPrioridad* nodoCabeza, int numPrioridad_){ //recorre la cola de prioridades,busca y retorna el nodoPrioridad q quiero trabajar
	while(nodoCabeza!=NULL){
		if(nodoCabeza->numPrioridad==numPrioridad_){
			return nodoCabeza;
			
			break;
		}
		nodoCabeza=nodoCabeza->siguiente;
	
	}

}


void encolar(ColaPrioridad* cola,nodoPrioridad* _nodoPriodidad){ //funcion que agrega nodo al final de la cola


	if (!cola->primero){
		cola->primero=_nodoPriodidad;
		cola->ultimo=_nodoPriodidad;
	}

	else{
		cola->ultimo->siguiente=_nodoPriodidad;
		cola->ultimo=_nodoPriodidad;
				
	}
}


int procesoEncolar(int prioridadNumber,int ultimoEncontrado){//return de proceso por encolar
	ordenaPrioridad();
	

	if(temp[prioridadNumber] == temp[prioridadNumber-1]){
		for(ultimoEncontrado;ultimoEncontrado<process_num;ultimoEncontrado++){
			if(temp[prioridadNumber]==priority[ultimoEncontrado]){
				return ultimoEncontrado;
				break;			
			}		
		}
	}
	else{
		for(int w=0;w<process_num;w++){
			if(temp[prioridadNumber]==priority[w]){
				return w;
				break;
			}
		}
	}
}

struct ColaProceso* auxiliarCola2;
struct ColaPrioridad* auxiliarColaPrioridad;
struct nodoPrioridad* auxiliarNodoPrio;
struct nodoProceso* auxiliarNodoProceso;


void ordenaColasMQS(){
	mqsOrdenado=1; //cuando ordena todas las colas se pone en 1
	ordenaPrioridad();
	ColaPrioridad* colaPrincipal=crearColas();
	auxiliarColaPrioridad=colaPrincipal;
	int auxiliar;

/////////////////////////////////////////////////////creacion de cola de prioridad/////////////////////////////

	for(int z=1; z<process_num+1;z++){
		if(temp[z]!=temp[z-1]){
			nodoPrioridad* _nodoPrioridad=crearNodoPrioridad(temp[z-1]);
			encolar(colaPrincipal,_nodoPrioridad);
		}
	}

///////////////creacion de cola de procesos determinados por la prioridad ////////////////////////////////////

	for (int y=0;y<process_num;y++){

		if(y==0){
			auxiliar=procesoEncolar(y,0);//me dice cual es el siguiente proceso a encolar
			ColaProceso* colaSecundaria=crearColas2();//cola donde mete los nodos de procesos
			nodoProceso* _NodoProceso=crearNodoProceso(process[auxiliar]);
			colaSecundaria->primero=colaSecundaria->ultimo=_NodoProceso;
			colaPrincipal->primero->nododeproceso=colaSecundaria->primero; //cabeza de cola principal apunta a cabeza de la cola secundaria
			
			//printf("carga de el proceso con prioridad 1: %f \n ",colaSecundaria->primero->_proceso.carga);
			auxiliarCola2=colaSecundaria;
		}
		else{
			auxiliar=procesoEncolar(y,auxiliar+1);//me dice cual es el siguiente proceso a encolar
			nodoProceso* _NodoProceso=crearNodoProceso(process[auxiliar]);

			if(temp[y]==temp[y-1]){//si son iguales no crea nueva cola y lo guarda en la cola anterior				
				auxiliarCola2->ultimo->siguiente=_NodoProceso;
				auxiliarCola2->ultimo=_NodoProceso;
			}
			else{//si son diferente crea una nueva cola secundaria
				ColaProceso* colaSecundaria=crearColas2();//cola donde mete los nodos de procesos
				colaSecundaria->primero=colaSecundaria->ultimo=_NodoProceso;//el nuevo nodo de proceso lo asigna a la cabeza de una nueva cola secundaria
				//printf("prioridad del nuevo nodoproceso creado %d \n",_NodoProceso->_proceso.prioridad);				
				auxiliarNodoPrio=extraerNodoPriridad(colaPrincipal->primero,temp[y]);//da la direccion del nodoPrioridad con ese numero de prioridad
				//printf("Prioridad del nodoPrioridad encontrado: %d \n",auxiliarNodoPrio->numPrioridad);
				auxiliarNodoPrio->nododeproceso=colaSecundaria->primero;//nodo de prioridad siempre apunta a la cabeza de cola secundaria(cola deprocesos)
				auxiliarCola2=colaSecundaria;//este auxiliar guarda la direccion de la nueva cola secundaria creada porque no puedo volver a llamarla afuera
				}
		}
		
		//printf("Carga procesos igual prioridad %f \n",auxiliarCola2->ultimo->_proceso.carga);
		

	}
	//auxiliarNodoPrio=extraerNodoPriridad(colaPrincipal->primero,4);
	//printf("carga del proceso que senala nodoprioridad encontrado: %d \n",auxiliarNodoPrio->nododeproceso->_proceso.idProcess);
}

void MQS(){
	int siguientePrioridad;
	if(mqsOrdenado==0){
		ordenaColasMQS();// ordena por primera vez las colas de prioridades y procesos
		modo=0;
		siguientePrioridad=auxiliarColaPrioridad->primero->numPrioridad;
		auxiliarNodoPrio=extraerNodoPriridad(auxiliarColaPrioridad->primero,siguientePrioridad);
		auxiliarNodoProceso=auxiliarNodoPrio->nododeproceso;
		currentProc=auxiliarNodoProceso->_proceso.idProcess;
	}
	else{

		if(auxiliarNodoProceso->siguiente==NULL){
			siguientePrioridad=auxiliarNodoPrio->siguiente->numPrioridad;
			auxiliarNodoPrio=extraerNodoPriridad(auxiliarColaPrioridad->primero,siguientePrioridad);//cuando se termina la cola de proceso, pasamos al siguiente nodoPrioridad
			auxiliarNodoProceso=auxiliarNodoPrio->nododeproceso;
		}
		
		else{
			auxiliarNodoProceso=auxiliarNodoProceso->siguiente;
		}

		currentProc=auxiliarNodoProceso->_proceso.idProcess;
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////hice el auxiliar de colaProceso
////hice una funcion de extraer nodo de la cola de proceso

struct ColaProceso* auxiliarColaPrincipal;
struct ColaProceso* auxiliarColaSecundaria;
struct ColaProceso* auxiliarColafcfs;
int ordenaMFQS=0;
int cs=1;
int cp=0;
int cfcfs=1;

void encolarProceso(ColaProceso* cola,nodoProceso* _nodoProceso){ //funcion que agrega nodo al final de la cola
	if (!cola->primero){
		cola->primero=_nodoProceso;
		cola->ultimo=_nodoProceso;}
	else{
		cola->ultimo->siguiente=_nodoProceso;
		cola->ultimo=_nodoProceso;}
}

nodoProceso* extraerNodoProceso(ColaProceso* cola){
	if(cola->primero){	
		nodoProceso* nodo;
		nodo=cola->primero;
		cola->primero=cola->primero->siguiente;
		return nodo;}
	return NULL;
}


void MFQS(){
	if(ordenaMFQS==0){
		modo=1;
		ordenaMFQS=1;
		ColaProceso* colaPrincipal= crearColas2(); 	auxiliarColaPrincipal=colaPrincipal;  
		ColaProceso* colaSecundaria= crearColas2(); auxiliarColaSecundaria=colaSecundaria;
		ColaProceso* colafcfs= crearColas2();		auxiliarColafcfs=colafcfs;

		for(int x=0;x<process_num;x++){
			nodoProceso* _nodoProceso= crearNodoProceso(process[x]);
			encolarProceso(colaPrincipal,_nodoProceso);
		}
	}
	

	if(cs==0){
		quantum=3000;
		if(process[currentProc].complete == 0){//si el nodo no se completa en el quantum pasa a la cola secundaria
			encolarProceso(auxiliarColafcfs,auxiliarNodoProceso);
		}		
		
		auxiliarNodoProceso = extraerNodoProceso(auxiliarColaSecundaria);
		
		if(auxiliarNodoProceso==NULL){
			auxiliarNodoProceso = extraerNodoProceso(auxiliarColaSecundaria);	
			cp=1;
			cs=1;
			cfcfs=0;
		}
	}	
	
	if(cp==0){
		quantum=1500;
		if(process[currentProc].complete == 0){//si el nodo no se completa en el quantum pasa a la cola secundaria
			encolarProceso(auxiliarColaSecundaria,auxiliarNodoProceso);
		}

		auxiliarNodoProceso = extraerNodoProceso(auxiliarColaPrincipal);	
		
		if(auxiliarNodoProceso==NULL){
			auxiliarNodoProceso = extraerNodoProceso(auxiliarColaSecundaria);	
			cp=1;
			cs=0;
		}
	}


	if(cfcfs==0){
		modo=0;
		auxiliarNodoProceso = extraerNodoProceso(auxiliarColafcfs);
	}



	currentProc=auxiliarNodoProceso->_proceso.idProcess;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////LOTTERY////////////////////////////////////////

void lottery(){
modo=1;
inicio_lottery:

	rand_num = rand()%(tickets_num-1);
	auxiliar2=auxiliarColaNodosTicket->primero;
	while(auxiliar2!=NULL){
		if (rand_num == auxiliar2->idTicket){
			currentProc = auxiliar2->idP;
			break;
		}
		printf("Numero de proceso %d numero de ticket %d \n",auxiliar2->idP,auxiliar2->idTicket);
		auxiliar2=auxiliar2->sig;
	}
	if(process[currentProc].complete == 1){ goto inicio_lottery;}
	
}
////////////////////////////////////////////////////////////////////////////////////
bool done(){
	int sum = 0;
	for(int i = 0; i < process_num; i++){
		sum = sum + process[i].complete;
	}
	if(modo==0){ ////no expropiativo
		if(sum == process_num || idTemp==process_num){
			return true;}
	}	
	if(modo==1){
		if(sum == process_num){
			return true ;}
		else if(idTemp==process_num){
				idTemp=0;
				return false;}
	
	return false;
	}
}



static gpointer trabajo(){

sch_tag:
	setTimeOut=FALSE;
	if(idcalendarizador == 0){ fcfs(currentProc);}
	else if(idcalendarizador == 1){ sjf();}
	else if(idcalendarizador == 2){ rr();}
	else if(idcalendarizador == 3){ ps(currentProc);}
	else if(idcalendarizador == 4){ ps_rr();}
	else if(idcalendarizador == 5){ MQS();}
	else if(idcalendarizador == 6){ MFQS();}
	else if(idcalendarizador == 7){ lottery();}	
	timer();
tay_tag:
			calculaTaylor(currentProc);
		
			while(gtk_events_pending()){
				gtk_main_iteration();}	
			
			if(process[currentProc].fraction >= 1){
					process[currentProc].complete = 1;
				
				if(seguro==0){
					currentProc=anteriorProc;
					numero=15;
					seguro=1;
					
				}
				
			}

			if(done()){
				goto finish_tag;
			}
	
			if(numero<10){
				goto tay_tag;
			}			

			//expropiativo
			else if (modo == 1){
				if(setTimeOut==FALSE){
					goto tay_tag;
				}
				else { //aqui tengo que ver como detengo el clock
					setTimeOut=FALSE;
					goto sch_tag;
				}
				
				if(process[currentProc].complete == 1){
					setTimeOut=FALSE;
					goto sch_tag;
				}
				
			}
			//no expropiativo
			else{
				if(process[currentProc].complete == 0){
					goto tay_tag;
				}
				else if(process[currentProc].complete == 1){
					goto sch_tag;
				}
			}
finish_tag:
	printf("FIN \n");

}
static gboolean actualizaGTK(gpointer data){
	int j;
	g_mutex_lock(&mutex);
	for(j = 0; j<process_num; j++){
	//Updating the bar
		process[j].progressBar = gtk_grid_get_child_at (GTK_GRID(grid), 1, j+1);
        gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(process[j].progressBar), process[j].fraction);

	//Updating the pi label
		if(process[j].currentPi==1){
			pi = g_strdup_printf("~");
			label= gtk_grid_get_child_at (GTK_GRID(grid), 2, j+1);
			gtk_label_set_text(GTK_LABEL(label), pi);
		}
		else{	
			pi = g_strdup_printf("%lf", process[j].currentPi*2);
			label= gtk_grid_get_child_at (GTK_GRID(grid), 2, j+1);
			gtk_label_set_text(GTK_LABEL(label), pi);
		}

	}
	g_mutex_unlock(&mutex);	
	return TRUE;

}

int main (int argc, char **argv){

    input_parser(argv[1]);
	
    gtk_init(0, 0); //inicializa GTK
    
	

	activateGTK();
  	g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK (gtk_main_quit), NULL);
  	g_signal_connect (G_OBJECT (window), "key_press_event", G_CALLBACK (on_key_press), NULL);

	g_thread_new("ejecucion",trabajo,NULL);
	g_timeout_add(16,actualizaGTK,window);
	gtk_main();
	
	
	
return 0;
}



///////////////////////////interrupciones////////

void comprueba(){
			if(numero!=currentProc && numero<process_num ){
				if(seguro){
					anteriorProc=currentProc;
					currentProc=numero;
					seguro=0;}
			}
			else{
				numero=15;
			}

}


gboolean
on_key_press (GtkWidget *widget, GdkEventKey *event, gpointer user_data)
{
  switch (event->keyval)
  {
    case GDK_0:
		printf("key pressed: %s\n", "0");
		numero=0; 
		comprueba();
		
      break;

    case GDK_1:
      printf("key pressed: %s\n", "1");
		numero=1;
		comprueba();
      break;

    case GDK_2:
      printf("key pressed: %s\n", "2");
		numero=2;
		comprueba();
      break;

    case GDK_3:
      printf("key pressed: %s\n", "3");
		numero=3; 
		comprueba();     
		break;

    case GDK_4:
      printf("key pressed: %s\n", "4");
		numero=4;
		comprueba();
      break;

    case GDK_5:
      printf("key pressed: %s\n", "5");
		numero=5;
		comprueba();
      break;


    case GDK_6:
      printf("key pressed: %s\n", "6");
		numero=6;
		comprueba();
      break;

    case GDK_7:
      printf("key pressed: %s\n", "7");
		numero=7;
		comprueba();	
      break;

    case GDK_8:
      printf("key pressed: %s\n", "8");
		numero=8;
		comprueba();
      break;

    case GDK_9:
      printf("key pressed: %s\n", "9");
		numero=9;
		comprueba();
      break;


    default:
      return FALSE; 
  }

  return FALSE; 
}
