#include "../include/header.h"
#include "../include/validation.h"
#include<time.h>
//#include "../include/max_array.h"  //to compute max_of_speech
//#include "../include/mean.h"
using namespace std;

//void readdata(FILE *fp,int spf,short bps,int *a,int inc,int *st)
void readdata_file(FILE *fp,int **a,int row,int col,int bps)
{
	int readvalue;
	int i,j;
	void *value;
//	UNCOMMENT THESE LINES FOR LINUX
	int16_t value1;
	int8_t value2;

	if(bps == 16)
	{	
		readvalue = 2;
		value = &value1;
	}
	else if(bps == 8)
	{
		readvalue = 1;
		value = &value2;
	}
	for(i=0;i<row;i++)
	{
		j=0;
		while(j<col)
		{
			if((fread(value,readvalue,1,fp))!=0)
			{
//	UNCOMMENT THESE LINES FOR LINUX  AND COMMENT FOR WINDOWS
				if(readvalue ==1)
					a[i][j++]= *((int8_t*)value);
				else
					a[i][j++]= *((int16_t*)value);
			}
			else
				while(j<col)
					a[i][j++]=0;
//printf("%d\n",a[i][j-1]);
		}
	}
}		

void find(int *min,int *max,int **a,int nf,int spf)
{
	int low,high,i,j;
	for(i=0;i<nf;i++)
	{
		low = 32767;
		high = -32768;
		for(j=0;j<spf;j++)
		{
			if(a[i][j] < low)
				low = a[i][j];
			if(a[i][j] > high)
				high = a[i][j];
		}
		min[i] = low;
		max[i] = high;
	}
}	

int bin(int *min,int size,int binsize,int orientation)
{
	int minval,maxval,diff;
	int i,j,count,comparemin,comparemax;
	int *bin=NULL,countprev,startpoint,endpoint;
	int before,after,retvalue;
	int totallength;
	float measure;

	bin = (int*)malloc(binsize *sizeof(int));
	i = 0;
	minval =maxval = min[i++];
	while(i<size)
	{
		if(minval > min[i])
			minval = min[i];
		else if(maxval < min[i])
			maxval = min[i];
		i++;
	}
	diff = abs((maxval-minval)/binsize);
//printf("minval = %d ,maxval = %d and diff = %d\n",minval,maxval,diff);
	count = 0;
	for(i=0;i<binsize-1;i++)
	{
		bin[i] = 0;
		comparemin = minval+(i*diff);
		comparemax = comparemin + diff;
		for(j=0;j<size;j++)
			if(min[j] >= comparemin && min[j] < comparemax)
				(bin[i])++;
		count += bin[i];
	}
	bin[i] = size - count;

	if(orientation ==0)
		measure = (float)bin[0]/(float)size;
	else
		measure = (float)bin[binsize-1]/(float)size;
	measure *= 100;
	if(measure >= 5.0)
		error_handler("file is clipped Input is not Accepted");

	countprev = 0;
	if(orientation == 0)
	{
		for(i=binsize-1;i>=0;i--)
		{
			count = 0;
			if(bin[i] == 0)
			{
				after = i;
				while(i>=0 && bin[i] == 0)
				{
					count++;
					i--;
				}
				before = ++i;
			}
			if(count >= 2)
			{
				totallength = 0;
				for(j=binsize-1;j>=after;j--)
					totallength += bin[j];
				measure = (float)totallength/(float)size;
				measure *= 100;

				if(measure > 95.0)
				{
					endpoint = after;
					startpoint = before;
					countprev = count;
					break;
				}
			}
		}
	}
	else if(orientation == 1)
	{
		for(i=0;i<binsize;i++)
		{
			count = 0;
			if(bin[i] == 0)
			{
				before = i;
				while(i<binsize && bin[i] == 0)
				{
					count++;
					i++;
				}
				after = --i;
			}
			if(count >=2)
			{
				totallength = 0;
				for(j=0;j<before;j++)
					totallength += bin[j];
				measure = (float)totallength/(float)size;
				measure *= 100;
				if( measure >95.0)
				{
					endpoint = after;
					startpoint = before;
					countprev = count;
					break;
				}
			}
		}
	}

/*	printf("count = %d and startpoint = %d endpoint = %d\n",count,startpoint,endpoint);
	for(i=0;i<binsize;i++)
		printf("bin[%d]=%d\t",i,bin[i]);
printf("\n\n");getchar();*/
	if(countprev >1)
	{
		if(orientation == 0)
		{
			retvalue = endpoint+1;
			retvalue *= diff;
			retvalue += minval;
		}
		else if(orientation ==1)
		{
			retvalue = startpoint * diff;
			retvalue += minval;
		}
	}
	else
	{
		if(orientation == 0)
			retvalue = minval;
		else if(orientation ==1)
			retvalue = maxval;
	}
	free(bin);
	return retvalue;
}



