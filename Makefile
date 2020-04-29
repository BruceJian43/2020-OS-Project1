b07705054_project1.out:main.o cpustate.o schedule.o
	gcc -o b07705054_project1.out main.o cpustate.o schedule.o
main.o:main.c process.h schedule.h
	gcc -c main.c
cpustate.o:cpustate.c cpustate.h process.h schedule.h
	gcc -c cpustate.c
schedule.o:schedule.c cpustate.h process.h schedule.h
	gcc -c schedule.c
clean:
	rm -f main.o cpustate.o schedule.o b07705054_project1.out
