#ifndef IMAGE_H
#define IMAGE_H
#include<assert.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
//LETS BE INCLUDED BY DEFAULT
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

//MACRO USED FOR GETING PIXEL IN A MATRIX LIKE STRUCTURE
#define PIXEL_AT(image,y,x) (image).pixels[(y)*(image).width + (x)]

//IMAGE PARAMETAR MACRO USED FOR LOADING IMAGE PATAMETARS IF IMAGE IS ALRREDY ALOCATED FROM STB IMAGE
#define IMAGE_PARAMETARS(i) (i).width/(i).chanels,(i).height,(i).chanels



#define IMAGE_ASSERT assert

#define IMAGE_CALLOC calloc

#include<complex.h>

typedef struct {

	int width;       //WIDTH  OF IMAGE IN PIXELS
	int height;      //HEIGHT OF IMAGE IN PIXELS*NUMBER OF CHANELS
	int chanels;     //HOW    MUTCH PIXEL IS USED TO REPRESENT THE IMAGE
	uint8_t *pixels; //ROW PIXELS

	} Image;




Image Image_Alloc(int width,int height,int chanels);  //ALLOCATE IMAGE FULL OF ZEROS
Image Image_Alloc_Name(const char* name);   //DYNAMICLY ALLOCATE IMAGE ALLOCATION IS IN STB_IMAGE
Image Image_Alloc_Name_Fft(const char* name,int xd,int yd);
void Image_Free(Image i);

void Image_Set(Image i, uint8_t number);

void Image_Save(Image i, const char* name); //WRITE IMAGE AS JPG
void Image_Print(Image i);                  //PRINT THE IMAGE WIDTH HEIGHT, CHANELS TO CONSOL NOT USE

void Image_Sum(Image a, Image b);           //SUM TWO IMAGES IN IMAGE ASSERT A MUST BE BIGER OR EQUAL
void Image_Dec(Image a, Image b);           //SUB TWO IMAGES IN IMAGE ASSERT A MUST BE BIGER OR EQUAL
void Image_Mul(Image a, Image b);           //MUL TWO IMAGES IN IMAGE ASSERT A MUST BE BIGER OR EQUAL
void Image_Div(Image a, Image b);           //DIV TWO IMAGES IN IMAGE ASSERT A MUST BE BIGER OR EQUAL
void Image_Sum_Shift(Image a, Image b,size_t startX,size_t startY); //SUM OF 2 IMAGES FROM START  IN A
void Image_Dec_Shift(Image a, Image b,size_t startX,size_t startY); //SUB OF 2 IMAGES FROM START  IN A
void Image_Mul_Shift(Image a, Image b,size_t startX,size_t startY); //MUL OF 2 IMAGES FROM START  IN A
void Image_Div_Shift(Image a, Image b,size_t startX,size_t startY); //DIV OF 2 IMAGES FROM START  IN A

void Image_Copy(Image dest, Image source);  //COPY IMAGE FROM SOURCE TO DEST
void Image_Copy_Shift(Image dest, Image source,size_t startX,size_t startY);  //COPY IMAGE FROM SOURCE TO DEST STARTING FROM STARTX STARTY

void Image_Applay_Kernel(Image a,Image i,float *kernel,int width,int height);//APPLAY CONVOLUTION TO IMAGE
float KERNEL_IDENTITY[] = {0,0,0,0,1,0,0,0,0};
float KERNEL_SHARPEN[] = {0,-1,0,-1,5,-1,-0,-1,-0,};
float KERNEL_BLURE[] = {0.11,0.11,0.11,0.11,0.11,0.11,0.11,0.11,0.11,};
float KERNEL_EDGE[] = {0,-1,0,-1,4,-1,-0,-1,-0,};
float KERNEL_RIDGE[] = {-1,-1,-1,-1,8,-1,-1,-1,-1,};
float KERNEL_BL[]={0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,};
float KERNEL_BOXBLURE[]={1,1,1,1,1,1,1,1,1,};
void Image_Black_White_Filtar(Image i, int Pixel_Treshold);  //MAKE IMAGE BLACK AND WHITE
void Image_Zero_Chanel(Image i, int chanel);                 //REMOVE R,G,B,A CHANEL(COLOR)
void Image_Invert(Image i);         //CHANGE IMAGE PIXELS INTO (INVERT) 255 - img.pixels[i]

