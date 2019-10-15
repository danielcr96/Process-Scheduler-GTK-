#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>
#include "parser.h"
//#include "taylor.h"

// GLOBAL VALUES
int nPorc = 2;

struct Proc {
	int ready;			//1=ready ; 0=waiting
	float currentPi;
	int percentage;
}process[25];


void newProc(int arrayPos){			//ingresa nuevo proceso en 
									//el arreglo de estructuras
	process[arrayPos].ready = 1;
	process[arrayPos].currentPi = 0;
	process[arrayPos].percentage = 0;
}

static gboolean
fill (gpointer   user_data)
{
  GtkWidget *progressBar1 = user_data;
  GtkWidget *progressBar2 = user_data;

  //Get the current progress/
  gdouble fraction;
  gdouble fraction2;
  fraction = gtk_progress_bar_get_fraction (GTK_PROGRESS_BAR (progressBar1));
  fraction2 = gtk_progress_bar_get_fraction (GTK_PROGRESS_BAR (progressBar2));

  //Increase the bar by 10% each time this function is called/
  fraction += 0.1;
  fraction2 += 0.1;


  //Fill in the bar with the new fraction/
  gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (progressBar1), fraction);
  gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (progressBar2), fraction2);
  gtk_progress_bar_set_show_text(GTK_PROGRESS_BAR(progressBar1),100);
  //Ensures that the fraction stays below 1.0/
  if (fraction < 10.0 & fraction2 <10.0) 
    return TRUE;
  
  return FALSE;
}

static void activateGTK(){

	GtkWidget *window, *progressBar1, *progressBar2, *fixed, *label;	//declaración de variables
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);				//crea una ventana
	gtk_window_set_default_size (GTK_WINDOW (window), 300, 20);		//se le asigna un tamaño
	gtk_window_set_title (GTK_WINDOW (window), "Estado actual del proceso");//un titulo
	gdouble fraction = 0.0;
	gdouble fraction2 = 0.0;

	//señal que cierra el programa con la "X"	 
	g_signal_connect(window,"delete_event",G_CALLBACK(gtk_main_quit),NULL);	
	
	progressBar1 = gtk_progress_bar_new();					//declaración de las barras de progreso
	progressBar2 = gtk_progress_bar_new();
	
	fixed = gtk_fixed_new();						
	gtk_container_add(GTK_CONTAINER(window), fixed);

	gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progressBar1),fraction);
	gtk_progress_bar_set_show_text(GTK_PROGRESS_BAR(progressBar1), 100);
	
	gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progressBar2),fraction2);
	gtk_progress_bar_set_show_text(GTK_PROGRESS_BAR(progressBar2), 100);

	gtk_container_add(GTK_CONTAINER(window),progressBar1);
	gtk_container_add(GTK_CONTAINER(window),progressBar2);

	label = gtk_label_new("P1");
	gtk_fixed_put(GTK_FIXED(fixed), label, 10, 0);
	label = gtk_label_new("P2");
	gtk_fixed_put(GTK_FIXED(fixed), label, 10, 54);

	gtk_fixed_put(GTK_FIXED(fixed), progressBar1, 80, 0); 
	gtk_fixed_put(GTK_FIXED(fixed), progressBar2, 80, 55);

	gtk_container_set_border_width(GTK_CONTAINER(window), 30);
	
	g_timeout_add (500, fill, GTK_PROGRESS_BAR (progressBar1));
 	g_timeout_add (500, fill, GTK_PROGRESS_BAR (progressBar2));
  	gtk_widget_show_all(window);	//se muestran todos los widgets
	gtk_main();						//inicia el loop de main()
}

int main (int argc, char **argv){
	
	input_parser(argv[1]);
	printf("Num_threads from file=%d\n", thread_num);
    printf("iteraciones=%d\n", iteraciones);
	printf("Modo from file=%d\n", modo);	
	
	gtk_init(0, 0);			//inicializa GTK
	activateGTK();

	//taylor(100);
		
	return 0;
}
