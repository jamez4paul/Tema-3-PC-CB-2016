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

void mfree(pixel **img,int height)
{
	int l;
	for(l=0;l<height;l++)
		free (img[l]);
	free(img);
	img=NULL;
}

pixel ** blackandwhite(pixel **img,  int height, int width)
{
	int i,j,k,r,g,b;
	for(i=0;i<height;i++){
		for(j=0;j<width;j++){
			r=(int)img[i][j].R;
			g=(int)img[i][j].G;
			b=(int)img[i][j].B;
			k=(unsigned char)((r+b+g)/3);
			img[i][j].R=k;
			img[i][j].G=k;
			img[i][j].B=k;
		}
	}
	return img;
}

pixel ** filter(int **F, pixel **img,  int height, int width)
{
	int u,v,w,i,j,ra,rb,rc,rd,re,rf,rg,r,rh,b,ba,bb,bc,bd,be,bf,bg,bh,g,ga,gb,gc,gd,ge,gf,gg,gh;
	pixel **filtrat;
	filtrat=(pixel **)malloc(height*sizeof(pixel*));
	for(i=0;i<height;i++)
		filtrat[i]=(pixel *)malloc(width*sizeof(pixel));
	for (i=0;i<height;i++){
		for(j=0;j<width;j++){
			filtrat[i][j]=img[height-i-1][j];
		}
	}
	for (i=0;i<height;i++){
		for(j=0;j<width;j++){
			img[i][j]=filtrat[i][j];
		}
	}

	for(i=0;i<height;i++){
		for(j=0;j<width;j++){
			ra=0;rb=0;rc=0;rd=0;re=0;rf=0;rg=0;rh=0;ba=0;bb=0;bc=0;bd=0;be=0;bf=0;bg=0;bh=0;ga=0;gb=0;gc=0;gd=0;ge=0;gf=0;gg=0;gh=0;
			r=(int)img[i][j].R;
			g=(int)img[i][j].G;
			b=(int)img[i][j].B;
			if(i>0 && j>0){
			ra=(int)img[i-1][j-1].R;
			ga=(int)img[i-1][j-1].G;
			ba=(int)img[i-1][j-1].B;
			}
			if(i>0){
			rb=(int)img[i-1][j].R;
			gb=(int)img[i-1][j].G;
			bb=(int)img[i-1][j].B;
			}
			if(i>0 && j+1<width){
			rc=(int)img[i-1][j+1].R;
			gc=(int)img[i-1][j+1].G;
			bc=(int)img[i-1][j+1].B;
			}
			if(j>0){
			rd=(int)img[i][j-1].R;
			gd=(int)img[i][j-1].G;
			bd=(int)img[i][j-1].B;
			}
			if(j+1<width){
			re=(int)img[i][j+1].R;
			ge=(int)img[i][j+1].G;
			be=(int)img[i][j+1].B;
			}
			if (i+1<height && j>0){
			rf=(int)img[i+1][j-1].R;
			gf=(int)img[i+1][j-1].G;
			bf=(int)img[i+1][j-1].B;
			}
			if (i+1<height){
			rg=(int)img[i+1][j].R;
			gg=(int)img[i+1][j].G;
			bg=(int)img[i+1][j].B;
			}
			if (i+1<height && j+1<width){
			rh=(int)img[i+1][j+1].R;
			gh=(int)img[i+1][j+1].G;
			bh=(int)img[i+1][j+1].B;
			}
		
			u=(ra*F[0][0]+rb*F[0][1]+rc*F[0][2]+
				rd*F[1][0]+r*F[1][1]+re*F[1][2]+rf*F[2][0]+rg*F[2][1]+rh*F[2][2]);
			v=(ga*F[0][0]+gb*F[0][1]+gc*F[0][2]+
				gd*F[1][0]+g*F[1][1]+ge*F[1][2]+gf*F[2][0]+gg*F[2][1]+gh*F[2][2]);
			w=(ba*F[0][0]+bb*F[0][1]+bc*F[0][2]+
				bd*F[1][0]+b*F[1][1]+be*F[1][2]+bf*F[2][0]+bg*F[2][1]+bh*F[2][2]);
				
			if(u> 255)
				u=255;
			if(v > 255)
			  v=255;
			if(w > 255)
				w=255;
			if(u< 0)
				u=0;
			if(v< 0)
				v=0;
			if(w< 0)
				w=0;
			filtrat[i][j].R=(unsigned char)u;
			filtrat[i][j].G=(unsigned char)v;
			filtrat[i][j].B=(unsigned char)w;
		}
	}
	for (i=0;i<height;i++){
		for(j=0;j<width;j++){
			img[i][j]=filtrat[height-i-1][j];
		}
	}
	//mfree(filtrat, height);
	return img;
}




