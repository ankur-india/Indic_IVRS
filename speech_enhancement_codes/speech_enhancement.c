#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include "fft.c"

#define getch getchar
double PI25 = 3.141592653589793238462643;

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

float polar_complex(float y,float x)
{
	if(x>0)
		return atan(y/x);
	else if(x<0 && y>=0)
		return PI25 + atan(y/x);
	else if(x<0 && y<0)
		return -PI25 + atan(y/x);
	else if(x==0 && y>0)
		return PI25;
	else if(x==0 && y<0)
		return -PI25;
	else
		return 0;
}

float *hamming(float window[], int w_size)
{
	int n;
	for(n=0;n<w_size;n++)
		window[n]=0.54-0.46*cos(2*PI25*n/(w_size-1));
	return window;
}

float max(float x,float y)
{
	if(x>=y)
		return x;
	else
		return y;
}

float min(float x,float y)
{
	if(x<=y)
		return x;
	else
		return y;
}

float vad(float signal[], float noise[], int W, int *noise_flag, int *speech_flag, int *noise_counter)
{
	float Dist = 0;
	float Gamma = 1.1;
	float NoiseMargin = 3;
	int Hangover = 8;
	
	int i,j;
	float SpectralDist[W],noisy[W];
	
	for(i=0;i<W;i++){
		signal[i] = pow(signal[i],1/Gamma);
		noisy[i] = pow(noise[i],1/Gamma);
		SpectralDist[i] = 20*(log10(signal[i]) - log10(noisy[i]));
		SpectralDist[i] = max(SpectralDist[i],0);
		Dist = Dist + SpectralDist[i];
	}

	Dist = Dist/W;
 
	if (Dist < NoiseMargin){
		*noise_flag = 1; 
		*noise_counter = *noise_counter + 1;
	}
	else
	{
		*noise_flag = 0;
		*noise_counter = 0;
	}

	// Detect noise only periods and attenuate the signal     
	if (*noise_counter > Hangover) 
		*speech_flag = 0;    
	else 
		*speech_flag = 1; 

	return Dist;
}


