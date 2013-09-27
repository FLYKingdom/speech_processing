#include "../include/header.h"
using namespace std;

int DELVALUE = 0;
char MYPATH[10000];
char testfiledir[10000];
char modeldir[10000];
char outdir[10000];
char trainfiledir[10000];
char hmmdir[10000];
char spkrlist[10000];
char distance_dist[10000];
char score_dist[10000];
char ubm_dist[10000];
char list_of_testdata[10000];
char temppath[10000];
char confpath[10000];
void set_path()
{
	if(!strcmp(MYPATH,"./"))
	{
		sprintf(confpath,"../conf/conf.conf");
		sprintf(temppath,"../conf/temp");
	}
	else
	{
		sprintf(confpath,"%sconf.conf",MYPATH);
		sprintf(temppath,"%stemp",MYPATH);
	}
	if(WINDOWS)

	{
		sprintf(confpath,"%sconf.conf",MYPATH);
		sprintf(temppath,"%stemp",MYPATH);
	}
	sprintf(spkrlist,"%sspkrlist",MYPATH);
	sprintf(testfiledir,"%stestfiles/",MYPATH);
	sprintf(modeldir,"%smodel/",MYPATH);
	sprintf(outdir,"%sdistance/",MYPATH);
	sprintf(trainfiledir,"%strainfiles/",MYPATH);
	sprintf(hmmdir,"%shmmdir/",MYPATH);
	sprintf(distance_dist,"%sdistance/distance_dist",MYPATH);
	sprintf(score_dist,"%sdistance/score_dist",MYPATH);
	sprintf(ubm_dist,"%sdistance/ubm_dist",MYPATH);
	sprintf(list_of_testdata,"%slist_of_testdata",MYPATH);
	std::cout<<"confpath=";
	puts(confpath);
}

void error_handler(const char *str)
{
	FILE *fp;
	char mystr[1000],name[100];
	int r;

	strcpy(mystr,str);
	if(DELVALUE)
	{
		strcat(mystr,"The following models are deleted from the database because\n The present model is an imposter to these\n");
		fp = filecheck("del.txt");
		while((r=fscanf(fp,"%s\n",name))==1)
		{
			strcat(mystr,name);
			strcat(mystr,"\n");
		}
		fclose(fp);
	}
		
	fp = filecheckw("final_output");
	fprintf(fp,"%s\n",str);
	fclose(fp);
/*
	sprintf(mystr,"sam.txt");
	if(!access(mystr,R_OK))
		remove(mystr);
	sprintf(mystr,"sec.txt");
	if(!access(mystr,R_OK))
		remove(mystr);
	sprintf(mystr,"del.txt");
	if(!access(mystr,R_OK))
		remove(mystr);
	sprintf(mystr,"retrain_list.txt");
	if(!access(mystr,R_OK))
		remove(mystr);
	sprintf(mystr,"mod0");
	if(!access(mystr,R_OK))
		remove(mystr);
	sprintf(mystr,"mod1");
	if(!access(mystr,R_OK))
		remove(mystr);
	sprintf(mystr,"mod2");
	if(!access(mystr,R_OK))
		remove(mystr);
	sprintf(mystr,"htk.mfc");
	if(!access(mystr,R_OK))
		remove(mystr);
*/
	exit(0);
}

