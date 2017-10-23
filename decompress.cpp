#include <stdio.h>
#include "bmp_header.h"
#include <stdlib.h>
#include <string.h>

typedef struct{
	unsigned char B;
	unsigned char G;
	unsigned char R;
}pixel;
#pragma pack(1)
typedef struct{
	short ox;
	short oy;
	unsigned char r;
	unsigned char g;
	unsigned char b;
}compr;
#pragma pack()

void FILL(pixel **img, int height, int width, int x, int y, int r, int g, int b, pixel P){
	img[x][y].R = r;
	img[x][y].G = g;
	img[x][y].B = b;
	
	if((y-1) >= 0 && P.R == img[x][y-1].R && P.G == img[x][y-1].G && P.B == img[x][y-1].B)
		FILL(img, height, width, x, y - 1, r, g, b, P);
		
	if((y+1) < width && P.R == img[x][y+1].R && P.G == img[x][y+1].G && P.B == img[x][y+1].B)
		FILL(img, height, width, x, y + 1, r, g, b, P);
		
	if((x-1) >= 0 && P.R == img[x-1][y].R && P.G == img[x-1][y].G && P.B == img[x-1][y].B)
		FILL(img, height, width, x - 1, y, r, g, b, P);
		
	if((x+1) < height && P.R == img[x+1][y].R && P.G == img[x+1][y].G && P.B == img[x+1][y].B)
		FILL(img, height, width, x + 1, y, r, g, b, P);		
}


int main(void){
	char  *input, *cmpname;
	int pad,h,i,j,k,size,lin,col,aux;

	compr *compressedimage;


	cmpname=(char*) malloc(100 * sizeof(char));
	input=(char*) malloc(15 * sizeof(char));
	input= "input.txt";
	bmp_fileheader bmpfile;
	bmp_infoheader bmpinfo;
	FILE *inp=fopen(input, "rt");
	fscanf(inp,"%s\n", cmpname);
	fclose(inp);
	FILE *bmpinit=fopen(cmpname,"rt");
	fread(&bmpfile,sizeof(bmp_fileheader),1,bmpinit);
	fread(&bmpinfo,sizeof(bmp_infoheader),1,bmpinit);
	fclose(bmpinit);
	FILE *compressed=fopen(cmpname,"rb");
	
	
	pixel ** decompressed;
	
	//alocare matrice
	decompressed=(pixel**)calloc(bmpinfo.height,sizeof(pixel*));
	
	for (i=0;i<bmpinfo.height;i++)
		decompressed[i]=(pixel*)calloc(bmpinfo.width,sizeof(pixel));
	fseek(compressed,bmpfile.imageDataOffset,SEEK_SET);
	compr coord;
	
	int ** matrice;
	matrice=(int**)calloc(bmpinfo.height,sizeof(int*));
	for (i=0;i<bmpinfo.height;i++)
		matrice[i]=(int*)calloc(bmpinfo.width,sizeof(int));

			
	//citeste din fisier elementele si completeaza matricea decompressed
	while( feof(compressed) == 0 )
	{
		fread(&coord.ox, sizeof(short), 1, compressed );
		fread(&coord.oy, sizeof(short), 1, compressed );
		fread(&decompressed[bmpinfo.height-coord.ox][coord.oy-1].R,sizeof(unsigned char), 1, compressed );
		fread(&decompressed[bmpinfo.height-coord.ox][coord.oy-1].G,sizeof(unsigned char), 1, compressed );
		fread(&decompressed[bmpinfo.height-coord.ox][coord.oy-1].B,sizeof(unsigned char), 1, compressed );
		matrice[bmpinfo.height-coord.ox][coord.oy-1] = 1;

	}
	pixel  padding;
	padding.R=0;
	padding.G=0;
	padding.B=0;
int x, y;
	char koala[64]="goala_ghidus.bmp";
	FILE *out = fopen( koala, "wb");

	fwrite(&bmpfile,sizeof(bmp_fileheader),1,out);
	fwrite(&bmpinfo,sizeof(bmp_infoheader),1,out);
	fseek(out,bmpfile.imageDataOffset,SEEK_SET);
	
	//decompressed=fill(decompressed,bmpinfo.height,bmpinfo.width);
	int P=0;
	lin=3;
	/*for(x=0;x<bmpinfo.height;x++){
		for(y=0;y<bmpinfo.width;y++){
		FILL(decompressed,  bmpinfo.height, bmpinfo.width, x,  y,  decompressed[x][y].R,  decompressed[x][y].G,  decompressed[x][y].B,  decompressed[x][y]);	
		}
	}*/
	
	while(P!=bmpinfo.height){

		for(i=0;i<bmpinfo.width-1;i++){
			if(matrice[P][i+1]==0){
				decompressed[P][i+1].R=decompressed[P][i].R;
				decompressed[P][i+1].G=decompressed[P][i].G;
				decompressed[P][i+1].B=decompressed[P][i].B;
			}
		}
		P++;
	}
		
	for(i=0;i<bmpinfo.height;i++){
		pad=0;
		for(j=0;j<bmpinfo.width;j++){
			fwrite(&decompressed[i][j],sizeof(pixel),1,out);
			pad++;
		}
		if(pad%4 != 0){
			fwrite(&padding,sizeof(pixel),pad%4,out);
		}
	}



return 0;
		
}
