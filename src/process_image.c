#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"

float get_pixel(image im, int x, int y, int c)
{

    x=x<0?0:(x>=im.w?im.w-1:x);
    y=y<0?0:(y>=im.h?im.h-1:y);
    c=c<0?0:(c>=im.c?im.c-1:c);
    return *(im.data+x+y*im.w+c*im.w*im.h);
}

void set_pixel(image im, int x, int y, int c, float v)
{
    if (x>=0 && x<im.w&&y>=0&& y<im.h&&c>=0&&c<im.c) *(im.data+x+y*im.w+c*im.w*im.h)= v;
    else return;
}

image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);
    memcpy(copy.data,im.data,sizeof(float)*(im.c*im.h*im.w));
    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 1);
      for (int i=0;i<im.w*im.h;i++){
        *(gray.data+i) = (*(im.data+i)*0.299)+(*(im.data+im.w*im.h+i)*0.587)+(*(im.data+2*im.w*im.h+i)*0.114);
    }
    return gray;
}

void shift_image(image im, int c, float v)
{
    for (int i=0; i<im.w*im.h;i++){
      *(im.data+i+c*im.h*im.w)+= v;
    }
}

void clamp_image(image im)
{
    for (int i=0;i<im.w*im.h*im.c;i++){
      if(*(im.data+i)>1) *(im.data+i)=1;
      else if(*(im.data+i)<0) *(im.data+i)=0;
    }
}


// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

void rgb_to_hsv(image im)
{
    for (int i=0;i<im.w*im.h;i++){
      float r=*(im.data+i);
      float g=*(im.data+i+im.w*im.h);
      float b=*(im.data+i+2*im.w*im.h);
      float v=three_way_max(r,g,b);
      float m=three_way_min(r, g, b);
      float c= v-m;
      float s=(v==0?0:c/v);
      float h1=0;
      if (v==m) h1=0;
      else if (v==r) h1=(g-b)/c;
      else if (v==g) h1=(b-r)/c+2;
      else if(v==b) h1=(r-g)/c+4;
      float h;
      if (h1<0) h=h1/6+1;
      else h=h1/= 6;
      *(im.data+i) = h;
      *(im.data+i+im.w*im.h)=s;
      *(im.data+i+2*im.w*im.h)=v;
    }
}
void hsv_to_rgb(image im)
{
    for (int i=0;i<im.w*im.h;i++) {
        float r=0,g=0,b=0;
        float h=*(im.data+i);
        float s=*(im.data+i+im.w*im.h);
        float v=*(im.data+i+2*im.w*im.h);
        float c=v*s;
        if(s!=0){
            h=h*6.0;
            float f=h-(int)h;
            float m=v-c;
            float m1=v-f*c;
            float m2=v-(1-f)*c;
            if((int)h==0){
                    r=v;
                    g=m2;
                    b=m;
            }
            else if((int)h==1){
                r=m1;
                g=v;
                b=m;
            }
            else if((int)h==2){
                r=m;
                g=v;
                b=m2;
            }
            else if((int)h==3){
                r=m;
                g=m1;
                b=v;
            }
            else if((int)h==4){
                r=m2;
                g=m;
                b=v;
            }
            else if((int)h==5){
                r=v;
                g=m;
                b=m1;
            }
        }
        else{
          r=v;
          g=v;
          b=v;
        }
        *(im.data+i)=r;
        *(im.data+i+im.w*im.h)=g;
        *(im.data+i+2*im.w*im.h)=b;
    }
}
void scale_image(image im, int c, float v)
{
    for (int i=0; i<im.w*im.h;i++){
      *(im.data+i+c*im.h*im.w)*=v;
    }
}

