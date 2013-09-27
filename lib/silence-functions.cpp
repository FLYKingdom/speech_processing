#include "../include/header.h"
#include "../include/silence-functions.h"
using namespace std;
//const int NULL = 0;

template <class type>
void copy_vector(type *a,type *b,int start,int len)
{
	int i,j=0;
	for(i=start;i<start+len;i++)
		a[j++]=b[i];
}

void inserthold(struct hold **h,int pos)
{
	struct hold *current,*temp1,*temp;
	current = *h;
	temp = new (struct hold);
	temp->position = pos;
	temp->next = NULL;
	if(current == NULL)
		*h = temp;
	else
 	{
		do{
			temp1 = current;
			current = current->next;
		}while(current!=NULL);
		temp1->next = temp;
	}
}

void free_hold(struct hold **h)
{
	struct hold *temp,*temp1;
	temp = *h;
	while(temp!=NULL)
	{
		temp1 = temp->next;
		delete temp;
		temp = temp1;
	}
}

unsigned short counting(struct hold *h)
{
	struct hold *temp;
	unsigned short i=0;
	temp = h;
	while(temp!=NULL)
	{
		i++;
		temp = temp->next;
	}
	return i;
}
void insert(struct holding **h,int pos,int count)
{
        struct holding *current,*temp1,*temp;
        current= *h;
        temp = new (struct holding);
        temp->pos = pos;
        temp->count = count;
        temp->next = NULL;
        if(current == NULL)
        {
                *h = temp;
        }
        else
        {
                do{
                        temp1 = current;
                        current = current->next;
                }while(current!=NULL);
                temp1->next = temp;
        }
}

void free_holding(struct holding **h)
{
	struct holding *temp,*temp1;
	temp = *h;
	while(temp!=NULL)
	{
		temp1 = temp->next;
		delete temp;
		temp = temp1;
	}
}
int* copyint(int *a, struct hold *h,int no_of_frames)
{
	int i=0;
	struct hold *temp;
	temp = h;
	a[i++] = 0;
	while(temp!=0)
	{
		a[i++] = temp->position;
		temp = temp->next;
	}
	a[i]= no_of_frames;
	return a;
}

void interchange_withext(double *c,double *d,int *a,int i, int j)
{
	double m,v;
	int ind;
	m = c[i];
	c[i] = c[j];
 	c[j] = m;
	v = d[i];
	d[i] = d[j];
	d[j] = v;
	ind = a[i];
	a[i] = a[j];
	a[j] = ind;
}
int partition_withext(double *meanarray,double *vararray,int *ind,int m,int p)
{
	double pivotmean,pivotvar;
	int i,j,pivotind;
	pivotmean = meanarray[m];
	pivotvar = vararray[m];
	pivotind = ind[m];
	i =m+1;
	j=p;
	while(i<=j)
	{
		while(i<j+1 && meanarray[i]<= pivotmean)
			i++;
		while(j>i-1 && meanarray[j]>= pivotmean)
			j--;
		if(i<j)
			interchange_withext(meanarray,vararray,ind,i,j);
	}
	meanarray[m] = meanarray[j];
	meanarray[j] = pivotmean;
	vararray[m] = vararray[j];
	vararray[j] = pivotvar;
	ind[m] = ind[j];
	ind[j] = pivotind;
	return j;
}
void quicksort_withext(double *mean_array,double *var_array,int *index,int start,int end)
{
	int j;
	if(start<end)
	{
		j = partition_withext(mean_array,var_array,index,start,end);
		quicksort_withext(mean_array,var_array,index,start,j-1);
		quicksort_withext(mean_array,var_array,index,j+1,end);
	}
}
int getenergy(double *sort,int size)
{
        double avgmin,checkenergy;
        int i,pos,presentpos;
        double *velocity=NULL,*acce=NULL;

/*for(i=0;i<size;i++)
	printf("%f\n",sort[i]);*/

	presentpos = size-1;
	checkenergy = sort[0] + ((sort[size-1]-sort[0])/2);
        velocity = assign_vector(velocity,presentpos);
        acce = assign_vector(acce,presentpos-1);
        for(i=1;i<=presentpos;i++)
                velocity[i-1]=sort[i]-sort[i-1];
        for(i=1;i<=presentpos-1;i++)
                acce[i-1] = velocity[i] - velocity[i-1];
        avgmin = acce[0];pos = 0;
        for(i=1;i<presentpos-1;i++)
                if(avgmin<acce[i])
                {
//			if(sort[i] < checkenergy)
			if(sort[i] < checkenergy && i > size/20)
			{
	                        avgmin = acce[i];
        	                pos = i;
			}
                }

	release_vector(velocity);
	release_vector(acce);
//printf(" check energy = %f energ = %f\n",checkenergy,sort[pos]);
        return pos;
}
// for calucalating break points in order to divide the file at silence zones
void calc_brkpoints(double *mean,long no_of_frames,int check,struct hold **h)
{
	double *sort_mean=NULL;
	int i;
	int tpercent;// to check only 20% sorted array

	sort_mean = assign_vector(sort_mean,no_of_frames);
	copy_vector(sort_mean,mean,0,no_of_frames);
	quicksort(sort_mean,0,no_of_frames-1);
	tpercent = no_of_frames /5;// no of frames * (20/100)
/*	for(i=0;i<no_of_frames;i++)
		printf("%f\n",mean[i]);*/
//	tpercent = getenergy(sort_mean,no_of_frames);
	i =  2 * check;
	while(i < no_of_frames)
	{
		if(mean[i] <= sort_mean[tpercent])
		{
			while((i+1) < no_of_frames && mean[i+1] <= sort_mean[tpercent])
				i++;
			if((i+check) >= no_of_frames)
				break;//inserthold(h,no_of_frames);
			else
				inserthold(h,i);
			i += check;
		}
		else
			i++;
	}
	release_vector(sort_mean);
}
double getvariance(double *mean,int len)
{
	int i;
	double max,min,bin,*arr=NULL;

	arr = assign_vector(arr,len);
	for(i=0;i<len;i++)
		arr[i] = log(mean[i]);
	max = min = arr[0];
	for(i=1;i<len;i++)
		if(arr[i]< min)
			min = arr[i];
		else if(arr[i]>max)
			max = arr[i];
	if(min== max)
		return min;
	bin = min + ((max-min)/3);  // considering only first bin
	for(i=0;i<len;i++)
		if(arr[i]<bin && min < arr[i])
			min = arr[i];

	release_vector(arr);
	return (exp(min));
}