void read_header(struct totalheader *a)
{
	char label[400];
	FILE *fp;
//	fp = filecheck("conf.conf");
//	fp = filecheck("/var/srs/conf.conf");
	fp = filecheck(confpath);
	fscanf(fp,"%s = %s\n",label,(a->window_type));
	cout<<"Window Type = ";           //printing the value of a->window_type for debugging
	puts(a->window_type);
	cout<<"\n";
	fscanf(fp,"%s = %d\n",label,&(a->framesize));
	fscanf(fp,"%s = %d\n",label,&(a->windowsize));
	fscanf(fp,"%s = %s\n",label,(a->processframes));
	fscanf(fp,"%s = %f\n",label,&(a->highfreq));
	fscanf(fp,"%s = %f\n",label,&(a->lowfreq));
	fscanf(fp,"%s = %d\n",label,&(a->numbanks));
	fscanf(fp,"%s = %d\n",label,&(a->deltavalue));
	fscanf(fp,"%s = %d\n",label,&(a->numoffiles));
	fscanf(fp,"%s = %d\n",label,&(a->final_banks));
	fscanf(fp,"%s = %d\n",label,&(a->num_of_mixtures));
	fscanf(fp,"%s = %hd\n",label,&(a->mobile));
	fscanf(fp,"%s = %hd\n",label,&(a->recorder));
	fscanf(fp,"%s = %hd\n",label,&(a->gui));
	fscanf(fp,"%s = %hd\n",label,&(a->mfcc_type));
	fscanf(fp,"%s = %hd\n",label,&(a->spec_sub));
	fscanf(fp,"%s = %hd\n",label,&(a->cms));
	fscanf(fp,"%s = %hd\n",label,&(a->cnv));
	a->peak_len = 2;
	if(a->gui)
	{
		sprintf(confpath,"%sconf.conf",MYPATH);
		sprintf(temppath,"%stemp",MYPATH);
	}
	fclose(fp);
}
void print_header(struct totalheader a)
{
	printf("sampfreq= %d\n",a.sampfreq);
	printf("fftorder= %d\n",a.fftorder);
	printf("window type = %s\n",a.window_type);
	printf("framesize = %d\n",a.framesize);
	printf("window size = %d\n",a.windowsize);
	printf("rawfile size = %ld\n",a.raw_length);
	printf("prcess frames = %s\n",a.processframes);
	printf("high freq = %f\n",a.highfreq);
	printf("low freq = %f\n",a.lowfreq);
	printf("near low = %f\n",a.nearlow);
	printf("nearhigh = %f\n",a.nearhigh);
	printf("breadth = %d\n",a.breadth);
	printf("numbanks = %d\n",a.numbanks);;
	printf("delta value = %d\n",a.deltavalue);
	printf("num of files = %d\n",a.numoffiles);
	printf("no of frames = %d\n",a.no_of_frames);
	printf("no of train frames = %d\n",a.train_frames);
	printf("final banks = %d\n",a.final_banks);
	printf("num of mixtures = %d\n",a.num_of_mixtures);
	printf("mfcc type = %hd\n",a.mfcc_type);
	printf("spec subtraction = %hd\n",a.spec_sub);
	printf("cms  = %hd\n",a.cms);
	printf("cnv = %hd\n",a.cnv);
	printf("mobile = %hd\n",a.mobile);
	printf("recorder  = %hd\n",a.recorder);
	printf("gui = %hd\n",a.gui);
}

void getidentity(struct totalheader *a)
{
	FILE *fp;
	int i,j,from,to,size;
	int count;
	char speech[100];

	a->ident = assign_vector(a->ident,a->no_of_frames);
	size = a->windowsize;
	fp = filecheck("sam.txt");
	i=count=0;
	while(fscanf(fp,"%d %d %s\n",&from,&to,speech)!=EOF)
	{
		j = (to - from+1)/size;
		j--; // uncomment this line gives good results
		while(j--!=0)
		{
			if(!strcmp(speech,"speech"))
				a->ident[i++] = 1;
			else
				a->ident[i++] = 0;
			count += a->ident[i-1];
		}
		a->ident[i++] = 0; // related to above commented line
	}
	while(i!=a->no_of_frames)  // to compensate appended zeros
		a->ident[i++] = 0;
	a->train_frames = count;
}

