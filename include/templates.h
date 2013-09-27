#ifndef TEMPLATES_H
#define TEMPLATES_H
template <class type1,class type2>
type1* assign_vector(type1 *myvec,type2 len)
{
	type1 *vec;
	vec= new type1[len];
	return vec;
}
template <class type1,class type2>
type1** assign_vector(type1 **myvec,type2 len,type2 breadth)
{
	type1 **vec;
	type2 i;
	
	vec = new type1*[len];
	for(i=0;i<len;i++)
		vec[i] = new type1[breadth];
	return vec;
}		
template <class type1,class type2,class type3>
type1*** assign_vector(type1 ***myvec,type2 len,type2 breadth,type3 height)
{
	type1 ***vec;
	type2 i,j;
	
	vec = new type1**[len];
	for(i=0;i<len;i++)
	{
		vec[i] = new type1*[breadth];
		for(j=0;j<breadth;j++)
			vec[i][j] = new type1[height];
	}
	return vec;
}		
template <class type>
void release_vector(type *myvec)
{
	delete []myvec;
}
template <class type1,class type2>
void release_vector(type1 **myvec,type2 len)
{
	type2 i;
	for(i=0;i<len;i++)
		delete []myvec[i];
	delete []myvec;
}
template <class type1,class type2,class type3>
void release_vector(type1 ***myvec,type2 len,type3 breadth)
{
	type2 i,j;
	for(i=0;i<len;i++)
	{
		for(j=0;j<breadth;j++)
			delete []myvec[i][j];
		delete []myvec[i];
	}
	delete []myvec;
}
template <class type1,class type2>
void initialize_matrix(type1 *cost_mat,type1 val,type2 len)
{
	type2 i;
	for(i=0;i<len;i++)
		cost_mat[i] = val;
}
template <class type1,class type2,class type3>
void initialize_matrix(type1 **cost_mat,type1 val,type2 len,type3 breadth)
{
	type2 i;
	type3 j;
	for(i=0;i<len;i++)
		for(j=0;j<breadth;j++)
			cost_mat[i][j] = val;
}
template <class type1,class type2>
void copy_matrix(type1 *mat1,type1 *mat2,type2 len)
{
	type2 i;
	for(i=0;i<len;i++)
		mat1[i] = mat2[i];
}
template <class type1,class type2>
void copy_matrix(type1 **mat1,type1 **mat2,type2 len,type2 breadth)
{
	type2 i,j;
	for(i=0;i<len;i++)
		for(j=0;j<breadth;j++)
			mat1[i][j] = mat2[i][j];
}
template <class type>
void interchange(type *c,int i, int j)
{
        type m;
        m = c[i];
        c[i] = c[j];
        c[j] = m;
}
template <class type>
int partition(type *meanarray,int m,int p)
{
        type pivotmean;
        int i,j;
        pivotmean = meanarray[m];
        i =m+1;
        j=p;
        while(i<=j)
        {
                while(i<j+1 && meanarray[i]<= pivotmean)
                        i++;
                while(j>i-1 && meanarray[j]>= pivotmean)
                        j--;
                if(i<j)
                        interchange(meanarray,i,j);
        }
        meanarray[m] = meanarray[j];
        meanarray[j] = pivotmean;
        return j;
}
template <class type>
void quicksort(type *mean_array,int start,int end)
{
        int j;
        if(start<end)
        {
                j = partition(mean_array,start,end);
                quicksort(mean_array,start,j-1);
                quicksort(mean_array,j+1,end);
	}
        
}

template <class type1,class type2>
void interchange_withindex(type1 *c,type2 *a,int i, int j)
{
	double m;
	int ind;
	m = c[i];
	c[i] = c[j];
 	c[j] = m;
	ind = a[i];
	a[i] = a[j];
	a[j] = ind;
}
template <class type1,class type2>
int partition_withindex(type1 *meanarray,type2 *ind,int m,int p)
{
	double pivotmean;
	int i,j,pivotind;
	pivotmean = meanarray[m];
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
			interchange_withindex(meanarray,ind,i,j);
	}
	meanarray[m] = meanarray[j];
	meanarray[j] = pivotmean;
	ind[m] = ind[j];
	ind[j] = pivotind;
	return j;
}
template <class type1,class type2>
void quicksort_withindex(type1 *mean_array,type2 *index,int start,int end)
{
	int j;
	if(start<end)
	{
		j = partition_withindex(mean_array,index,start,end);
		quicksort_withindex(mean_array,index,start,j-1);
		quicksort_withindex(mean_array,index,j+1,end);
	}
}

template <class type>
type median(type *arr,int len)
{
	int i;
	type *myarr,ret;

	myarr = new type[len];
	for(i=0;i<len;i++)
		myarr[i] = arr[i];
	quicksort(myarr,0,len-1);
	i = len/2;
	if((len % 2) ==0)
		ret = (myarr[i] + myarr[i-1])/2;
	else
		ret = myarr[i+1];
	delete []myarr;
	return ret;
}
	
template <class type>
type hist_vector(type *arr,int &present_len,int &binsize)
{
	type minval,maxval,diff;
	type comparemin,comparemax;
	int i,j,count;
	int *bin;

	bin = new int[binsize];
	i = 0;
	minval =maxval = arr[i++];
	while(i<present_len)
	{
		if(minval > arr[i])
			minval = arr[i];
		else if(maxval < arr[i])
			maxval = arr[i];
		i++;
	}
	diff = abs((maxval-minval)/binsize);
	count = 0;
	for(i=0;i<binsize-1;i++)
	{
		bin[i] = 0;
		comparemin = minval+(i*diff);
		comparemax = comparemin + diff;
		for(j=0;j<present_len;j++)
			if(arr[j] >= comparemin && arr[j] < comparemax)
				(bin[i])++;
		count += bin[i];
	}
	bin[i] = present_len - count;
	return (minval+diff);
}
template<class type>
type mymax(type a,type b)
{
	return (a)>(b)?(a):(b);
}

template<class type>
type mymin(type a,type b)
{
	return (a)<(b)?(a):(b);
}

template<class type>
type maxi(type *arr,int n)
{
	type max=arr[0];
	for(int i=1;i<n;i++)
		if(max<arr[i])
			max=arr[i];
	return max;
}

template<class type>
type mini(type *arr,int n)
{
	type min=arr[0];
	for(int i=1;i<n;i++)
		if(min>arr[i])
			min=arr[i];
	return min;
}

template<class type>
void myswap(type &a,type &b)
{
	type tmp=a;
	a=b;
	b=tmp;
}

#endif
