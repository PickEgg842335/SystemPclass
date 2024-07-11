targetname=`ls -1 ./test2 | egrep '\.html$'`

cd test2

for i in $targetname
do
	j="${i%.html}"
	mv $j.html $j.htm 
	
done

