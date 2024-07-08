# sh05.sh
PATH=/bin:/sbin:/usr/bin:/usr/sbin:/usr/local/bin:/usr/local/sbin:~/bin
export PATH
# 1. ���ϥΪ̿�J�ɦW�A�åB�P�_�ϥΪ̬O�_�u������J�r��H
echo -e "Please input a filename, I will check the filename's type and \
permission. \n\n"
read -p "Input a filename : " filename
test -z $filename && echo "You MUST input a filename." && exit 0
# 2. �P�_�ɮ׬O�_�s�b�H�Y���s�b�h��ܰT���õ����}��
test ! -e $filename && echo "The filename '$filename' DO NOT exist" && exit 0
# 3. �}�l�P�_�ɮ������P�ݩ�
test -f $filename && filetype="regulare file"
test -d $filename && filetype="directory"
test -r $filename && perm="readable"
test -w $filename && perm="$perm writable"
test -x $filename && perm="$perm executable"
# 4. �}�l��X��T�I
echo "The filename: $filename is a $filetype"
echo "And the permissions are : $perm"


