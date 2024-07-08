# sayH.sh
#
#Script to read your name from key-board
#
echo "Your first name and last name:"
read fname lname
lname=${lname:-"先生"}
echo "Hello $fname $lname, Lets be friend!"


