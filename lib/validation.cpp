#include <iostream>
#include "../include/header.h"
#include "../include/validation.h"
using namespace std;

void input_validation(char *path,struct totalheader *a)
{
	struct Header hdr;
	struct FormatChunk fmt;
	struct DataChunkHeader dat;

	FILE *fp;
	char format[100];
	int val;
	unsigned short timeSamplePerFrame,timeincrement;
	long samplesPerFrame,increment;

	timeSamplePerFrame =  20;// frame size in milli sec's 
	timeincrement =       10;// overlap size in milli sec's

	fp = filecheck(path);
	fread(&hdr,sizeof(struct Header),1,fp); // read the header
	if(memcmp(hdr.groupid,"RIFF",4)!=0)
	{
		fclose(fp);
		error_handler("File is not in wav format");
	}
	fread(&fmt,sizeof(struct FormatChunk),1,fp); // read fmt in header
//	hdr.type[4]=0;// appending value 0 to end of string to perform strcpy
	memcpy(format,hdr.type,4);
	format[4]='\0';
	strncat(format,fmt.chunkID,4);
	if(memcmp(format,"WAVEfmt ",8)!=0)
	{
		fclose(fp);
		error_handler("File is not in wav format");
	}
	if(fmt.chunkSize !=16)
	{
		if(fmt.chunkSize <16)
		{
			fclose(fp);
			error_handler("File is not in wav format");
		}
		val = fmt.chunkSize -16;
		fread(&format,val,1,fp);
	}
	if(fmt.wFormatTag !=1)
	{
		fclose(fp);
		error_handler("File is not in wav format");
	}
	if(fmt.wCh !=1)
	{
		fclose(fp);
		error_handler("File is not in wav format");
	}
	if(fmt.wBitsPerSample != 8 && fmt.wBitsPerSample != 16)
	{
		fclose(fp);
		error_handler("File is not in wav format");
	}
	while(1)
	{
		fread(&dat,sizeof(struct DataChunkHeader),1,fp);
		if(memcmp(dat.chunkID,"data",4)==0)
			break;
		else if(memcmp(dat.chunkID,"fact",4)==0)
		{
			if(dat.chunkSize%2 !=0)
				dat.chunkSize++;
			fread(&format,dat.chunkSize,1,fp);
		}
		else
		{
			fclose(fp);
			error_handler("File is not in wav format");
		}
	}

	if((samplesPerFrame = (long)(ceil)((timeSamplePerFrame * fmt.dwSamplesPerSec)/1000)) >(long) dat.chunkSize)
		error_handler("samples per frame is less than required");
	if((increment = (long)(ceil)((timeincrement * fmt.dwSamplesPerSec)/1000)) > (long)dat.chunkSize)
		increment = samplesPerFrame;

	mean(fp,fmt,samplesPerFrame,increment,a,dat.chunkSize/(fmt.wBitsPerSample/8));
	fclose(fp);
	a->sampfreq = fmt.dwSamplesPerSec;
	a->totalsamples = dat.chunkSize/fmt.wBlockAlign;
printf("total samplles = %d\n",a->totalsamples);
}
