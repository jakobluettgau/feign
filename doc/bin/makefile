SRC_PU = $(wildcard *.pu)
SRC_DIA = $(wildcard *.dia)

all: pu dia

pu:
	plantuml $(SRC_PU)
	plantuml -tsvg $(SRC_PU)
	plantuml -teps $(SRC_PU)

dia:
	dia --nosplash -t eps $(SRC_DIA)

clean:
	rm -rf *.png
#	rm -rf *.svg
	rm -rf *.eps
	rm -rf *-eps-converted-to.pdf
