echo "Hi there"
echo "what is your name? (Type your name here and press Enter)"
read name
echo "Hello $name"
echo "Numbers are in one line space separated:"
read a b c < data_file
echo a = $a
echo b = $b
echo c = $c
echo "Numbers are stored one per line:"
{
 read a
 read b
 read c
} < data_file
echo a = $a
echo b = $b
echo c = $c