int max_matrix(int **a,int row,int col)  //function to find maximum value in a matrix
{
  int max=0;
  int i,j;
  for(i=0;i<row;i++)
    {
      for(j=0;j<col;j++)
        {
          if(max<a[i][j])
            max=a[i][j];
        }
    }
  return max;
}
int min_matrix(int **a,int row,int col)  //function to find out minimum value in a matrix
{
  int min=0;
  int i,j;
  for(i=0;i<row;i++)
    {
      for(j=0;j<col;j++)
        {
	 
          if(min>a[i][j])
            min=a[i][j];
        }
    
    }
  return min;
}
float std_dev(int **a,int row,int col,float *mean)  //function to find out the standard deviation of whole matrix
{
  float std_dev,variance;
  long int sum,sqsum;
  int i,j;
  for(i=0;i<row;i++)
    {
      for(j=0;j<col;j++)
	{
	  sum+=a[i][j];
	  sqsum+=a[i][j]*a[i][j];
	}
    }
  // cout<<" sum 1="<<sqsum<<"\n"; 
  *mean=(float)sum/(row*col);
  variance=(float)sqsum/(row*col)-(float)(sum*sum)/(row*row*col*col);

  std_dev=sqrt(variance);
  return std_dev;
  
}
void devpf(int **a,int row,int col,float *deviationpf,float *meanpf,float *absmeanpf) //function to find standard deviation and mean
{
  float variance;
  long int sum,abssum;
  int i,j;
  for(i=0;i<row;i++)
    {
      sum=0;
      abssum=0;
      variance=0.0;
      for(j=0;j<col;j++)
	{
	  sum+=a[i][j];
	  abssum+=abs(a[i][j]);
	 
	}
      absmeanpf[i]=(float)abssum/col;
      meanpf[i]=(float)sum/col;
      for(j=0;j<col;j++)
	variance+=(float)((a[i][j]-meanpf[i])*(a[i][j]-meanpf[i]))/col;
     
      deviationpf[i]=sqrt(variance);
    }
}
void global_devpf(int **a,int row,int col,float *global_deviationpf,float mean) //function to find deviation using global mean
{
  int i,j;
  float variance;
  for(i=0;i<row;i++)
    {
      variance=0.0;
      for(j=0;j<col;j++)
	{
	  variance+=(float)((a[i][j]-mean)*(a[i][j]-mean))/col;
	}
      global_deviationpf[i]=sqrt(variance);
    }
}

void extremumpf(int **a,int row,int col,int *maxpf,int *minpf) //function to find max and min values per frame
{
  int temp_max,temp_min;
  int i,j;
  for(i=0;i<row;i++)
    {
      temp_max=0;
      temp_min=0;
      for(j=0;j<col;j++)
	{
	  if(temp_max<a[i][j])
	    temp_max=a[i][j];

	  if(temp_min>a[i][j])
	    temp_min=a[i][j];
	}
      maxpf[i]=temp_max;
      minpf[i]=temp_min;
    }
}

//function to find the outlier samples. (spf=160) 

/*int outliers(int **a,float *dev,int length,int noofsamples,float z,short int *p) 
{
  float mean,deviation,variance;
  int i,j,count=0,counter=0;
  long long int sqsum=0;
  long double sum=0;
  
 
  for(i=0;i<length;i++)
    {
      sum+=dev[i];
      sqsum+=(int)(dev[i]*dev[i]);
    }

  mean=(float)sum/length;
  
  variance=(float)(sqsum/length) - (float)(mean*mean);
  deviation=sqrt(variance);
 
 
 
  for(i=0;i<length;i++)
    {
      
      for(j=0;j<noofsamples;j++)
	{
	  if((a[i][j]>(mean+z*deviation))||(a[i][j]<(mean-z*deviation)))
	    {
	    p[count++]=1;
	    counter++;
	    }
	  else
	    p[count++]=0;
	}
    }
 
  return counter;
}*/

