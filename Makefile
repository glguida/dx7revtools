all: genrom raw2plot

genrom: genrom.c


raw2plot: raw2plot.c
	$(CC) $< -o $@ -lm