//for calucalating threshold for each section of wavefile
int calc_threshold(double *mean,double *var,int start, int end,int no_of_silence_frames)
{
	double *sort_mean=NULL,*sort_var=NULL;
	int *sort_index=NULL;
	double min_var=10000.0;
	int tpercent;// to consider 20% of sorted array
	int no_of_frames,ind,i;	
	no_of_frames = (end - no_of_silence_frames) - start +1;
	if(no_of_frames <= 5)
		return start;
   	sort_mean = assign_vector(sort_mean,no_of_frames);
   	sort_var = assign_vector(sort_var,no_of_frames);
	sort_index = assign_vector(sort_index,no_of_frames);
   	copy_vector(sort_mean,mean,start,no_of_frames);
   	copy_vector(sort_var,var,start,no_of_frames);
	for(i=0;i<no_of_frames;i++)
      		sort_index[i]=i;
	quicksort_withext(sort_mean,sort_var,sort_index,0,no_of_frames-1);
	tpercent = getenergy(sort_mean,no_of_frames);
//printf("start = %d and tpercent = %d\n",start,tpercent);
//   	tpercent = no_of_frames /5;// no of frames * (20 /100)
	if(tpercent !=0)
	{
		for(i=0;i<tpercent;i++)
			if(sort_var[i] < min_var)
			{
				min_var = sort_var[i];
				ind = sort_index[i];
			}
		min_var = getvariance(sort_var,tpercent);
		for(i= tpercent -1;i>=0;i--)
			if(sort_var[i] <= min_var)
			{
				ind = sort_index[i];
				break;
			}
	}
	else
		ind = sort_index[0];
	release_vector(sort_mean);
	release_vector(sort_var);
	release_vector(sort_index);
//	printf("ind = %d ",ind+start);
	return (ind+start);
}

// conversion of single 1's to 0's or single 0's to 1's
void conversion(int *a,int len,int speech)
{
        int i;
        for(i=0;i<len;i++)
        {
           if(i==0)
           {
                if(a[i] == !speech) //if(a[i]==0)
                {
                    if(a[i]!=a[i+1] && a[i+1]!=a[i+2])
                    {
                        a[i+1] = !speech; //a[i+1]=0;
                        i = i+2;
                    }
                }
                else
                    if(a[i]!=a[i+1] && a[i+1]!=a[i+2])
                    {
                        a[i] = !speech; //a[i]=0;
                        i = i+1;
                    }
            }
            else
            {
                if(a[i] ==  speech) //if(a[i]==1)
                    if(a[i-1]!=a[i] && a[i]!=a[i+1])
                    {
                        a[i] = !speech; //a[i]=0;
                        i = i+1;
                    }
            }
        }
}
struct holding* leftiteration(struct holding **find, int *a,int threshold) // going towards left
{
	int count_of_zero,count_of_one,present,count;
    	struct holding *temp;
    	temp = *find;
	while(1)
	{
    		present = temp->pos;
		if(present == 0)
			break;
		count_of_zero = 0;
		while( present -1 >=0 && a[present-1]!=1)
		{
        		count_of_zero++;
			present--;
		}
		count_of_one = 0;
		while(present-1 >= 0 && a[present-1]!=0)
		{
			count_of_one++;
			present--;
		}
		if((count_of_zero < threshold && count_of_one >= ceil(threshold/2))||(count_of_zero <threshold && present==0))
//    		if(count_of_zero < threshold && count_of_one >= ceil(threshold/2))
	    	{
			count = count_of_zero + count_of_one;
        		temp->count += count;
			temp->pos = present;
			present += count_of_one;
	        	while(count_of_zero--!=0)
        	    		a[present++] = 1;
	    	}	
		else
			break;
	}
	return temp;
}

