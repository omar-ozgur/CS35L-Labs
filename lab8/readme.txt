Name: Omar Ozgur
ID: 704465898
Date: 02/26/2016
Lab Section: 3
TA: Lauren Samy

*** Assignment 8 ***

README:

The goal of this assignment was to modify a ray-tracer program in order to
use multithreading capabilities provided by the "pthread.h" library. By using
functions such as "pthread_create()" and "pthread_join()" to manage a specified
number of threads, portions of the program could be run in parallel.

Inititially, I had trouble deciding how to split the program amongst a given
number of threads. After analyzing the program, I decided that the best way
to go about this was to allow each thread to calculate the colors of pixels
at a specific set of columns. In order to do this, an array of "pthread_t"
structs was dynamically allocated, and the "pthread_create()" function was
used to create each thread. These threads were made to invoke a common function
that allowed for columns of pixel colors to be calculated based on the given
thread ID number.

Another issue was due to the fact that race conditions between the threads
caused the color values of certain pixels to be printed before the color values
of other pixels. In order to fix this problem, a 3-dimensional float array
was used to store the three "scale_color" values for each pixel. After using
the "pthread_join()" function to wait for all threads to finish calculations,
the stored color values were printed in the correct order.

After modifying the ray-tracer code to utilize multithreading, the
"make clean check" command was run in order to test the speed and accuracy
of the output with 1, 2, 4, and 8 threads. The output of this command was
saved in the file "make-log.txt". The time that it took for the program
to run with each specified number of threads is shown below.

	(Unmodified program)
	real	0m51.937s
	user	0m51.923s
	sys	0m0.001s

	(1 Thread)
	real	0m54.262s
	user	0m54.247s
	sys	0m0.009s

	(2 Threads)
	real	0m27.420s
	user	0m54.034s
	sys	0m0.001s

	(4 Threads)
	real	0m17.174s
	user	0m59.268s
	sys	0m0.006s

	(8 Threads)
	real	0m7.274s
	user	0m56.109s
	sys	0m0.008s

As could be seen, the unmodified program ran in approximately the same
amount of time as the multithreaded program when 1 thread was used. This
was due to the fact that using 1 thread did not take advantage of the
multithreading capabilities of the program, since none of the components
of the program were run in parallel.

By comparing the times that it took for the multithreaded program to run
with 1, 2, 4, and 8 threads, it could be seen that increasing the number
of threads generally caused a noticeable increase in the speed of the
program. This was due to the fact that larger numbers of threads were
useful in allowing many sections of the program to be run at the same
time. The results of the lab show that the multithreaded implementation
of the ray-tracer program allowed for performance to be increased drastically
with the use of an optimal number of threads.