void calc_header_values(struct totalheader *a)
{
	int i,framecount;
        float frame,freq;

	a->fftorder = 2;
	a->framesize = (a->framesize * a->sampfreq) / 1000;
	a->windowsize = (a->windowsize * a->sampfreq) / 1000;
	while(a->framesize > a->fftorder)
		a->fftorder *= 2;

        frame=(float)(a->framesize * a->highfreq *1.00/ (float)a->sampfreq);
        frame=(float)(ceil(frame));
        a->nearhigh = frame*a->sampfreq/a->framesize;

        frame=(float)(a->framesize * a->lowfreq *1.00/(float)a->sampfreq);
        frame=(float)floor(frame);
        a->nearlow = frame*a->sampfreq/a->framesize;

//      framecount =0;
        framecount =-1;//since 0th coefficient we are not considering
	frame = a->fftorder/2;
        for(i=1;i<=frame;i++)
        {
                freq = (float)((i*a->sampfreq*1.0)/(float)(a->fftorder));
//printf("%d = %f \n",i,freq);
                if ( freq >= a->nearlow && freq <= a->nearhigh && freq != 0)
                       framecount++;
        }
	a->breadth = framecount;
	a->no_of_frames = (a->raw_length - a->framesize + a->windowsize)/a->windowsize;
	getidentity(a);
/*	for(i=0;i<a->no_of_frames;i++)
		a->ident[i] = 1;
	a->train_frames = a->no_of_frames;*/
}

void getfilename(char *filename,char *wv)
{
        char *ptr=NULL;
        int loc,i,count,j=0;
        ptr = strrchr(wv,'.');
        count = (int)(ptr - wv);
        loc = count - 1;
        while(loc!=0)
        {
                if(wv[loc]=='/'|| wv[loc] =='\\')
                {
                        loc++;
                        break;
                }
                loc--;
        }
        for(i=loc;i<count;i++)
                filename[j++] = wv[i];
        filename[j]='\0';
}

void getname(int count,char *fn,char *list)
{
	FILE *fp;
	int i;

	fp = filecheck(list);
	for(i=0;i<=count;i++)
		fscanf(fp,"%s\n",fn);
	fclose(fp);
}

/* We Needed One Function to See if the File given the user exists or not */
/* If the File Exists Open the File in the Read Mode and Return a Pointer to that File */
FILE *filecheck(const char *s)
{
	FILE *f;
	char mystr[200];
	if ( (f = fopen(s,"rb")) == NULL) 
	{
		sprintf(mystr,"cannot open %s file",s);
		error_handler(mystr);
	}
	return f;
}
/* We Need to See If the FILE exists if not open in write mode */
FILE *filecheckw(const char *s)
{
        FILE *fp;
	char mystr[200];
        if ( (fp=fopen(s,"wb")) == NULL )
	{
		sprintf(mystr,"cannot open %s file",s);
		error_handler(mystr);
	}
        return fp;
}
FILE *filechecka(const char *s)
{
	FILE *fp;
	char mystr[200];
	if((fp = fopen(s,"a+"))==NULL)
	{
		sprintf(mystr,"cannot open %s file",s);
		error_handler(mystr);
	}
	return fp;
}
FILE *filecheckab(const char *s)
{
	FILE *fp;
	char mystr[200];
	if((fp = fopen(s,"ab+"))==NULL)
	{
		sprintf(mystr,"cannot open %s file",s);
		error_handler(mystr);
	}
	return fp;
}

void appendheader(const char *filename,FILE *fp)
{
        FILE *header;
        char c;
        header=filecheck(filename);
        /*First Put the Header Info to temp */
        while ( (c=fgetc(header)) != EOF )
                fputc(c,fp);
        fclose(header);
}

float rounding(float val)
{
	float var1;
	int mn;

	mn = (int) (val);
	var1 = val - mn;
	if(var1 >0.5)
		return (float)mn+1;
	else
		return (float)mn;
}

double get_median(double *vec,int len)
{
	int index;
	quicksort(vec,0,len-1);
	index = len/2;
	if((len%2) == 1)
		index++;
	return vec[index];
}
/* Function to do LOG */
void mylog(float *data, int N)
{
	int i;

        for(i=0; i<N; i++)
	{
		if ( data[i] != 0 )
        	        data[i]=(float)(log(data[i]));
		else
			data[i] = -708.0;
	}
}
/* Function to do CUBE ROOT */
void cube_root(float *data, int N)
{
	int i;
        for(i=0; i<N; i++)
		data[i]=(float)(pow(data[i],1/3));
}