void Image_Center_Of_Mass(Image i,size_t *yx);  //FUNCTION USED TO CALCULATE CENTER OF MASS IN IMAGE THIS IS UDED TO DESIDE IS SOMTETHING IN IMAGE, SUBIMAGE

void Image_Draw_Rect(Image i, size_t startX,size_t startY,size_t h,size_t w,uint8_t pixel);


size_t Load_Binary_To_Image(Image i,char *name);
void Image_To_Binary(Image i,size_t bin_size,char *name);


//FFT
void fft(uint8_t *in,size_t pivot,int complex *out,size_t n); 


#endif

#ifdef IMAGE_IMPLEMENTATION

Image Image_Alloc(int width,int height,int chanels) { //ALLOCATE IMAGE FULL OF ZEROS
	Image i;
	i.chanels = chanels;
	i.width   = chanels * width; //MULTIPLAY WITH NUMBER OF CHANELS
	i.height  = height;
	i.pixels = (uint8_t*) IMAGE_CALLOC(chanels * width * height,sizeof(*i.pixels));
	return i;
	}

Image Image_Alloc_Name(const char* name) {  //DYNAMICLY ALLOCATE IMAGE ALLOCATION IS IN STB_IMAGE
	Image i;
	i.pixels = stbi_load(name,&i.width,&i.height,&i.chanels,0);
	i.width  = i.chanels * i.width; //MULTIPLAY WITH NUMBER OF CHANELS
	return i;
	}

void Image_Save(Image i, const char* name) { //WRITE IMAGE AS JPG

	stbi_write_jpg(name,(int)(i.width / i.chanels),i.height,i.chanels,i.pixels, 100);
	}


void Image_Print(Image i) { //PRINT THE IMAGE WIDTH HEIGHT, CHANELS TO CONSOL NOT USE

	printf("__________________________________________________________________\n");

	printf("width %d height: %d chanels :%d\n",i.height,(int)(i.width/i.chanels),i.chanels);

	printf("\n__________________________________________________________________\n");
	}
void Image_Sum(Image a, Image b) {   //SUM TOW IMAGES IN IMAGE ASSERT A MUST BE BIGER OR EQUAL

	IMAGE_ASSERT(a.width  >=  b.width);
	IMAGE_ASSERT(a.height >=  b.height);


	for(size_t y = 0; (y < a.height) && (y < b.height); y++) {
		for(size_t x = 0; (x < a.width) && (x < b.width); x++) {

			PIXEL_AT(a,y,x) += PIXEL_AT(b,y,x);
			}
		}
	}

void Image_Dec(Image a, Image b) {   //SUM TOW IMAGES IN IMAGE ASSERT A MUST BE BIGER OR EQUAL

	IMAGE_ASSERT(a.width  >=  b.width);
	IMAGE_ASSERT(a.height >= b.height);


	for(size_t y = 0; (y < a.height) && (y < b.height); y++) {
		for(size_t x = 0; (x < a.width) && (x < b.width); x++) {

			PIXEL_AT(a,y,x) -= PIXEL_AT(b,y,x);
			}
		}
	}

void Image_Mul(Image a, Image b) {   //SUM TOW IMAGES IN IMAGE ASSERT A MUST BE BIGER OR EQUAL

	IMAGE_ASSERT(a.width  >=  b.width);
	IMAGE_ASSERT(a.height >= b.height);


	for(size_t y = 0; (y < a.height) && (y < b.height); y++) {
		for(size_t x = 0; (x < a.width) && (x < b.width); x++) {
			PIXEL_AT(a,y,x) *= PIXEL_AT(b,y,x);
			}
		}
	}

