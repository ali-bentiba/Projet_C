gym_app: equipement.o reservation.o main.o
	gcc equipement.o reservation.o main.o -o gym_app -g
main.o: main.c equipement.h reservation.h
	gcc -c main.c -g
equipement.o: equipement.c equipement.h
	gcc -c equipement.c -g
reservation.o: reservation.c reservation.h
	gcc -c reservation.c -g
  
