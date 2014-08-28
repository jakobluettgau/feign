all:
	R -f plot.r --no-save

update:
	cp ../*.csv .
	cp ../*.raw .

print:
	more *.csv | cat

clean:
	rm -rf Rplots.pdf
	rm -rf plot_baseline-vs-fadvised.pdf

