# ginfo.sh
#
#
# Script to print user information who currently login , current date & time
#
clear
echo "Hello $USER"
echo "Today is /home/charly";date
echo "Number of user login : /home/charly"; who | wc -l # wc: word count
echo "Calendar"
cal
exit 0


