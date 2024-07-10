PATH=/bin:/sbin:/usr/bin:/usr/sbin:/usr/local/bin:/usr/local/sbin:~/bin
export PATH

currentpath=`pwd`;

cd /root

dirname="test"
targetname="logical"

if ! [ -d $dirname ]
then
    mkdir $dirname
    echo "Create $dirname folder."
fi

cd ./$dirname

if ! [ -e $targetname ] && ! [ -d $targetname ]
then
    touch $targetname
    echo "Create $targetname file."
elif ! [ -d $targetname ]
then
    echo "You already have $targetname file."
    rm -r $targetname
    echo "Remove $targetname file."
    mkdir $targetname
    echo "Create $targetname folder."
else
    echo "You aleady have $targetname folder."
    rm -r $targetname
    echo "Remove $targetname folder."
fi

cd $currentpath