void sort(int *array,int length)
{
  int i,j,min,pos;
  for(i=0;i<length;i++)
    {
      min=array[i];
      for(j=i;j<length;j++)
	{
	  if(min>array[j])
	    {
	    min=array[j];
	    pos=j;
	    }
	}
      array[pos]=array[i];
      array[i]=min;
    }
}

void median(int **mat,int row,int col,float *medianpf)
{
  int i,j;
  int *p;
  p=(int*)malloc(col*sizeof(int));
  for(i=0;i<row;i++)
    {
      for(j=0;j<col;j++)
	{
	  p[j]=mat[i][j];
	}
      sort(p,col);
      medianpf[i]=(float)(p[(col/2)]+p[((col/2)-1)])/2;
    }
  free(p);
}



float histogram(float *vector,long int length,int bins,int *bin_count)
{
  int i,j;
  FILE *f;
  float bin_size,max=0.0,min=0.0;
  for(i=0;i<bins;i++)
    bin_count[i]=0;

  f=fopen("histogram50.txt","wb");
  for(i=0;i<length;i++)
    {
      if(max<vector[i])
	max=vector[i];
      if(min>vector[i])
	min=vector[i];
    }
  bin_size=(float)(max-min)/bins;
  for(i=0;i<bins;i++)
    {
      for(j=0;j<length;j++)
	{
	   if( vector[j]>=(min+i*bin_size) && vector[j]<=(max-((bins-i-1)*bin_size)))
	    bin_count[i]++;
	}
      fprintf(f,"%f to %f =  %d \n",(min+(i*bin_size)),(min+((i+1)*bin_size)),bin_count[i]);
    }
 
  fclose(f);
  return bin_size;
}




long int sample_count;

void threshold(float *p,int bins,double *max,double *min,int *bin_count,int total_samples,float bin_size,int *pos,int max_array,int min_array)
{
  int i,point;
  float maxima=0.0;
  float *q;
  long int sample_count=0;

  q=(float*)malloc(bins*sizeof(float));
  for(i=0;i<bins;i++)
    *(q+i)=*(p+i);

  for(i=0;i<bins;i++)
    if(maxima<q[i])
     {
       maxima=q[i];
       point=i;
     }
  printf("%d  %d \n",pos[1],pos[0]);
  if(pos[1]!=49 && pos[0]!=0)
    {
      if((bins-1-point)<(point-0))
	pos[1]=point;  //pos[1] stores the right extremum.
      else
	pos[0]=point; //pos[0] stores the left extremum;
    }
  else if(pos[1]==49 && pos[0]!=0)
    pos[1]=point;
   
  else
    pos[0]=point;
  

  // printf("%d  %d\n",pos[1],pos[0]);
  q[pos[1]]=0;
  q[pos[0]]=0; 

  /*  if(pos[0]>pos[1])   //swap if left extremum stores a value greater than right extremum
    {
      temp=pos[0];
      pos[0]=pos[1];
      pos[1]=temp;
      }*/

  for(i=0;i<bins;i++)
    {
      if(pos[0]!=0 && pos[1]!=49)
	{
	  if(i>(pos[0]) && i<(pos[1]))
	    sample_count+=bin_count[i];
	}
      else if(pos[0]==0 && pos[1]!=49)
	{
	  sample_count+=bin_count[pos[1]];
	}
      else
	{
	  sample_count+=bin_count[pos[0]];
	}
    }


  // printf("%d\n",sample_count);
  if(sample_count>=(.999*total_samples))
    {
    *max=min_array+(pos[1]-1)*bin_size;    
    *min=min_array+(pos[0]+1)*bin_size;
    }
  else
    threshold(q,bins,max,min,bin_count,total_samples,bin_size,pos,max_array,min_array);

}

 float minimum(float a,float b)
   {
     if(a>0 && b>0)
       {
	 if(a<b)
	   return a;
	 else
	   return b;
       }
     else
       {
	 if(a<b)
	   return b;
	 else
	   return a;
       }
   }

 void interpolation(float *arr,int *outliers,int total_outliers)
   {
     int i=0,k,j;
     float temp1,temp2;
     int p=0,count=0;
    
     for(j=0;j<total_outliers;j++)
       {
	 k=1;
	 count=0;
	 if(outliers[j+1]!=(outliers[j]+1))
	   {
	     i=outliers[j]-1;
	     temp1=2*arr[i-1]-arr[i-2];
	     temp2=2*arr[i+1]-arr[i+2];
	     arr[i]=(temp1+temp2)/2;

	     
	   }
	 else
	   {
	    
	     while(outliers[j+k]==outliers[j]+k) 
	       k++;
	     while(((j+k)<total_outliers) && (outliers[j+k]==outliers[j+k-1]+2))
	       {
		 k=k+2;
		 count+=2;
	       }
	     // printf("%d  %d\n",k,count);
	     i=outliers[j]-1;
	     temp1=arr[i-1]-arr[i-2];
	     temp2=arr[i+k]-arr[i+k+1];
	     //   printf("%f %f \n",temp1,temp2);
	    
	     //     for(p=0;p<k;p++)
	     //    arr[i+p]=minimum((arr[i-1]+(p+1)*(arr[i-1]-arr[i-2])),(arr[i+k]+(k-p)*(arr[i+k]-arr[i+k+1])));
	 
	     for(p=0;p<k;p++)
	       {
		 arr[i+p]=(((k-p)*(arr[i-1]+(p+1)*temp1)+(p+1)*(arr[i+k]+(k-p)*temp2))/(k+1));   //weighted average 
	       }
	     
	     j=j+k-1-count;
	   }
       

       }
       }
      



