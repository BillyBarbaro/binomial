Binomial
==========

An exercise in learning the fork(), exec(), sleep(), and other commands for creating concurrently executing Linux processes.  The script creates 4 children.  The first prints an introduction line.  The second and third alternate, printing binomial coefficients.  The final child executes the command ls -l and prints the results to standard out.

Upon termination of each process, the parent prints various info about the terminating process.