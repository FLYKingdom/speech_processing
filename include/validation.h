#ifndef VALIDATION_H
#define VALIDATION_H

//	UNCOMMENT THESE LINES FOR LINUX
//Header of a WAVE file
struct Header
{
	char groupid[4];		//RIFF
	uint32_t len;			//total length of rest of file
	char type[4];			//WAVE
};
//The Format Chunk of a WAVE file
struct FormatChunk
{
	char chunkID[4];		//'fmt '
	uint32_t chunkSize;		//after this
	uint16_t wFormatTag;		//1 if uncompressed
	uint16_t wCh;			//1 for monoaural
	uint32_t dwSamplesPerSec;
	uint32_t dwAvgBytesPerSec;
	uint16_t wBlockAlign;
	uint16_t wBitsPerSample;	//8 for case we handle here
};
//The DATA header
struct DataChunkHeader
{
	char chunkID[4];		//'data'
	uint32_t chunkSize;		//of data that follows
};

void mean(FILE *fp,struct FormatChunk fmt,long spf,int inc,struct totalheader *a,long size);
#endif
