#
# Classify character given as argument
#
if [ $# -ne 1 ]
then
       echo Usage: ctype char
       exit 1
fi
#
# Ensure that only one character was typed
#
char="$1"
numchars=$( echo "$char" | wc -c )
if [ $numchars -ne 2 ]
then
        echo Please type a single character
        exit 1
fi
#
# Now classify it
#
case "$char"
in
        [0-9]  ) echo digit;;
        [[:upper:]]* ) echo uppercase letter;;
        [[:lower:]]* ) echo lowercase letter;;
        *      ) echo special character;;
esac

