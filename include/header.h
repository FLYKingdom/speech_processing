#ifndef HEADER_H
#define HEADER_H
#define WINDOWS 0 // 1 == FOR WINDOWS VERSION AND 0 = LINUX VERSION
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <sys/stat.h>
#include <sys/types.h>
// comment the following 2 lines for windows
#include <stdint.h>
#include <dirent.h>

#include "templates.h"

struct totalheader
{
	char filename[1000];
	int sampfreq;
	float *raw;
	unsigned long raw_length;
	long totalsamples;
	int fftorder;
	char window_type[40];
	int framesize;
	int windowsize;
	char processframes[40];
	float highfreq;
	float lowfreq;
	float nearlow;
	float nearhigh;
	int breadth;
	int numbanks;
	int deltavalue;
	int numoffiles;
	int no_of_frames;
	int train_frames;
	int final_banks;
	int peak_len;
	int num_of_mixtures;

	short mfcc_type;
	short spec_sub;
	short cms;
	short cnv;
	short mobile;
	short recorder;
	short gui;
	
	int *ident;
	float **values;
	float *frequency;
	float **mfcc_coef;
	float **avg_mfcc;
	float **delta_coef;
	float **accel_coef;
	double *energy;
	float **path;
};

extern char MYPATH[10000];
extern int DELVALUE;
extern char testfiledir[10000];
extern char modeldir[10000];
extern char outdir[10000];
extern char trainfiledir[10000];
extern char hmmdir[10000];
extern char spkrlist[10000];
extern char distance_dist[10000];
extern char score_dist[10000];
extern char ubm_dist[10000];
extern char list_of_testdata[10000];
extern char temppath[10000];
extern char confpath[10000];

void set_path();
void error_handler(const char *str);	
void read_header(struct totalheader *a);
void print_header(struct totalheader a);
void calc_header_values(struct totalheader *a);

// this function gets the file name
extern void getfilename(char *filename,char *wv);
void getname(int count,char *fn,char *list);
void getidentity(int *id,int size);

FILE *filecheckw(const char *s);
FILE *filecheck(const char *s);
FILE *filechecka(const char *s);
FILE *filecheckab(const char *s);

void appendheader(const char *filename,FILE *fp);
float rounding(float val);

double get_median(double *vec,int len);
void mylog(float *data, int N);
void cube_root(float *data, int N);
#endif
