library(ggplot2)

pdf("fadvise-injection-runtimes.pdf", width=5, height=2)

# baseline_application.csv
# baseline_replay.csv
# fadvise.csv

fad = read.csv(file="fadvise.csv",head=TRUE,sep=",")
rep = read.csv(file="baseline_replay.csv",head=TRUE,sep=",")
app <- read.csv(file="baseline_application.csv",head=TRUE,sep=",")

fadm = mean(fad$us)/1000000
repm = mean(rep$us)/1000000
appm = mean(app$runtime)


names = c(
	"Application Baseline",
	"Replay Baseline",
	"Optimized Replay"
	)
times = c(appm, repm, fadm)

df = data.frame( run = factor(names, levels=names), runtime = times)
p = ggplot(data=df, aes(x=run, y=runtime, fill=run)) + geom_bar(stat="identity") + guides(fill=FALSE)

print(p)

dev.off()
