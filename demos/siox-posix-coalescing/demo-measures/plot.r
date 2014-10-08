library(ggplot2)
library(scales)

pdf("plot-coalescing-runtimes.pdf", width=4, height=2)

# baseline_application.csv
# baseline_replay.csv
# fadvise.csv

chunksizes = c(
2	,
4	,
8	,
16	,
32	,
64	,
128	,
256	,
512	,
1024	,
2048	,
4096	,
8192	,
16384	,
32768	,
65536	,
131072	,
262144	,
524288	,
1048576	,
2097152	,
4194304	,
8388608	,
16777216	

)

optmeans = c()
for (i in chunksizes) {
	csvfile = paste("optimized_replay_", i, ".csv", sep = "")
	csvdata = read.csv(file=csvfile,head=TRUE,sep=",")
	csvmean = mean(csvdata$us)/1000000
	optmeans = c(optmeans, csvmean)
}
print(optmeans)

rep = read.csv(file="baseline_replay.csv",head=TRUE,sep=",")
repm = mean(rep$us)/1000000

print(repm)

# plot
df = data.frame( chunksize = chunksizes, runtime = optmeans)
print(df)
p = ggplot(data=df, aes(x=chunksize, y=runtime)) + geom_point()  + ylim(0,repm) + stat_function(fun=function(x)repm, geom="line", aes(colour="replay baseline")) + scale_x_continuous(trans=log10_trans()) + ylab("runtime in s") + xlab("buffer size")
#p = ggplot(data=df, aes(x=chunksize, y=runtime)) + geom_point()

print(p)

dev.off()
