CC = gcc
CFLAGS = -Wall -Wextra

SRCS = main.c displayFile.c fight.c game.c get_data.c save.c searchFunction.c   # Liste des fichiers source
OBJS = $(SRCS:.c=.o)                    # Liste des fichiers objets
EXEC = exe                               # Nom de l'exécutable

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(EXEC)
	find . -name "*.txt" -exec ./$(EXEC) {} +

clean:
	rm -f $(OBJS) $(EXEC)
