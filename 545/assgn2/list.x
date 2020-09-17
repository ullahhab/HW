

typedef int arr[10];
program LIST_PROG{
	version LIST_VERS{
		void append(arr) = 1;
		void query(int)=2;
		void remove(int)=3;
	}=1;
}=0x32134567;
