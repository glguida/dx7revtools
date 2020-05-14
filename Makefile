all: genrom raw2plot envtest0

genrom: genrom.c

raw2plot: raw2plot.c

envtest0: envtest0.c
	$(CC) $< -o $@ -lm
