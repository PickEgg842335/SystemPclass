#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
int main(void){
	char c;
	int in, out;
	/*開啟檔案file.in讀取的權限*/
	in = open("file.in", O_RDONLY);

	/*建立file.out寫入的權限傭有者可以讀寫*/
	out = open("file.out", O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR);

	/*迴圈將file.in的資料讀取出來*/
	while(read(in, &c, 1) == 1) {
		write(out, &c, 1);
	}
	close(in);	
	close(out);
	return 0;
}
