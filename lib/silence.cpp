#include "../include/header.h"
#include "../include/mean.h"
#include "../include/silence-functions.h"
using namespace std;

#define BYTE 8
#define timeSilenceZone 100

void silence(char *currentfile,struct totalheader a,int val)
{
	FILE *fp,*sp;
	double threshold; // used for holding threshold to set 1's and 0's for each frame
	char content[50];
	long spf,nfsilenceframes,nf_speech;
	long noofframes,inc,count;
	float speech_len;
	unsigned short breaktime,checkframeno,nfbreakpoints;
	unsigned short time_inc;
	double *energy=NULL,*mean_sil_frame_energy=NULL,*var_sil_frame_energy=NULL;
	double sum_inc;
	struct hold *head=NULL;
	struct holding *starting = NULL;
	int *index=NULL,*value=NULL,ind,speech,min_speech_unit_time,pos; 
	int i=0,j,k,initial,check;
	int writingsamples,initialsamp,finalsamp;
	unsigned short time_spf;

	time_spf 		= 20;// frame size in milli sec's 
	time_inc 		= 10;// overlap size in milli sec's
	breaktime 		= 1;// breakpoint time after which we have to check for each time
	min_speech_unit_time 	= 60; //minimum time unit in milli sec's that consitutes speech
	speech 			= 0; // whether noised speech or not
	cout<<"(from silence.cpp)breaktime="<<breaktime<<"\n";
	/* Get the data size(fmt.dwSamplesPerSec from the wave file header Also evaluate our
	constants in terms of frames. Checkframeno- min no. of frames to mark the breakpoint
	(dynamic thresholding) nf_speech= min number of frames constituing speech/silence
	(used for smoothening) nfsilenceframes no of frames for running average of energy
	calculation*/
	spf = (long int)(ceil) (time_spf * a.sampfreq)/1000;
	inc = (long int)(ceil) (time_inc * a.sampfreq)/1000;
	checkframeno = (((breaktime * 1000) - time_spf) / (time_spf - time_inc)) + 1;
	nf_speech = ((min_speech_unit_time - time_spf)  / (time_spf - time_inc)) + 1;
	nfsilenceframes = ((timeSilenceZone - time_spf) / (time_spf - time_inc)) + 1;
	


	cout<<"samples per frame"<<spf<<"\n";
	cout<<"inc="<<inc<<"\n";

	noofframes = (a.raw_length-spf)/(spf-inc)+1;
	

	energy = assign_vector(energy,noofframes);
	energy[0] = 0.0; k= 0;
	sum_inc = 0.0;

       	
	for(i=0,k=0;i<(spf-inc);i++,k++)
	  {
		sum_inc += a.raw[k]*a.raw[k];
		
	  }
	cout<<"sum_inc="<<sum_inc<<"\n";
	for(i=0;i<noofframes;i++)
	{
		energy[i] = sum_inc;
		sum_inc = 0.0;
		for(j=0;j<inc;j++,k++)
			sum_inc += a.raw[k]*a.raw[k];
		
		energy[i] += sum_inc;
	      	energy[i] /= (float)spf;
		energy[i] = 10 * log10(energy[i]);	
		
		
	}
	
       	cout<<"no of frames"<<noofframes<<"\n";
	value = assign_vector(value,noofframes);
  
	mean_sil_frame_energy=assign_vector(mean_sil_frame_energy,noofframes-nfsilenceframes+1);
	var_sil_frame_energy=assign_vector(var_sil_frame_energy,noofframes-nfsilenceframes+1);
	for(i=0;i<noofframes-nfsilenceframes+1;i++)
	{
		mean_sil_frame_energy[i]=0.0;
		for(j=i;j<i+nfsilenceframes;j++)
			mean_sil_frame_energy[i] += energy[j];
		mean_sil_frame_energy[i] /= (float)nfsilenceframes;
	}
        for(i=0;i<noofframes-nfsilenceframes+1;i++)
        {
   		var_sil_frame_energy[i]=0.0;
      	        for(j=i;j<i+nfsilenceframes;j++)
           		var_sil_frame_energy[i] += pow(energy[j] - mean_sil_frame_energy[i],2);
//           		var_sil_frame_energy[i] += (energy[j] - mean_sil_frame_energy[i]) * (energy[j] - mean_sil_frame_energy[i]);
	    	var_sil_frame_energy[i] /= (float)nfsilenceframes;
        }

	calc_brkpoints(mean_sil_frame_energy,noofframes-nfsilenceframes,checkframeno,&head);

	nfbreakpoints = counting(head);
	index = assign_vector(index,nfbreakpoints+2);// since addding 0 and noofframes
	index = copyint(index,head,noofframes-nfsilenceframes);
	free_hold(&head);

	j=0;pos = 1;
	while(index[j]!=noofframes-nfsilenceframes) 
	{
//		pos = silencecheck(energy,index[j],index[j+1]);
//		pos = silencecheck(mean_sil_frame_energy,index[j],index[j+1]);
		if(pos)
		{
			ind = calc_threshold(mean_sil_frame_energy,var_sil_frame_energy,index[j],index[j+1],nfsilenceframes);
			threshold = energy[ind] ;
			for(k=ind; k<ind+nfsilenceframes-1;k++)
				if(energy[k] > threshold)
					threshold = energy[k];
		}
		else
			threshold = 1000000.0;
		j++;
//		printf(" threshold = %f for frames %d to %d\n",threshold,index[j-1],index[j]);
		for(k=index[j-1]; k<index[j]+nfsilenceframes; k++)
			if(energy[k] <= threshold)
				value[k] = 0;
			else
				value[k] = 1;
	}

	conversion(value,noofframes,speech); // converting single 1's to 0's or 0's to 1's

	i=0;//threshold=3;
	while(i<noofframes)
	{
    		if(value[i]==0)
        	{
       			while(value[i] !=1)
			{
            			i++;
				if(i == noofframes)
					break;
			}
        	}
        	else
        	{
        		pos = i;
            		count = 0;
		        while (value[i]!=0 )
            		{
             			count++;
                		i++;
				if(i == noofframes)
					break;
            		}
            		if(count >= nf_speech)
				insert(&starting,pos,count);
        	}
	}
	smooth(value,noofframes,&starting,nf_speech);
	free_holding(&starting);
	// changing a club of 0's less than speech unit into 1's
	i=0;
	while(i<noofframes)
    	{
		count = 0;
        	while(value[i] != 1)
		{
        		i++;
			if(i == noofframes)
				break;
		}
		if(i!=noofframes)
		{
        		while(value[i] != 0)
			{
            			count++;
				i++;
				if(i == noofframes)
					break;
			}
		}
        	if(count < nf_speech )
        		while(count != 0)
            			value[i-(count--)] = 0;
    	}

	fp = filecheckw("sec.txt");
	sp = filecheckw("sam.txt");
	i=0;
	writingsamples = spf - inc;
	while(i<noofframes)
	{
		initial = i;
		switch(value[i])
		{
			case 1: check = 0; strcpy(content,"speech");  break;
			case 0: check = 1; strcpy(content,"silence"); break;
		}
		count = 0;
		while((i<noofframes) && (value[i++] !=check))
			count++;

		initialsamp = ( initial * writingsamples) + 1;
		finalsamp = (initial + count) * writingsamples;
		if(initial == 0)
		{
			fprintf(sp,"1\t");
			fprintf(fp,"0.0\t");
		}
		else
		{
			fprintf(sp,"%d\t",initialsamp);
			fprintf(fp,"%f\t",((float)(initialsamp-1)/(float)a.sampfreq)+.001);
		}
		if(finalsamp >= a.totalsamples)
			finalsamp = a.totalsamples;
		fprintf(sp,"%d\t%s\n",finalsamp,content);
		fprintf(fp,"%f\t%s\n",(float)finalsamp/(float)a.sampfreq,content);
		if( i != noofframes)
			i--;
	}
	fclose(fp);
	fclose(sp);

	for(i=0,count=0;i<noofframes;i++)
		count += value[i];
	speech_len  = ((float)count * 10)/1000;
	if(val == 1)
	{
		if(speech_len < 12.0)
		{
			fp = filechecka("files_lessdata.txt");
			fprintf(fp,"%s file has %f speech required is 24 sec\n",currentfile,speech_len);
			fclose(fp);
			error_handler("file doesn't hav enough data to process");
		}
	}
	else
	{
		if(speech_len < 1.0)
		{
			fp = filechecka("testfiles_lessdata.txt");
			fprintf(fp,"%s file has %f speech required is 1 sec\n",currentfile,speech_len);
			fclose(fp);
			error_handler("file doesn't hav enough data to process");
		}
	}
	release_vector(energy);
	release_vector(value);
	release_vector(index);
	release_vector(mean_sil_frame_energy);
	release_vector(var_sil_frame_energy);
	
}