void Image_Div(Image a, Image b) {   //SUM TOW IMAGES IN IMAGE ASSERT A MUST BE BIGER OR EQUAL

	IMAGE_ASSERT(a.width  >=  b.width);
	IMAGE_ASSERT(a.height >= b.height);


	for(size_t y = 0; (y < a.height) && (y < b.height); y++) {

		for(size_t x = 0; (x < a.width) && (x < b.width); x++) {
			//IMAGE_ASSERT(PIXEL_AT(b,y,x) != 0);
			if(PIXEL_AT(b,y,x)!=0)
			   PIXEL_AT(a,y,x) /= PIXEL_AT(b,y,x);
			}
		}
	}

void Image_Copy(Image dest, Image source) {
	IMAGE_ASSERT(dest.width  >= source.width);
	IMAGE_ASSERT(dest.height >= source.height);
	for(size_t y = 0; (y < dest.height) && (y < source.height); y++) {
		for(size_t x = 0; (x < dest.width) && (x < source.width); x++) {
			PIXEL_AT(dest,y,x) = PIXEL_AT(source,y,x);
			}
		}
	}
void Image_Set(Image i, uint8_t number) {

	for(size_t y = 0; y < i.height; y++) {
		for(size_t x = 0; x < i.width; x++) {
			PIXEL_AT(i,y,x) = number;
			}
		}
	}

void Image_Applay_Kernel(Image a,Image i, float  *kernel, int width,int height) {
	//IMAGE_ASSERT(1);
	//IMAGE_ASSERT(1);
	//Image a = Image_Alloc(IMAGE_PARAMETARS(i));
	for(size_t y = 0; y < i.height - height ; y++) {
		for(size_t z = 0;z < i.chanels;z++)
		for(size_t x = 0; x < i.width - width; x+=i.chanels) {
			float sum = 0.0f;
			//printf("sum %d\n",sum);
			for(size_t j = 0; j < height; j++) {
				for(size_t k = 0; k < width; k++) {

					sum+=(float)(kernel[j*height + k]*(float)PIXEL_AT(i,y+j,x+(k)*z));

					//printf("sum %d\n",sum);
					}
				}
			//sum = (uint8_t)sum;
			if(sum  > 255.0f){
				sum = 255.0f;
			}
				
			if(sum < 0.0f){
				sum = 0.0f;
			}
				
			//printf("sum %u\n",(uint8_t)sum);
			PIXEL_AT(a,y,x + z) = (uint8_t)sum;
			}
		}
	//return a;

	}
void Image_Black_White_Filtar(Image img, int Pixel_Treshold) {
	for(size_t i = 0; i < img.width*img.height-3; i+=img.chanels) {
		float sum = img.pixels[i]+img.pixels[i+1]+img.pixels[i+2];
		sum/=img.chanels;
		if(sum > Pixel_Treshold) {
			for(size_t j = 0; j < img.chanels; j++)
				img.pixels[i + j]   = 255;
			}
		else {
			for(size_t j = 0; j < img.chanels; j++)
				img.pixels[i+j]   = 0;
			}
		}
	}

void Image_Zero_Chanel(Image i, int chanel) {
	for(size_t y = 0; y < i.height*i.width-3; y+=i.chanels) {
		i.pixels[y + chanel] = 0;

		}
	}
void Image_Invert(Image i) {
	for(size_t y = 0; y < i.height*i.width-3; y++)
		i.pixels[y] = 255 - i.pixels[y];
	}
void Image_Center_Of_Mass(Image i,size_t *yx) {
	Image_Black_White_Filtar(i,60);  // That Image is Allredy Black and White
	size_t xc = 0,yc = 0,nyc=0;
	for(size_t y = 0; y < i.height - 1; y++) {
		for(size_t x = 0,counter = 0; x < i.width; x+=i.chanels,counter++) {
			//printf("x %d y %d\n",x,y);
			if(PIXEL_AT(i,y,x) >= 10) {
				xc+=counter;
				yc+=y;
				nyc++;
				}
			}

		}
	if(nyc!=0) {
		yx[0] += yc / nyc;
		yx[1] += xc / nyc;
		//printf("y %d x %d",yc ,xc);
		}
	//printf("y %d x %d",yc ,xc);
	//system("pause");
	}