int main (void) {
	char *blackname, *input, *bmpname,*compressed;
	int pad,i,j,prag;
	int ** F1, **F2, **F3;

	bmpname=(char*) malloc(100 * sizeof(char));
	compressed=(char*) malloc(100 * sizeof(char));
	input=(char*) malloc(15 * sizeof(char));
	input= "input.txt";
	bmp_fileheader bmpfile;
	bmp_infoheader bmpinfo;
	FILE *inp=fopen(input, "rt");
	fscanf(inp,"%s %d %s", bmpname,&prag, compressed);
	fclose(inp);
	FILE *bmpinit=fopen(bmpname,"rt");
	fread(&bmpfile,sizeof(bmp_fileheader),1,bmpinit);
	fread(&bmpinfo,sizeof(bmp_infoheader),1,bmpinit);
	fclose(bmpinit);
	
	//pixel pixy;
	pixel **bitmap;
	bitmap=(pixel**)malloc(bmpinfo.height*sizeof(pixel*));
	for (i=0;i<bmpinfo.height;i++)
		bitmap[i]=(pixel*)malloc(bmpinfo.width*sizeof(pixel));
	FILE *bmpbin=fopen(bmpname,"rb");
	fseek(bmpbin,bmpfile.imageDataOffset,SEEK_SET);
	//fread(&pixy,sizeof(pixel),1,bmpbin);
	//printf ("%hhu %hhu %hhu", pixy.B,pixy.G,pixy.R);
	for(i=0;i<bmpinfo.height;i++){
		for(j=0;j<bmpinfo.width;j++){
			fread(&bitmap[i][j],sizeof(pixel),1,bmpbin);
		}
	}

	/*for(i=0;i<bmpinfo.height;i++){
		for(j=0;j<bmpinfo.width;j++){
			printf("(%hhu %hhu %hhu)",bitmap[i][j].B,bitmap[i][j].G,bitmap[i][j].R);
		}
	}*/
	pixel ** blackmap;
	pixel  padding;
	padding.R=0;
	padding.G=0;
	padding.B=0;

	blackmap=(pixel**)malloc(bmpinfo.height*sizeof(pixel*));
	for (i=0;i<bmpinfo.height;i++)
		blackmap[i]=(pixel*)malloc(bmpinfo.width*sizeof(pixel));
	blackmap=blackandwhite(bitmap,bmpinfo.height,bmpinfo.width);
	blackname=strtok(bmpname,".");
	char * filtername1,*filtername2, *filtername3;
	filtername1=(char*) malloc(100 * sizeof(char));
	filtername2=(char*) malloc(100 * sizeof(char));
	filtername3=(char*) malloc(100 * sizeof(char));
	char decompname[]="decompressed.bmp";
	memcpy(filtername1,blackname,strlen(blackname));
	memcpy(filtername2,blackname,strlen(blackname));
	memcpy(filtername3,blackname,strlen(blackname));

	strcat(blackname,"_black_white.bmp");
	//printf("%s",blackname);
	FILE *bmpblackwhite=fopen(blackname,"wb");
	fwrite(&bmpfile,sizeof(bmp_fileheader),1,bmpblackwhite);
	fwrite(&bmpinfo,sizeof(bmp_infoheader),1,bmpblackwhite);
	fseek(bmpblackwhite,bmpfile.imageDataOffset,SEEK_SET);
	for(i=0;i<bmpinfo.height;i++){
		pad=0;
		for(j=0;j<bmpinfo.width;j++){
			fwrite(&blackmap[i][j],sizeof(pixel),1,bmpblackwhite);
			pad++;
		}
		if(pad%4 != 0){
			fwrite(&padding,sizeof(pixel),pad%4,bmpblackwhite);
		}
	}

	pixel ** filtermap,**filtermap2,**filtermap3;
	filtermap=(pixel**)malloc(bmpinfo.height*sizeof(pixel*));
	for (i=0;i<bmpinfo.width;i++)
		filtermap[i]=(pixel*)malloc(bmpinfo.width*sizeof(pixel));
	filtermap2=(pixel**)malloc(bmpinfo.height*sizeof(pixel*));
	for (i=0;i<bmpinfo.width;i++)
		filtermap2[i]=(pixel*)malloc(bmpinfo.width*sizeof(pixel));
	filtermap3=(pixel**)malloc(bmpinfo.height*sizeof(pixel*));
	for (i=0;i<bmpinfo.width;i++)
		filtermap3[i]=(pixel*)malloc(bmpinfo.width*sizeof(pixel));
	for(i=0;i<bmpinfo.height;i++){
		for(j=0;j<bmpinfo.width;j++){
			filtermap[i][j]=blackmap[i][j];
			filtermap2[i][j]=blackmap[i][j];
			filtermap3[i][j]=blackmap[i][j];
		}
	}
	
	F1=calloc(3,sizeof(int*));
	for(i=0;i<3;i++)
		F1[i]=(int*)calloc(3,sizeof(int));
	F2=calloc(3,sizeof(int*));
	for(i=0;i<3;i++)
		F2[i]=(int*)calloc(3,sizeof(int));
	F3=calloc(3,sizeof(int*));
	for(i=0;i<3;i++)
		F3[i]=(int*)calloc(3,sizeof(int));
	for (i=0;i<3;i++){
		for(j=0;j<3;j++){
			F1[i][j]=-1;
		}
	}
	
	F1[1][1]=8;
	F2[0][1]=1;
	F2[1][0]=1;
	F2[1][2]=1;
	F2[2][1]=1;
	F2[1][1]=-4;
	F3[0][0]=1;
	F3[0][2]=-1;
	F3[2][0]=-1;
	F3[2][2]=1;

	filtermap=filter(F1,filtermap,bmpinfo.height,bmpinfo.width);
	strcat(filtername1,"_f1.bmp");
	strcat(filtername2,"_f2.bmp");
	strcat(filtername3,"_f3.bmp");
	FILE *filter1=fopen(filtername1,"wb");
	fwrite(&bmpfile,sizeof(bmp_fileheader),1,filter1);
	fwrite(&bmpinfo,sizeof(bmp_infoheader),1,filter1);
	fseek(filter1,bmpfile.imageDataOffset,SEEK_SET);
	for(i=0;i<bmpinfo.height;i++){
		pad=0;
		for(j=0;j<bmpinfo.width;j++){
			fwrite(&filtermap[i][j],sizeof(pixel),1,filter1);
			pad++;
		}
		if(pad%4 != 0){
			fwrite(&padding,sizeof(pixel),pad%4,filter1);
		}
	}
//	mfree(filtermap,bmpinfo.height);
	filtermap2=filter(F2,filtermap2,bmpinfo.height,bmpinfo.width);
	FILE *filter2=fopen(filtername2,"wb");
	fwrite(&bmpfile,sizeof(bmp_fileheader),1,filter2);
	fwrite(&bmpinfo,sizeof(bmp_infoheader),1,filter2);
	fseek(filter2,bmpfile.imageDataOffset,SEEK_SET);
	for(i=0;i<bmpinfo.height;i++){
		pad=0;
		for(j=0;j<bmpinfo.width;j++){
			fwrite(&filtermap2[i][j],sizeof(pixel),1,filter2);
			pad++;
		}
		if(pad%4 != 0){
			fwrite(&padding,sizeof(pixel),pad%4,filter2);
		}
	}
		//mfree(filtermap2,bmpinfo.height);
	filtermap3=filter(F3,filtermap3,bmpinfo.height,bmpinfo.width);
	FILE *filter3=fopen(filtername3,"wb");
	fwrite(&bmpfile,sizeof(bmp_fileheader),1,filter3);
	fwrite(&bmpinfo,sizeof(bmp_infoheader),1,filter3);
	fseek(filter3,bmpfile.imageDataOffset,SEEK_SET);
	for(i=0;i<bmpinfo.height;i++){
		pad=0;
		for(j=0;j<bmpinfo.width;j++){
			fwrite(&filtermap3[i][j],sizeof(pixel),1,filter3);
			pad++;
		}
		if(pad%4 != 0){
			fwrite(&padding,sizeof(pixel),pad%4,filter3);
		}
	}
		//mfree(filtermap,bmpinfo.height);
	/*for(i=0;i<3;i++){
		for(j=0;j<3;j++){
			printf("(%d)",F3[i][j]);

		}puts(" ");
	}*/

		FILE *cmpimage=fopen(compressed,"rb");
		pixel ** decompressed;
		decompressed=(pixel**)malloc(bmpinfo.height*sizeof(pixel*));
	
		for (i=0;i<bmpinfo.height;i++)
			decompressed[i]=(pixel*)malloc(bmpinfo.width*sizeof(pixel));
		fseek(cmpimage,bmpfile.imageDataOffset,SEEK_SET);
		compr coord;
		for(i=0;i<bmpinfo.height;i++){
		for(j=0;j<bmpinfo.width;j++){
			decompressed[i][j].R=0;
			decompressed[i][j].R=0;
			decompressed[i][j].R=0;
		}
	}
		while( feof(cmpimage) == 0 )
	{
		fread(&coord.ox, sizeof(short), 1, cmpimage );
		fread(&coord.oy, sizeof(short), 1, cmpimage );
		fread(&decompressed[bmpinfo.height-coord.ox][coord.oy-1].R,sizeof(unsigned char), 1, cmpimage);
		fread(&decompressed[bmpinfo.height-coord.ox][coord.oy-1].G,sizeof(unsigned char), 1, cmpimage);
		fread(&decompressed[bmpinfo.height-coord.ox][coord.oy-1].B,sizeof(unsigned char), 1, cmpimage);

	}
		
		FILE *out = fopen( decompname, "wb");
		fwrite(&bmpfile,sizeof(bmp_fileheader),1,out);
		fwrite(&bmpinfo,sizeof(bmp_infoheader),1,out);
		fseek(out,bmpfile.imageDataOffset,SEEK_SET);
		int P=0;
		while(P!=bmpinfo.height){
			for(i=0;i<bmpinfo.width-1;i=i+1){
					if(decompressed[P][i + 1].R == 0 && decompressed[P][i + 1].G == 0 && decompressed[P][i + 1].B == 0){
						decompressed[P][i+1].R = decompressed[P][i].R;
						decompressed[P][i+1].G = decompressed[P][i].G;
						decompressed[P][i+1].B = decompressed[P][i].B;
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
				if(pad%4!=0){
				fwrite(&padding,sizeof(pixel),pad%4,out);
				}
			}








	return 0;

}
