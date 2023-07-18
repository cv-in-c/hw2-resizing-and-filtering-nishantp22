#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"
#define TWOPI 6.2831853

void l1_normalize(image im){
    float s=0;
    for (int i=0;i<im.w*im.h*im.c;i++) s=s+im.data[i];
    if (s==0) return;
    else for (int i=0;i<im.w*im.c*im.h;i++){
      im.data[i]=im.data[i]/s;
    }
}

image make_box_filter(int w){
    image im=make_image(w, w, 1);
    for (int i=0;i<w*w;i++){
      im.data[i]=((float)1)/(w*w);
    }
    return im;
}


image convolve_image(image im, image filter, int preserve){
    assert(filter.c== 1||filter.c==im.c);
    float a;
    if(preserve==1) a=im.c;
    else a=1;
    image img=make_image(im.w,im.h,a);
    int deltaH=filter.h/2;
    int deltaW=filter.w/2;
    for (int i=0;i<im.c;i++){
      for (int j=-deltaH;j<im.h-deltaH;j++){
        for (int k=-deltaW;k<im.w-deltaW;k++){
          float f;
          if(preserve==1){
            f=0;
          }
          else f=get_pixel(img, k+deltaW, j+deltaH, 0);
          for (int l=0;l<filter.h;l++){
            for (int m=0;m<filter.w;m++){
              float q;
              if(filter.c==1){
                q=0;
              }
              else q=i;
              f=f+get_pixel(im,k+m,j+l,i)*\
            get_pixel(filter,m,l,q);
            }
          }
          float r;
          if(preserve==1) r=i;
          else r=0;
          set_pixel(img,k+deltaW,j+deltaH,r,f);
        }
      }
    }
    return img;
}

image make_highpass_filter(){
    image im = make_image(3, 3, 1);
    float matrix[] = {0,-1,0,-1,4,-1,0,-1,0};
    memcpy(im.data, matrix, sizeof(matrix));
    return im;
}

image make_sharpen_filter(){
    image im = make_image(3, 3, 1);
    float matrix[] = {0,-1,0,-1,5,-1,0,-1,0};
    memcpy(im.data, matrix, sizeof(matrix));
    return im;
}

image make_emboss_filter(){
    image im=make_image(3, 3, 1);
    float arr[] = {-2,-1,0,-1,1,1,0,1,2};
    memcpy(im.data,arr,sizeof(arr));
    return im;
}

image make_gaussian_filter(float sigma){
    int w=6*sigma;
    if(w>0) if(w%2!=1)w++;
    else w=1;
    image im=make_image(w,w,1);
    int delta= w/2;
    for (int i=-delta;i<w-delta;i++){
      for (int j=-delta;j<w-delta;j++){
        float f=1/(TWOPI*sigma*sigma)*exp((-i*i-j*j)/(2*sigma*sigma));
        set_pixel(im,j+delta,i+delta,0,f);
      }
    }
    return im;
}

image add_image(image a, image b){
    assert(a.h==b.h&&a.w==b.w&&a.c==b.c);
    image im=make_image(a.w,a.h,a.c);
    for (int i=0;i<a.c*a.h*a.w;i++){
      im.data[i]=a.data[i]+b.data[i];
    }
    return im;
}

image sub_image(image a, image b){
    assert(a.w==b.w&&a.h==b.h&&a.c==b.c);
    image im=make_image(a.w,a.h,a.c);
    for(int i=0;i<a.w*a.h*a.c;i++){
      im.data[i]=a.data[i]-b.data[i];
    }
    return im;
}

image make_gx_filter(){
    image im=make_image(3, 3, 1);
    float arr[] = {-1,0,1,-2,0,2,-1,0,1};
    memcpy(im.data,arr, sizeof(arr));
    return im;
}

image make_gy_filter(){
    image im=make_image(3, 3, 1);
    float arr[]={-1,-2,-1,0,0,0,1,2,1};
    memcpy(im.data,arr, sizeof(arr));
    return im;
}

void feature_normalize(image im){
    float max=0;
    float min=1;
    for (int i=0;i<im.w*im.h*im.c;i++){
      if(im.data[i]>max) max=im.data[i];
      if(im.data[i]<min) min=im.data[i];
    }
    float delta=max-min;
    if (delta!=0) for(int i=0;i<im.w*im.h*im.c;i++) im.data[i]=0; 
    else for(int i=0;i<im.w*im.h*im.c;i++) im.data[i]=(im.data[i]-min)/delta;
}

image *sobel_image(image im){
    int i = 0;
    image x=make_gx_filter();
    image y=make_gy_filter();
    image *img=malloc(2*sizeof(image));
    image convx=convolve_image(im,x,0);
    image convy=convolve_image(im,y,0);
    image en=make_image(convx.w,convx.h,1);
    image new=en;
    for (int i=0;i<im.w*im.h;i++){
      en.data[i] = sqrt(convx.data[i]*convx.data[i] + convy.data[i]*convy.data[i]);
      new.data[i] = atan2(convy.data[i], x.data[i]);
    }
    free_image(x);
    free_image(y);
    free_image(convx);
    free_image(convy);
    img[0]=en;
    img[1]=new;
    return img;
}

image colorize_sobel(image im){
    image new=make_image(im.w, im.h,3);
    image *img=sobel_image(im);
    feature_normalize(img[0]);
    feature_normalize(img[1]);
    memcpy(new.data,img[1].data,im.w*im.h*sizeof(float));
    memcpy(new.data+im.w*im.h,img[0].data,im.w*im.h*sizeof(float));
    memcpy(new.data+2*im.h*im.w,img[0].data,im.w*im.h*sizeof(float));
    free_image(img[0]);                  //
    free_image(img[1]);
    hsv_to_rgb(new);
    return new;
}