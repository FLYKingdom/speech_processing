#ifndef FUNCTIONS_H
#define FUNCTIONS_H

void getpath(char *spkrname,char *str_final);
void input_validation(char *path,struct totalheader *a);
void silence(char *currentfile,struct totalheader a,int val);
#endif
