targetname=`ls -1 ./test2 | egrep '\.htm$'`

cd test2

for i in $targetname
do
	j="${i%.htm}"
	mv $j.htm $j.html 
	#mv $i.htm $i.html
done

