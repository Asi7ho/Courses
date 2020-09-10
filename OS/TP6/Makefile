CC=`which clang`
LD=$(CC)
RM=`which rm` -f

# Do things DURATION_SCALE times slower
DURATION_SCALE=100

# Scheduler parm
QUANTUM_DFLT=12

# Process types
PROCESS_INTERACTIVE=100 16 16
PROCESS_NO_IO_SLOW =100 16 1
PROCESS_NO_IO_FAST =100 8 1

# Process parms (unused by now)
MIN_PROCESS_DIVERSITY=1
MAX_PROCESS_DIVERSITY=13
MIN_BURST_DURATION=4
MAX_BURST_DURATION=16
MIN_PROC_DURATION=52
MAX_PROC_DURATION=152

CFLAGS=-Wall -Wextra -g -Iinclude -DQUANTUM_DFLT=$(QUANTUM_DFLT) -DDURATION_SCALE=$(DURATION_SCALE)

SRC_SCHED=sched.c queue.c
SRC_PROC=process.c

OBJ_SCHED=$(SRC_SCHED:.c=.o)
OBJ_PROC=$(SRC_PROC:.c=.o)

all : scheduler process


scheduler : $(OBJ_SCHED)
	$(LD) $^ -o $@ $(LFLAGS) 

process : $(OBJ_PROC)
	$(LD) $^ -o $@ $(LFLAGS)

launch_sched_rr : all
	xterm -e "./scheduler rr" & 

launch_sched_sjf : all
	xterm -e "./scheduler sjf" & 

launch_sched_mlfq : all
	xterm -e "./scheduler mlfq" & 

launch_proc_no_io_slow : all
	xterm -e "./process $(PROCESS_NO_IO_SLOW)" &

launch_proc_no_io_fast : all
	xterm -e "./process $(PROCESS_NO_IO_FAST)" &

launch_proc_interactive : all
	xterm -e "./process $(PROCESS_INTERACTIVE)" &


launch_session_rr : clean all
	make launch_sched_rr
	sleep 1s
	make launch_proc_no_io_fast
	make launch_proc_no_io_fast
	make launch_proc_no_io_slow

launch_session_sjf : clean all
	make launch_sched_sjf
	sleep 1s
	make launch_proc_no_io_fast
	make launch_proc_no_io_fast
	make launch_proc_no_io_slow

launch_session_mlfq : clean all
	make launch_sched_mlfq
	sleep 1s
	make launch_proc_interactive
	make launch_proc_interactive
	make launch_proc_no_io_slow

%.o : %.c
	$(CC) $(CFLAGS) $< -c

clean : 
	$(RM) scheduler process $(OBJ_SCHED) $(OBJ_PROC) *~ semantic.cache include/*~ 

