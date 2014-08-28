#!/bin/sh

SAMPLES=3


#OUT=baseline_application.raw
#for i in {1..3}
#do
#	echo $i
#	./fadvise-readAhead.withoutfadvise.stride12288.think100 >> $OUT
#done


OUT=optimized_replay.csv
echo "us,us/op,op/us" > $OUT
for i in {1..3}
do
	echo $i
	./replay.sh demo-trace --loglevel 0 $@ | grep "csv:" | sed -r 's/^.{4}//' >> $OUT
done

OUT=baseline_replay.csv
echo "us,us/op,op/us" > $OUT
for i in {1..3}
do
	echo $i
	./replay_unoptimized.sh demo-trace --loglevel 0 $@ | grep "csv:" | sed -r 's/^.{4}//' >> $OUT
done

cd demo-measures
make update
make