struct holding* rightiteration(struct holding **find, int *a,int threshold,int len) // going towards right
{
	int count_of_zero=0,count_of_one=0,present,count;
    	struct holding *temp,*temp1;
    	temp = *find;
    	present = temp->pos + temp-> count;
    	if(present == len) // final or end condition
		return temp->next;
    	while(present != len && a[present] != 1 )
	{
        	count_of_zero++;
		present++;
	}
//    	present -= 1; // back to end of 0's
    	if(count_of_zero < threshold)
    	{
		if(present < len)
		{
			count_of_one = 0;
			do{
				count_of_one++;
				present++;
			}while(present!=len && a[present]!=0);
			if(count_of_one < ceil(threshold/2))
				return (temp->next);
			else
			{
				if(count_of_one >= threshold)
				{
					temp1 = temp->next;
					temp->next = temp1->next;
					delete temp1;
				}
				count = count_of_zero + count_of_one;
				temp->count += count;
				present -= count;
				while(count_of_zero--!=0)
					a[present++]=1;
			}
		}
		else
		{
			count = count_of_zero;
			temp->count += count_of_zero;
			while(count_of_zero--!=0)
				a[--present] = 1;
		}
		return temp;
	}
	else
		return (temp->next);
}

void smooth(int *a,int len,struct holding **now,int threshold)
{
	struct holding *temp;
    	temp = *now;
    	while(temp!=NULL)
    	{
        	if(temp->pos == 0) // if position is left end
        	{
			temp = rightiteration(&temp,a,threshold,len);
        	}
        	else     // (present!=0) if position is not the left end
        	{
            		// going towards left
			temp = leftiteration(&temp,a,threshold);
            		// going towards right
			temp = rightiteration(&temp,a,threshold,len);
        	}
    	}
}
int silencecheck(double *a,int start,int end)
{
        int nf,i;
        double *sort_mean=NULL;
        double max,min,diff,bin[2];
        int count[3] = {0};

        nf = end - start;
        sort_mean = assign_vector(sort_mean,nf);
        copy_vector(sort_mean,a,start,nf);

        max = min = sort_mean[0];
        for(i=1;i<nf;i++)
                if(sort_mean[i] <min)
                        min = sort_mean[i];
                else if(sort_mean[i] > max)
                        max = sort_mean[i];
	if(max-min <10)
		return 0;
        diff = (max-min)/3;
        bin[0] = min + diff;
        bin[1] = min + 2 * diff;
        for(i=0;i<nf;i++)
                if(sort_mean[i] < bin[0])
                        count[0]++;
                else if(sort_mean[i] < bin[1])
                        count[1]++;
                else
                        count[2]++;
	release_vector(sort_mean);
//printf("%d %d %d ",count[0],count[1],count[2]);
//      if(count[0] > count[1] && count[0] > count[2])
//	if(count[0] > count[1] + count[2])
	if(20 > count[1] + count[2])
                return 0;
        else
                return 1;
}

int getvalue(double *mean,int len)
{
	int i;
	float percent1,percent2,percent3;
	double max,diff,min,bin[2];
	int count[3] ={0};

	max = min = mean[0];
	for(i=1;i<len;i++)
		if(mean[i] < min)
			min = mean[i];
		else if(mean[i] > max)
			max = mean[i];
	diff = (max-min)/3;
	bin[0] = min + diff;
	bin[1] = min +2*diff;
	for(i=0;i<len;i++)
		if(mean[i] <bin[0])
			count[0]++;
		else if(mean[i]<bin[1])
			count[1]++;
		else
			count[2]++;
	percent1 = (float)(count[0])/(float)(count[0]+count[1]+count[2]);
	percent2 = (float)(count[1])/(float)(count[0]+count[1]+count[2]);
	percent3 = (float)(count[2])/(float)(count[0]+count[1]+count[2]);
//	printf("%d \t%d \t%d \t%f \t%f \t%f \t",count[0],count[1],count[2],percent1,percent2,percent3);
//	printf("%f %f %f %f %f\n",min,bin[0],bin[1],max,diff);
	return 1;
}

/*int getenergy(double *sort,int size)
{
	double diff,energy;
	int norm=64;
	int *count,ind,i;
	float val,val1;

	count = assign_vector(count,norm);
	diff = (sort[size-1] - sort[0]) / norm;
	for(i=0;i<norm;i++)
		count[i] =1;

	for(i=0;i<size;i++)
	{
		ind =(int)floor((sort[i]-sort[0])/diff);
		count[ind]++;
	}
	ind=0;
	val = ((float)(count[1]-count[0]))/count[0];
	for(i=1;i<norm-1;i++)
	{
		val1 =((float) count[i+1]-count[i])/count[i];
		if(val<val1)
		{
			if(i < (.8*size))
			{
				ind = i;
				val = val1;
			}
		}
	}
	energy = sort[0] + (diff*ind);
	i=0;
	while(energy > sort[i])
		i++;
	release_vector(count);
	return i;
}*/