void mean(FILE *fp,struct FormatChunk fmt,long spf,int inc,struct totalheader *a,long size)
{

        int minimum,maximum,diff,max_of_speech,min_of_speech;
        float *meanpf=NULL,*absmeanpf=NULL,*deviationpf=NULL,*global_deviationpf=NULL,*medianpf=NULL,*COV=NULL;
        int *minpf=NULL,*maxpf=NULL,*bin_count=NULL,*bin_raw=NULL;
        float global_deviation,global_mean;
        float avg,temp=0.0;
        int **arr=NULL,*ma=NULL,*mi=NULL;
        float *farr=NULL;
        int i,j,k=0,l=0,counter=0,count=0;
        long totalframes,length;
        short int check_bit=1;
        float binsize,rawsize;
        int noofbins=50;
        double max_threshold=0.0,min_threshold=0.0;
        float *percent_bin_diff;
        int position[2];
	position[0]=0;
	position[1]=(noofbins-1);
	int frame_outlier[spf];
	float *difference;


  
        int num_outliers,num_g_outliers;
	float z=3.0;
	FILE *fs; 

	totalframes =(int)(size/spf);
	if(size%spf !=0)
		totalframes++;
       
       	int *outlier_samples;
	short int outlier_frames[totalframes];
	short int g_outlier_samples[totalframes*spf];
	short int and_outliers[totalframes*spf];

	percent_bin_diff=(float*)malloc(noofbins*sizeof(float));
	farr = (float*)malloc(totalframes*spf*sizeof(float*));
        mi = (int*)malloc(totalframes*sizeof(int));
        ma = (int*)malloc(totalframes*sizeof(int));
        arr = (int**)malloc(totalframes*sizeof(int*));
	difference=(float*)malloc(totalframes*spf*sizeof(float));

        for(i=0;i<totalframes;i++)
                arr[i] = (int*)malloc(spf*sizeof(int));

	bin_count=(int*)malloc(noofbins*sizeof(int));
	meanpf=(float*)malloc(totalframes*sizeof(float));
	absmeanpf=(float*)malloc(totalframes*sizeof(float));
	deviationpf=(float*)malloc(totalframes*sizeof(float));
	global_deviationpf=(float*)malloc(totalframes*sizeof(float));  //for storing global deviation
	maxpf=(int*)malloc(totalframes*sizeof(int));
	minpf=(int*)malloc(totalframes*sizeof(int));

	medianpf=(float*)malloc(totalframes*sizeof(float));
	COV=(float*)malloc(totalframes*sizeof(float));
	bin_raw=(int*)malloc(noofbins*sizeof(int));

	readdata_file(fp,arr,totalframes,spf,fmt.wBitsPerSample);

	/*FILE* mypointer;
	  mypointer = fopen("raw-values","w");
	  for(i=0;i<totalframes;i++)
	  for(j=0;j<spf;j++)
	  fprintf(mypointer,"%d\n",arr[i][j]);
	  fclose(mypointer);*/

        find(mi,ma,arr,totalframes ,spf);

	max_of_speech=max_matrix(arr,totalframes,spf); 
	cout<<"maximum value in speech ="<<max_of_speech<<"\n";
	min_of_speech=min_matrix(arr,totalframes,spf);
	cout<<"minimum value in speech ="<<min_of_speech<<"\n";
	global_deviation=std_dev(arr,totalframes,spf,&global_mean);
	cout<<"standard deviation="<<global_deviation<<"\n";
	cout<<"global mean="<<global_mean<<"\n";

	
	
	

	devpf(arr,totalframes,spf,deviationpf,meanpf,absmeanpf);	
	extremumpf(arr,totalframes,spf,maxpf,minpf);
	median(arr,totalframes,spf,medianpf);



	for(i=0;i<totalframes;i++)
	  COV[i]=abs(deviationpf[i]/absmeanpf[i]);
	 
    
	//	num_outliers=outliers(arr,deviationpf,totalframes,spf,z,outlier_samples); 
	//	global_devpf(arr,totalframes,spf,global_deviationpf,global_mean);
	//	num_g_outliers=outliers(arr,global_deviationpf,totalframes,spf,z,g_outlier_samples);



	/*	for(i=0;i<totalframes;i++)  
	  outlier_frames[i]=0;

	for(i=0;i<totalframes;i++)                      
	  {
	    for(j=0;j<spf;j++)
	      {
		if(outlier_samples[i*160+j]==1)
		  {
		    outlier_frames[i]=1;
		   break;
		  }
		
	      }
	  }

       	fs=fopen("outlier_f.txt","wb");
	for(i=0;i<totalframes;i++)
	fprintf(fs,"%d \n",outlier_frames[i]);
	fclose(fs);*/


      

	//	binsize=histogram(COV,totalframes,noofbins,bin_count);   //making histogram for COV per frame

	/*	for(i=0;i<noofbins;i++)                //Calculating two outlayer deiations as per histogram
	  {	      
	    if(bin_count[i]==0)
	      {
		if(bin_count[i+1]==0)
		  {
		    outlayer_dev2=(i+2)*binsize;
		    break;
		  }
		else if(bin_count[i+1]!=0 && flag==0)
		  {
		    outlayer_dev1=(i+1)*binsize;
		    flag=1;
		  }
		
	      }
	      }*/

	/*	for(i=0;i<noofbins;i++)                //Calculating two outlayer deviations as per histogram
	  {
	    if(bin_count[i]!=0)
	      flagx=1;
	    if(flagx==1)
	      {	      
		if(bin_count[i]==0)
		  {
		    if(bin_count[i+1]==0)
		      {
			outlayer_dev2=(i+2)*binsize;
			break;
		      }
		    else if(bin_count[i+1]!=0 && flag==0)
		      {
			outlayer_dev1=(i+1)*binsize;
			flag=1;
		      }
		    
		  }
	      }
	  }



      	fs=fopen("outlier_frames1.txt","wb");
	for(i=0;i<totalframes;i++)
	  {
	    if(COV[i]>=outlayer_dev1)
	      {
		outlier_frames[i]=1;                           //outlier_frames calculated by coefficient of variation.
		fprintf(fs,"%d  %f\n",i+1,i*(.02));
	      }
	    else
	      outlier_frames[i]=0;

	  }
	  fclose(fs);
	
	  fs=fopen("outlier_frames2.txt","wb");
	  for(i=0;i<totalframes;i++)
	  {
	  if(COV[i]>=outlayer_dev2)
	  fprintf(fs,"%d \n",i+1);
	  }
	  fclose(fs);

	for(i=0;i<totalframes;i++)
	  {
	    for(j=0;j<spf;j++)
	      {
		if(outlier_frames[i]==1 && outlier_samples[(i*160)+j]==1 && g_outlier_samples[(i*160)+j]==1)
		  {
		    outlier_samples[(i*160)+j]=1;        //outlier_samples rewritten using outlier_frames from COV
		    counter++;                           // Number of final outliers
		  }
		else
		  outlier_samples[(i*160)+j]=0;
	      }
	      }*/

	/*	fs=fopen("difference.txt","wb");
	for(i=0;i<totalframes;i++)
	  for(j=0;j<spf;j++)
	    if(outlier_frames[i]==1)
	      fprintf(fs,"%d %d \n",((i*160)+j+1),(arr[(i*160)+j]-arr[(i*160)+j-1]));
	      fclose(fs);*/

   

	/*fs=fopen("final_outliers.txt","wb");

	for(i=0;i<totalframes;i++)
	  {
	    for(j=0;j<spf;j++)
	      {
		if(outlier_samples[(i*160)+j]==1 && outlier_samples[(i*160)+j-1]==0 && outlier_samples[(i*160)+j+1]==0)
		  fprintf(fs,"%d        %d   \n",i+1,((i*160)+j+1));
		if(outlier_samples[(i*160)+j]==1 && outlier_samples[(i*160)+j-1]==0 && outlier_samples[(i*160)+j+1]==1 && outlier_samples[(i*160)+j+2]==0)
		  fprintf(fs,"%d        %d   \n",i+1,((i*160)+j+1));

	      }
	      }*/





	//	for(i=0;i<totalframes*spf;i++)                       //AND operation on global and local flags.
	//	  {
	//	    if(outlier_samples[i]==1 && g_outlier_samples[i]==1)
	//	      {
	//	      and_outliers[i]=1;
	//	      counter++;
	      //	      }
	//	    else
	//	      and_outliers[i]=0;
	//	  }
	//	printf("Total outlier from and of local and global= %d\n",counter);

	
	/*	fs=fopen("outliers.txt","wb");
	for(i=0;i<totalframes*spf;i++)
	  fprintf(fs,"%d %d  %d \n",outlier_samples[i],g_outlier_samples[i],and_outliers[i]);
	  fclose(fs);


	fs=fopen("time.txt","wb");
	for(i=0;i<totalframes*spf;i++)                        //TIME
	  {
	    
	    if(and_outliers[i]==1 && check_bit==1)
	      {
		fprintf(fs,"%f  ",(float)(i+1)/8000);
		check_bit=0;
	      }
	    else if(and_outliers[i]==0 && check_bit==0)
	      {
		fprintf(fs,"to %f \n",(float)i/8000);
		check_bit=1;
	      }
	  }
	  fclose(fs); */

	

	fs=fopen("deviations.txt","wb");
	for(i=0;i<totalframes;i++)
	  fprintf(fs,"%f    %f   %f  %f  %f     %f\n",deviationpf[i],global_deviationpf[i],meanpf[i],absmeanpf[i],medianpf[i],COV[i]);
	fclose(fs);

       	




	minimum = bin(mi,totalframes,40,0);  //40 is bin size and 0 is for min values
	maximum = bin(ma,totalframes,40,1);  //40 is bin size and 1 is for min values
	      

        free(mi);free(ma);
	
    
	for(i=0,k=0;i<totalframes;i++)
		for(j=0;j<spf;j++)
			farr[k++] = arr[i][j];

	fs=fopen("first_odiff","wb");
	for(i=0;i<totalframes;i++)
	  for(j=0;j<spf;j++)
	    {
	    if(i==0 && j==0)
	      difference[i*160+j]=abs(farr[0]);
	    else
	      difference[i*160+j]=abs(farr[i*160+j]-farr[i*160+j-1]);
	    fprintf(fs,"%f\n",difference[i*160+j]);
	    }
	fclose(fs);
      

	//	fs=fopen("data","wb");

	//	clock_t start=clock();
	//	rawsize=threshold(farr,k,noofbins,bin_count,&lower_threshold,&upper_threshold,fs);
	//	fclose(fs);

	//	printf("Time Elapsed=%f seconds\n",((double)clock()-start)/CLOCKS_PER_SEC);


	//	threshold(bin_raw,rawsize,noofbins,k,&max_threshold,&min_threshold,min_of_speech,max_of_speech);

	//	printf("lower=%f ",lower_threshold);
	//	printf("upper=%f \n",upper_threshold);
	
	/*	fs=fopen("farr.txt","wb");
	for(i=0;i<k;i++)
	  if((farr[i]<lower_threshold) || (farr[i]>upper_threshold))
	    {
	      temp=(float)i/8000;
	     fprintf(fs,"%d  %f\n",i+1,temp);
	    }
	    fclose(fs);*/
	  
	binsize=histogram(farr,totalframes*spf,noofbins,bin_raw); //bin_raw contains count of values in each bin.

	for(i=0;i<noofbins;i++)
	{
	  if(i<(noofbins/2))
	    {
	     
		  if(bin_raw[i]!=0)
		    {
		      temp=(float)(bin_raw[i+1]-bin_raw[i])/bin_raw[i];
		      percent_bin_diff[i]=abs(temp*100);
		    }
		  else
		    percent_bin_diff[i]=0.0;
		
	    }
	  else
	    {
	      
		if(bin_raw[i]!=0)
		  {
		  temp=(float)(bin_raw[i-1]-bin_raw[i])/bin_raw[i];
		  percent_bin_diff[i]=abs(temp*100);
		  }
		else
		  percent_bin_diff[i]=0.0;
		
	    }
	}

	threshold(percent_bin_diff,noofbins,&max_threshold,&min_threshold,bin_raw,k,binsize,position,max_of_speech,min_of_speech);

	printf("max=%f   min=%f\n",max_threshold,min_threshold);

	for(i=0,j=0;i<k;i++)
	  if((farr[i]>max_threshold) || (farr[i]<min_threshold))
	    j++;
       
	outlier_samples=(int*)malloc(j*sizeof(long int));

       

	for(i=0,j=0;i<k;i++)
	  if(farr[i]>=max_threshold || farr[i]<=min_threshold)
	    outlier_samples[j++]=(i+1);



	interpolation(farr,outlier_samples,j);

	/*	for(i=0;i<k;i++)
	  {
	    
	    if(farr[i]>max_threshold)
	      farr[i]=max_threshold;
	    if(farr[i]<min_threshold)
	      farr[i]=min_threshold;
	      }*/

	fs=fopen("out","wb");
	for(i=0;i<j;i++)
	  fprintf(fs,"%d\n",outlier_samples[i]);
	fclose(fs);

	for(i=0;i<j;i++)
	  {
	    outlier_frames[l++]=((outlier_samples[i]-1)/160)+1;
	    counter=outlier_samples[i];
	    while(outlier_samples[i]<(counter+160))
	      i++;
	  }

	counter=0;
	/*	for(i=0;i<l;i++)
	  {
	    counter=outlier_frames[i]-1;
	    printf("%d %f %f  %d  %d  \n",outlier_frames[i],meanpf[counter],deviationpf[counter],maxpf[counter],minpf[counter]);
	  }
	*/  
	
	/*fs=fopen("newframe6941","wb");
	  for(i=0;i<spf;i++)
	  fprintf(fs,"%f \n",farr[(6940*spf)+i]);
	  fclose(fs);
	*/

	/*	l=0;
	for(i=0;i<spf;i++)
	  {
	    counter=(6940*spf)+i;
	    printf("%f %f\n",meanpf[6940]+3*deviationpf[6940],meanpf[6940]-3*deviationpf[6940]);
	    if( (farr[counter]>=(meanpf[6940]+1.6*deviationpf[6940])) || (farr[counter]<=(meanpf[6940]-1.6*deviationpf[6940])))
	      {
	    	frame_outlier[l++]=counter;
	    	printf("%d \n",counter);
	      }
	  }
	*/

	fs=fopen("outlier","wb");
	for(i=0;i<k;i++)
	  fprintf(fs,"%f\n",farr[i]);
	fclose(fs);

	a->raw_length = k;
	a->raw = farr;

      
}


