#ifndef SILENCE_FUNCTIONS_H
#define SILENCE_FUNCTIONS_H
struct hold
{
	int position;
	struct hold *next;
};
struct holding
{
        int pos;
        int count;
        struct holding *next;
};
template <class type>
void copy_vector(type *a,type *b,int start,int len);
void inserthold(struct hold **h,int pos);
void free_hold(struct hold **h);
unsigned short counting(struct hold *h);
void insert(struct holding **h,int pos,int count);
void free_holding(struct holding **h);
int* copyint(int *a, struct hold *h,int no_of_frames);
double* vector(long no_of_frames);
int* int_vector(int len);
template <class type>
void free_vector(type *v);
void calc_brkpoints(double *mean,long no_of_frames,int check,struct hold **h);
int calc_threshold(double *mean,double *var,int start, int end,int no_of_silence_frames);
void conversion(int *a,int len,int speech);
void smooth(int *a,int len,struct holding **now,int threshold);
int silencecheck(double *a,int start,int end);
#endif
