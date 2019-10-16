#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <math.h>
#include <glib.h>
#include "parser.h"
#include "taylor.h"


//gcc main.c -o main `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0`



GThread *thread;


static gpointer activateGTK(){

	GtkWidget *window, *fixed, *label; //declaración de variables

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL); //crea una ventana
	gtk_window_set_default_size (GTK_WINDOW (window), 300, 20); //se le asigna un tamaño
	gtk_window_set_title (GTK_WINDOW (window), "Estado actual del proceso");//un titulo
	g_signal_connect(window,"delete_event",G_CALLBACK(gtk_main_quit),NULL);
	
	fixed = gtk_fixed_new();
	gtk_container_add(GTK_CONTAINER(window), fixed);

	gtk_container_set_border_width(GTK_CONTAINER(window), 30);
	
	label = gtk_label_new("P1");
	gtk_fixed_put(GTK_FIXED(fixed), label, 10, 0);
	label = gtk_label_new("P2");
	gtk_fixed_put(GTK_FIXED(fixed), label, 10, 54);
	
	gdouble fraction=0.5;
	gdouble fraction2=0.9;
	GtkWidget*progressBar[25];

	int y=0;
	
	for(int i=0;i<5;i++){
		GtkWidget*progressBar[i]; 

		progressBar[i] = gtk_progress_bar_new(); //declaración de las barras de progreso

		gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progressBar[i]),fraction);
		gtk_progress_bar_set_show_text(GTK_PROGRESS_BAR(progressBar[i]), 100);

		gtk_container_add(GTK_CONTAINER(window),progressBar[i]);

		gtk_fixed_put(GTK_FIXED(fixed), progressBar[i], 80, y); 
		y=y+30;

}	


	gtk_widget_show_all(window); //se muestran todos los widgets
	gtk_main(); //inicia el loop de main()
}




int main (int argc, char **argv){
input_parser(argv[1]);
printf("Num_threads from file=%d\n", thread_num);
printf("iteraciones=%d\n", iteraciones);
printf("Modo from file=%d\n", modo);
taylor(300);



gtk_init(0, 0); //inicializa GTK

activateGTK();


return 0;
}
