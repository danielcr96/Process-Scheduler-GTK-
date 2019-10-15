# Process-Scheduler-GTK-
Several CPU-intensive processes are executed (calculation of pi by Taylor series) which are scheduled according to the specification of a .txt file and the process of calculating each task is displayed using process bars in a graphical interface created with GTK

gcc main.c -o main `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0`

./main input.txt
