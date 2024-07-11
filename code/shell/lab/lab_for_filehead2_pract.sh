targetname=`ls -1 ./test2 | egrep '\.htm$'`

cd test2

for i in $targetname
do	
	head -n 2 $i >> file_heads
done

