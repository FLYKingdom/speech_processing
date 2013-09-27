/*
		This code is written by Anand Dhandhania.
		School of Computing & Electrical Engineering.
		IIT Mandi

		In the project 
*/

#include <iostream>
#include "../include/header.h"
#include "../include/functions.h"
#include "../include/variable.h"
using namespace std;

int main(int argc,char *argv[])
{
	char filename[200],path[500];
	char mystring[500],delname[200];
	struct totalheader a;
	FILE *fp;
	int retval;
	int train_len;
	int total_spkrs;
	if(argc < 2)
		error_handler("usage is <exe> <wavefilename>");
	else if( argc == 2)
		strcpy(MYPATH,"./");
	else if( argc == 3)
		strcpy(MYPATH,argv[2]);
	else
		error_handler("usage is wrong please check");

	set_path();

	read_header(&a);
	//		if(a.gui == 1)
	//		{
	//	strcpy(filename,argv[1]);
		//	getpath(filename,path);
	//	}
	//	else
		  //	  {
		getfilename(filename,argv[1]);
		strcpy(path,argv[1]);
		//		}

	strcpy(a.filename,filename);
	input_validation(path,&a);

//for(int i = 0;i<a.raw_length;i++)
//	printf("%f \n",a.raw[i]);
	
	
	silence(filename,a,1);
	cout<<"Sample Frequency="<<a.sampfreq<<"\n";

	calc_header_values(&a);
       
//	print_header(a);exit(0);
	printf("%s trained succesfully\n",filename);
	error_handler("trained successfully");
	return 0;
}
