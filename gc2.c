#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>
#include "parser.h"
#include "taylor.h"
// GLOBAL VALUES

// hola daniel cochino
struct Proc {
int ready; //1=ready ; 0=notReady
float currentPi;
int percentage;
}process[25];


void newProc(int arrayPos){ //ingresa nuevo proceso en 
//el arreglo de estructuras
process[arrayPos].ready = 1;
process[arrayPos].currentPi = 0;
process[arrayPos].percentage = 0;
}

static gboolean
fill (gpointer   user_data)
{
  GtkWidget *progressBar = user_data;


  //Get the current progress/
  gdouble fraction;
  gdouble fraction2;
  fraction = gtk_progress_bar_get_fraction (GTK_PROGRESS_BAR (progressBar));
  

  //Increase the bar by 10% each time this function is called/
  fraction += 0.1;
  


  //Fill in the bar with the new fraction/
  gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progressBar),fraction);
  gtk_progress_bar_set_show_text(GTK_PROGRESS_BAR(progressBar), 100);

  //Ensures that the fraction stays below 1.0/
  if (fraction < 10.0 & fraction2 <10.0) 
    return TRUE;
  
  return FALSE;
}

static void activateGTK(){
int i;
GtkWidget *window, *progressBar,*label, *grid; //declaración de variables
window = gtk_window_new(GTK_WINDOW_TOPLEVEL); //crea una ventana
gtk_window_set_default_size (GTK_WINDOW (window), 300, 40); //se le asigna un tamaño
gtk_window_set_title (GTK_WINDOW (window), "Estado actual del proceso");//un titulo
gdouble fraction = 0.0;
/* Here we construct the container that is going pack our buttons */
   grid = gtk_grid_new ();

  /* Pack the container in the window */
   gtk_container_add (GTK_CONTAINER (window), grid);

//señal que cierra el programa con la "X"  
g_signal_connect(window,"delete_event",G_CALLBACK(gtk_main_quit),NULL);
progressBar = gtk_progress_bar_new(); //declaración de las barras de progreso
gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progressBar),fraction);
gtk_progress_bar_set_show_text(GTK_PROGRESS_BAR(progressBar), 100);
gtk_container_add(GTK_CONTAINER(window),progressBar);
// creacion de procesos
label= gtk_label_new ("Procesos");
  gtk_grid_attach (GTK_GRID (grid), label, 0, 0, 1, 1);
label= gtk_label_new ("Porcentaje");
  gtk_grid_attach (GTK_GRID (grid), label, 1, 0, 1, 1);
label= gtk_label_new ("Valor estimado");
  gtk_grid_attach (GTK_GRID (grid), label, 3, 0, 1, 1);
gchar *num_label;
for(i = 1; i < thread_num; i++){
progressBar = gtk_progress_bar_new();
gtk_grid_attach (GTK_GRID (grid), progressBar, 1, i, 1, 1);// # columna, fila , cantidad total de columnas, cantidad total de filas
num_label = g_strdup_printf("Process %d", i);
  label= gtk_label_new (num_label);
  gtk_grid_attach (GTK_GRID (grid), label, 0, i, 1, 1);
g_timeout_add (500, fill, GTK_PROGRESS_BAR (progressBar));}
//gtk_fixed_put(GTK_FIXED(fixed), label, 10, 54);

gtk_container_set_border_width(GTK_CONTAINER(window), 30);
  //g_timeout_add (500, fill, GTK_PROGRESS_BAR (progressBar2));
   gtk_widget_show_all(window); //se muestran todos los widgets
gtk_main(); //inicia el loop de main()
}

int main (int argc, char **argv){
	input_parser(argv[1]);
	thread_num=thread_num+1;
    printf("Num_threads from file=%d\n", thread_num);
    printf("iteraciones=%d\n", iteraciones);
    printf("Modo from file=%d\n", modo);
	taylor(100);
    gtk_init(0, 0); //inicializa GTK
    activateGTK();

   
return 0;
}