void Image_Mul_Shift(Image a, Image b,size_t startX,size_t startY) {
	IMAGE_ASSERT((startY + a.height) < b.height);
	IMAGE_ASSERT((startX*a.chanels + a.width) < b.width);
	for(size_t y = 0; y <a.height; y++) {
		for(size_t x = 0; x <a.width; x++) {
			//printf("x %d y %d\n",x,y);
			PIXEL_AT(a,y,x) *= PIXEL_AT(b,y+startY,x+(startX*a.chanels));
			//printf("Pixel %d\n",PIXEL_AT(a,(y-startY),(x - startX)));
			}
		}
	}
void Image_Sum_Shift(Image a, Image b,size_t startX,size_t startY) {
	IMAGE_ASSERT((startY + a.height) < b.height);
	IMAGE_ASSERT((startX*a.chanels + a.width) < b.width);
	for(size_t y = 0; y <a.height; y++) {
		for(size_t x = 0; x <a.width; x++) {
			//printf("x %d y %d\n",x,y);
			PIXEL_AT(a,y,x) += PIXEL_AT(b,y+startY,x+(startX*a.chanels));
			//printf("Pixel %d\n",PIXEL_AT(a,(y-startY),(x - startX)));
			}
		}


	}

void Image_Dec_Shift(Image a, Image b,size_t startX,size_t startY) {
	IMAGE_ASSERT((startY + a.height) < b.height);
	IMAGE_ASSERT((startX*a.chanels + a.width) < b.width);
	for(size_t y = 0; y <a.height; y++) {
		for(size_t x = 0; x <a.width; x++) {
			//printf("x %d y %d\n",x,y);
			PIXEL_AT(a,y,x) -= PIXEL_AT(b,y+startY,x+(startX*a.chanels));
			//printf("Pixel %d\n",PIXEL_AT(a,(y-startY),(x - startX)));
			}
		}


	}

void Image_Div_Shift(Image a, Image b,size_t startX,size_t startY) {
	IMAGE_ASSERT((startY + a.height) < b.height);
	IMAGE_ASSERT((startX*a.chanels + a.width) < b.width);
	for(size_t y = 0; y <a.height; y++) {
		for(size_t x = 0; x <a.width; x++) {
			IMAGE_ASSERT(PIXEL_AT(b,y+startY,x+(startX*a.chanels))!=0);
			PIXEL_AT(a,y,x) /= PIXEL_AT(b,y+startY,x+(startX*a.chanels));
			//printf("Pixel %d\n",PIXEL_AT(a,(y-startY),(x - startX)));
			}
		}


	}

void Image_Copy_Shift(Image dest, Image source,size_t startX,size_t startY) {
	IMAGE_ASSERT(dest.width  >= source.width);
	IMAGE_ASSERT(dest.height >= source.height);
	IMAGE_ASSERT((startY + dest.height) < source.height);
	IMAGE_ASSERT((startY*dest.chanels + dest.width) < source.width);
	for(size_t y = 0; y < dest.height; y++) {
		for(size_t x = 0; x < dest.width; x++) {
			PIXEL_AT(dest,y,x) = PIXEL_AT(source,y+startY,x+(startX*dest.chanels));
			}
		}
	}

void Image_Draw_Rect(Image i, size_t startX,size_t startY,size_t h,size_t w,uint8_t pixel) {
	IMAGE_ASSERT((startY + h) <= i.height);
	IMAGE_ASSERT((startX + w) <= i.width);
	for(size_t y = startY; y < startY + h; y++) {
		for(size_t x = (startX*i.chanels); x < (startX + w)*i.chanels; x++) {
			PIXEL_AT(i,y,x) = pixel;
			}
		}
	}
	
	
