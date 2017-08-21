PROG     = mp4
FLAGS    = -g -std=gnu11 -Wall -Werror -O3
CFLAGS   = $(FLAGS) -Iatom -I.
LDFLAGS  = $(FLAGS)
LDADD    = -ldynamic
OBJS     = main.o bytes.o mp4_atom.o mp4_atom_descriptor.o
ATOMS   += atom/mp4_atom_base.o
ATOMS   += atom/mp4_atom_container.o
ATOMS   += atom/mp4_atom_store.o
ATOMS   += atom/mp4_atom_types.o
ATOMS   += atom/mp4_atom_sidx.o

$(PROG): $(OBJS) $(ATOMS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDADD)

clean:
	rm -f $(PROG) $(OBJS) $(ATOMS)
