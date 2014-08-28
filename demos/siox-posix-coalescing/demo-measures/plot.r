library(ggplot2)

pdf("plot-coalescing-runtimes.pdf", width=5, height=2)

# baseline_application.csv
# baseline_replay.csv
# fadvise.csv

opt = read.csv(file="optimized_replay.csv",head=TRUE,sep=",")
rep = read.csv(file="baseline_replay.csv",head=TRUE,sep=",")
#app <- read.csv(file="baseline_application.csv",head=TRUE,sep=",")

optm = mean(opt$us)/1000000
repm = mean(rep$us)/1000000
#appm = mean(app$runtime)


names = c(
	"Replay Baseline",
	"Optimized Replay"
	)
#times = c(appm, repm, optm)
times = c(repm, optm)

df = data.frame( run = factor(names, levels=names), runtime = times)
p = ggplot(data=df, aes(x=run, y=runtime, fill=run)) + geom_bar(stat="identity") + guides(fill=FALSE)

print(p)

dev.off()
