#ifndef VARIABLE_H
#define VARIABLE_H
#define no_N 10
#define no_B 5
extern int N[no_N],F[no_N],BN[no_B],BF[no_B];

void init_dist_ubm(char *fn);
void dist_ubm(char *fn,int numspk);
void initialdistance(char *);
int distance(char *filename,int numspk);
int backgrd(int numspk);
//int firstscore(int numspk);
//int score(int numspk);
int verifying(int numspk,char *fn);
int setscore(int numspk);
int setscore_ubm(char *claimant);
float verify(float **Xi,int no_samples,char *ourspkr);
float verify_now(float **Xi,int no_samples,char *ourspkr);
float verify_ubm(float **Xi,int no_samples,char *ourspkr);

#endif
