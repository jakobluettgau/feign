library(ggplot2)

pdf("plot-fadvise-injection-runtimes.pdf", width=4, height=2)

# baseline_application.csv
# baseline_replay.csv
# fadvise.csv

opt = read.csv(file="optimized_replay.csv",head=TRUE,sep=",")
rep = read.csv(file="baseline_replay.csv",head=TRUE,sep=",")
app <- read.csv(file="baseline_application.csv",head=TRUE,sep=",")
appopt <- read.csv(file="optimized_application.csv",head=TRUE,sep=",")

optm = mean(opt$us)/1000000
repm = mean(rep$us)/1000000
appm = mean(app$runtime)
appoptm = mean(appopt$runtime)


how = c("Application","Application","Replay","Replay")

what = c("Baseline", "Optimized", "Baseline", "Optimized")

times = c(appm, appoptm, repm, optm)

df = data.frame(how=factor(how, levels=c("Application", "Replay")), what=factor(what, levels=c("Baseline", "Optimized")), runtime=times)

p = ggplot(data=df, aes(x=how, y=runtime, fill=what)) + geom_bar(stat="identity", position=position_dodge())

print(p)

dev.off()
