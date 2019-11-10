# Process-Scheduler-GTK-
Several CPU-intensive processes are executed (calculation of pi by Taylor series) which are scheduled according to the specification of a .txt file and the process of calculating each task is displayed using process bars in a graphical interface created with GTK

#To compile:
gcc gc2.c -o gc2 `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0` -lm

#To Run:
./main input.txt
