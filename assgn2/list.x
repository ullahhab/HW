

typedef int arr[10];
program LIST_PROG{
	version LIST_VERS{
		void append(arr) = 1;
		int query(int)=2;
		int remove(int)=3;
	}=1;
}=0x32134567;
