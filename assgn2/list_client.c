/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "list.h"
#include <string.h>

void
list_prog_1(char *host, char *opt, int arr[10])
{
	CLIENT *clnt;
	void  *result_1;
	int  append_1_arg;
	int  *result_2;
	int  query_1_arg=arr[0];
	int  *result_3;
	int  remove_1_arg=arr[0];
	char *app = "append";
	char *qry = "query";
	char *rm = "remove";
	//printf("debug level 1 %d",arr[9]);

#ifndef	DEBUG
	clnt = clnt_create (host, LIST_PROG, LIST_VERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */
	if(strcmp(opt, app)==0){  
		//printf("debug = append\n");
		result_1 = append_1(arr, clnt);
		if (result_1 == (void *) NULL) {
			clnt_perror (clnt, "call failed");
		}
	}
	else if(strcmp(opt,qry)==0){
		//printf("debug = query\n");
		result_2 = query_1(&query_1_arg, clnt);
		if (*result_2 == -1) {
			clnt_perror (clnt, "Can not perform this operation. Index doesn't exist\n");
		}
		else{
			printf("query[%d] result = %d\n",query_1_arg, *result_2);
		}
	}
	else if(strcmp(opt, rm)==0){
		//printf("debug = remove\n");
		result_3 = remove_1(&remove_1_arg, clnt);
		if (*result_3 == -1) {
			clnt_perror (clnt, "Can not perform this operation, Index doesn't exist\n");
		}
	}
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


int
main (int argc, char *argv[])
{
	char *host;
	char *opt;
	int arr[10];

	if (argc < 2) {
		printf ("usage: %s server_host <IP> <Options> <integers/integer>\n", argv[0]);
		exit (1);
	}
	host = argv[1];
	opt = argv[2];
	for(int i=3; i<argc; i++){
		sscanf(argv[i],"%d",&arr[i-3]);
		//printf("argv[i]=%d", arr[i-3]);
	}
		
	list_prog_1 (host, opt,arr);
exit (0);
}