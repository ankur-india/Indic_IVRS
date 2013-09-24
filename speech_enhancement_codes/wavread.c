#include<stdio.h>
#include<string.h>
#include<stdlib.h>

typedef struct header
{
	char chunk_id[4];
	int chunk_size;
	char format[4];
	char subchunk1_id[4];
	int subchunk1_size;
	short int audio_format;
	short int num_channels;
	int sample_rate;
	int byte_rate;
	short int block_align;
	short int bits_per_sample;
	char subchunk2_id[4];
	int subchunk2_size;
} header;

typedef struct header* header_p;


int main(int argc, char *argv[]){
	if ( argc != 3 ) /* argc should be 3 for correct execution */
	{
		/* We print argv[0] assuming it is the program name */
		printf( "Usage: %s <input_wavfile> <output_textfile>\n", argv[0] );
	}
	else
	{
		int BUFSIZE,i;
		int no_of_samples;
		int FS;
		int *buffer;
		float *s;

		// used for processing 16-bit file
		short int *inbuff16;

		// signal reading
		FILE *infile = fopen(argv[1], "rb");
		FILE *outfile;

		outfile = fopen(argv[2],"w");

		//points to a header struct that contains the file's metadata fields
		header_p meta = (header_p)malloc(sizeof(header));

		fread(meta, 1, sizeof(header), infile);

		//reading the remaining BUFSIZE from header format(Chunk_size)
		BUFSIZE = meta->chunk_size - 36;
		no_of_samples= BUFSIZE/2;
		FS = meta->sample_rate;
		//	printf("no. of samples: %d\n",no_of_samples);
		//	printf("Sampling Frequency: %d\n",FS);

		buffer=(int *)malloc(sizeof(int)*no_of_samples);
		s=(float *)malloc(sizeof(float)*no_of_samples);
		inbuff16=(short *)malloc(sizeof(float)*no_of_samples);
		fread(inbuff16, 1, BUFSIZE, infile);

		for(i=0;i<no_of_samples;i++){
			buffer[i]=(inbuff16[i]);
			fprintf(outfile,"%d\n",inbuff16[i]);
			s[i]=buffer[i]/32768;        //Normalising the Amplitude(buffer) levels of a signal
		}

		if (infile) { fclose(infile); }
		if (outfile) { fclose(outfile); }
		if (meta) { free(meta); }

		return 0;
	}
}

