#! /bin/bash
for((i=1;i<=10;i=i+1))
do
    echo "$i" 
    ./mydisambig output_data/sep/$i.sep ZhuYin-Big5.map output_data/corpus.lm output_data/mine/$i.out 
    diff output_data/ref/$i.out output_data/mine/$i.out
done