void Image_Free(Image i){
	free(i.pixels);
}

size_t Load_Binary_To_Image(Image i,char *name){
	FILE *f = fopen(name,"rb");
	if(f==NULL)
	 IMAGE_ASSERT(0 && " FILE IS NOT LOADED !!! ");
	 for(size_t y = 0; y < i.height;y++){
	 	for(size_t x = 0; x < i.width;x++){
		 if(feof(f) != 0){
		 	fclose(f);
		 	return x+y*i.width-1;
		 }
		 else{
		 	    fread(&PIXEL_AT(i,y,x),sizeof(uint8_t),1,f);
					//printf("%d\n",PIXEL_AT(i,y,x)); 
		 }
		 }
	 }
 
}
void Image_To_Binary(Image i,size_t bin_size,char *name){
	FILE *f = fopen(name,"wb+");
	//for(size_t y = 0;y < bin_size;y++){
		fwrite(i.pixels,sizeof(uint8_t),bin_size,f);
		printf("Nesto");
	//}
	fclose(f);
	
}

void fft(uint8_t *in,size_t pivot,int complex *out,size_t n)  //coll
{
    float pi=3.141569;
    assert(n > 0);
    //printf("nesto");
    if(n==1)
    {
        out[0]=in[0];
        return; //braking assert error
    }
    fft(in,pivot*2,out,n/2);//even
    fft(in+pivot,pivot*2,n/2+out,n/2);//odd  
    for (size_t k = 0; k < n/2; k++)
    {
        float t=(float)k/n;
        float complex e = out[k];
        float complex v = cexp(-2*pi*I*t)*out[k+n/2];
        out[k]=e+v;
        out[k+n/2]=e-v; 
    }
   
    
    
}


Image Image_Alloc_Name_Fft(const char* name,int xd,int yd)
{
	Image ia = Image_Alloc_Name(name);
	IMAGE_ASSERT(xd <= 500 && yd <= 500);
	int complex mag[xd*yd];
	fft(ia.pixels,0,mag,xd*yd);
	//system("pause");
  int max_r = 0,max_i = 0; 
	for(size_t i = 0;i < xd*yd;i++){
		if(creal(mag[i]) > max_r){
			max_r = (int)creal(mag[i]);
		}
		if(cimag(mag[i]) > max_i){
			max_i = (int)cimag(mag[i]);
		}
	}
	
	
	Image b = Image_Alloc(xd,yd,1);
	size_t z = 0;
	for(size_t y = 0;y < yd;y++){
		for(size_t x = 0;x < xd;x++){
			float real = (creal(mag[z])); // max_r;
			float imag = (cimag(mag[z])); // max_i;
			uint8_t magnitude = (uint8_t)(sqrt(real*real + imag*imag)); //* 255;
			PIXEL_AT(b,y,x) = magnitude; 
			z++;
	}}
  free(ia.pixels);
  return b;


}



#endif




#ifdef DYNAMIC

#ifndef ARRAY_TYPE
#define ARRAY_TYPE int 
#endif 

typedef struct d{
	
	ARRAY_TYPE *array;
	size_t index_of_last;  //INDEX OF LAST ELEMET
	size_t _size;          //DEFINE THIS SIZE(IN INIT OF ARRAY) 

}D_ARRAY;



void init_Array(D_ARRAY *array,size_t initialSize){
	
	array->array = calloc(initialSize,sizeof(ARRAY_TYPE));
	array->_size = initialSize;
	array->index_of_last = 0;
	
	
}
void insert_Array(D_ARRAY *array,ARRAY_TYPE elem){
	
	if(array->index_of_last == array->_size){
		array->_size *= 2;
		array->array = realloc(array->array,sizeof(ARRAY_TYPE)*array->_size);
		if(array->array == NULL)
			assert(0 && "REALOC FAIL!!! ");
			printf("Realloc!!!\n");
		
	}
	array->array[array->index_of_last++] = elem;
	
}

#endif