int main(int argc, char *argv[]){
	if ( argc != 3 ) /* argc should be 3 for correct execution */
	{
		/* We print argv[0] assuming it is the program name */
		printf( "Usage: %s <noisy_wavfile> <enhanced_wavfile>\n", argv[0] );
	}
	else
	{
		int BUFSIZE,i;
		int no_of_samples;
		int fs;
		int *buffer;
		float *s;

		// used for processing 16-bit file
		short int *inbuff16;

		// signal reading
		FILE *infile = fopen(argv[1], "rb");
		FILE *outfile = fopen(argv[2], "wb");

		//points to a header struct that contains the file's metadata fields
		header_p meta = (header_p)malloc(sizeof(header));

		fread(meta, 1, sizeof(header), infile);

		//reading the remaining BUFSIZE from header format(Chunk_size)
		BUFSIZE = meta->chunk_size - 36;
		no_of_samples= BUFSIZE/2;
		fs = meta->sample_rate;

		printf("chunk_id: %s\n",meta->chunk_id);
		printf("chunk_size: %d\n",meta->chunk_size);
		printf("format: %s\n",meta->format);
		printf("subchunk1_id: %s\n",meta->subchunk1_id);
		printf("subchunk1_size: %d\n",meta->subchunk1_size);
		printf("subchunk2_id: %s\n",meta->subchunk2_id);
		printf("subchunk2_size: %d\n",meta->subchunk2_size);
	
		buffer=(int *)malloc(sizeof(int)*no_of_samples);
		s=(float *)malloc(sizeof(float)*no_of_samples);
		inbuff16=(short *)malloc(sizeof(float)*no_of_samples);
		fread(inbuff16, 1, BUFSIZE, infile);

		for(i=0;i<no_of_samples;i++){
			buffer[i]=(inbuff16[i]);
			s[i]=buffer[i];        //Normalising the Amplitude(buffer) levels of a signal
		}

		if (infile) { fclose(infile); }


		float IS = 0.25;                      // Initial Silence Region in seconds (250ms->23 frames)
		//	int W = floor(0.025*fs);               // window length 25ms == 400 samples
		int W=256;

		float sp = 0.5;                      // shift percentage

		int NIS = floor((IS*fs-W)/(sp*W)) + 1;
		float gamma = 1.1;                  // 1 for magnitude SS & 2 for power SS

		int w_size = W;                     // change window length here
		float *window, w[w_size];

		window = hamming(w,w_size);

		int L = no_of_samples;
		sp = floor(W*sp);
		int N = floor((L-W)/sp+1);

		int j=0,k=0;
		float seg[N][W];

		for(i=0;i<N;i++){
			for(j=0;j<W;j++)
				seg[i][j] = s[k+j]*window[j]/32768;
			k=k+sp;
		}

		printf("NIS: %d SP: %f Frames: %d\n",NIS,sp,N);	

		float ff[N][W][2];

		double (*x1)[2];         // pointer to time-domain samples 
		double (*X1)[2];         // pointer to frequency-domain samples 
		double dummy=0;            // scratch variable 
		char file[FILENAME_MAX];  // name of data file 
		FILE *chukka;

		x1 = malloc(2 * W * sizeof(double));
		X1 = malloc(2 * W * sizeof(double));

		for(i=0;i<N;i++){
			for(j=0;j<W;j++){
				x1[j][0] = seg[i][j];
				x1[j][1] = dummy;
			} 
			fft(W, x1, X1);
			for(k=0;k<W;k++){
				ff[i][k][0] = X1[k][0];
				ff[i][k][1] = X1[k][1];
			}
		}

		float Xmag[N][W],mag[N][W],phas[N][W],par_mag[N][W/2];

		for(i=0;i<N;i++){
			for(j=0;j<W;j++){
				mag[i][j] = pow(sqrt(pow(ff[i][j][0],2)+pow(ff[i][j][1],2)),gamma);
				phas[i][j] = polar_complex(ff[i][j][1],ff[i][j][0]);
				if(j<W/2)
					par_mag[i][j] = mag[i][j];
			}
		}

		int numberOfFrames = N;
		int FreqResol = W/2;

		float noise[W/2];
		for(i=0;i<W/2;i++){
			noise[i] = 0;
			for(j=0;j<NIS;j++)
				noise[i] = noise[i] + mag[j][i];       	// initial noise spectrum mean
			noise[i] = noise[i]/NIS;
		}

		int NoiseCounter = 0;
		int NoiseLength = 9;  		// This is a smoothing factor for the noise updating
		float Beta = .03;
		float minalpha = 1;
		float maxalpha = 5;
		float minSNR = -5;
		float maxSNR = 20;
		float alphaSlope = (minalpha-maxalpha)/(maxSNR-minSNR);
		float alphaShift = maxalpha-alphaSlope*minSNR;

		float alpha[W/2],SNR[W/2],D[W/2],BN[W/2];
		for(i=0;i<W/2;i++)
			BN[i] = Beta*noise[i];

		int NoiseFlag, SpeechFlag;
		float Dist;

		for(i=0;i<numberOfFrames;i++) {
			Dist = vad(par_mag[i], noise, W/2, &NoiseFlag, &SpeechFlag, &NoiseCounter);  	//Magnitude Spectrum Distance VAD
			if(SpeechFlag==0){
				for(j=0;j<W/2;j++){
					noise[j] = (NoiseLength*noise[j]+par_mag[i][j])/(NoiseLength+1); 			// Update and smooth noise
					BN[j] = Beta*noise[j];
				}
			}

			for(j=0;j<W/2;j++){
				SNR[j] = 10*log10(par_mag[i][j]/noise[j]);
				alpha[j] = alphaSlope*SNR[j] + alphaShift;
				alpha[j] = max(min(alpha[j],maxalpha),minalpha);
				D[j] = par_mag[i][j] - (alpha[j]*noise[j]);    		// Nonlinear (Non-uniform) Power Specrum Subtraction
				Xmag[i][j] = max(D[j],BN[j]); 				// if BY>D X=BY else X=D which sets very small values of subtraction
				Xmag[i][j] = pow(Xmag[i][j],1/gamma);
			}
		}

		// reconstruction of signal using ifft

		int sig_len = N*128 + 256 - 128;
		float recon_signal[sig_len];

		for(i=0;i<sig_len;i++)
			recon_signal[i]=0;	

		int flag;
		k=0;
		for(i=0;i<N;i++){
			flag = 128;	
			for(j=0;j<W;j++){
				if(j<W/2){
					X1[j][1] = Xmag[i][j]*sin(phas[i][j]);
					X1[j][0] = Xmag[i][j]*cos(phas[i][j]);
				}
				else{
					X1[j][1] = -Xmag[i][--flag]*sin(phas[i][j]);
					X1[j][0] = Xmag[i][flag]*cos(phas[i][j]);
				}
				x1[j][0] = x1[j][1] = 0;
			}

			ifft(W, x1, X1);

			for(j=0;j<W;j++)
				recon_signal[k+j] = recon_signal[k+j] + x1[j][0];

			k = k + 128;
		}

		short int *outbuff16; 
		outbuff16=(short *)malloc(sizeof(float)*sig_len);
		for(i=0;i<sig_len;i++){
			outbuff16[i] = recon_signal[i] * 32768;
			printf("%d %f ",outbuff16[i],recon_signal[i]);
		}

		printf("output signal length: %d\n",sig_len);

		int output_bufsize = sig_len*2;

		meta->chunk_size = output_bufsize + 36;
		meta->subchunk2_size = output_bufsize;

		fwrite(meta, 1, sizeof(header), outfile);
		fwrite(outbuff16, 1, output_bufsize, outfile);

		printf("Output successfully written to %s !! \n",argv[2]);
		if (outfile) { fclose(outfile); }
		if (meta) { free(meta); }

		free(x1);
		free(X1); 

		return 0;

	}
}
