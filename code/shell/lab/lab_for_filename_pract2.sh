targetfile=`ls ./test`

cd test

for i in $targetfile
do
	echo "$i"
	mv $i `echo $i | tr '[A-Z]' '[a-z]'`
done

