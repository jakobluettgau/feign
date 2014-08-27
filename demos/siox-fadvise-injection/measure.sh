#!/bin/sh

SAMPLES=3


#OUT=baseline_application.csv
#for i in {1..3}
#do
#	echo $i
#	./fadvise-readAhead.withoutfadvise.stride12288.think100 >> $OUT
#done

#OUT=fadvise.csv
#Echo "us,us/op,op/us" > $OUT
#For i in {1..3}
#Do
#	echo $i
#	./replay.sh demo-trace --loglevel 0 | grep "csv:" | sed -r 's/^.{4}//' >> $OUT
#Done

OUT=baseline_replay.csv
echo "us,us/op,op/us" > $OUT
For i in {1..3}
Do
	echo $i
	./replay_unoptimized.sh demo-trace --loglevel 0 | grep "csv:" | sed -r 's/^.{4}//' >> $OUT
Done

