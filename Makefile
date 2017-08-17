PROG    = mp4
FLAGS   = -g -std=gnu11 -Wall -Werror -O3
CFLAGS  = $(FLAGS)
LDFLAGS = $(FLAGS)
LDADD   = -ldynamic
OBJS    = main.o bytes.o mp4.o

$(PROG): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDADD)

clean:
	rm -f $(PROG) $(OBJS)
