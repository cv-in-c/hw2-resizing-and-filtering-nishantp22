#include <math.h>
#include "image.h"

float nn_interpolate(image im, float x, float y, int c)
{
    // TODO Fill in
    return get_pixel(im, round(x), round(y), c);    
}

image nn_resize(image im, int w, int h)
{
    // TODO Fill in (also fix that first line)
    image im=make_image(w, h, im.c);
    float hRes=(float)im.h/h;
    float wRes=(float)im.w/w;
    for (int i=0;i<im.c;i++){
      for (int j=0; j<h;j++){
        for (int k=0; k<w;k++){
          float hNew=hRes*(j+(1/2))-(1/2);
          float wNew=wRes*(k+(1/2))-(1/2);
          float f=nn_interpolate(im,wNew,hNew,i);
          set_pixel(im,k,j,i,f);
        }
      }
    }
    return im;
}

float bilinear_interpolate(image im, float x, float y, int c)
{
    // TODO
    float p=get_pixel(im,floor(x),floor(y),c);
    float q=get_pixel(im,ceil(x),floor(y),c);
    float r=get_pixel(im,floor(x),ceil(y),c);
    float s=get_pixel(im,ceil(x),ceil(y),c);
    float t=q*(x-floor(x))+p*(ceil(x)-x);
    if(floor(x)==ceil(x)) t=p;
    float u=s*(x-floor(x))+r*(ceil(x)-x);
    if(floor(x)==ceil(x)) u=r;
    float q=u*(y-floor(y))+t*(ceil(y)-y);
    if(floor(y)==ceil(y)) q=t;
    return q;
}
image bilinear_resize(image im, int w, int h)
{
    // TODO
    image img=make_image(w, h, im.c);
    float hBil=(float)im.h/h;
    float wBil=(float)im.w/w;
    for (int i=0;i<im.c;i++){
      for (int j=0;j<h;j++){
        for (int k=0; k<w;k++){
          float hNew=hBil*(j+(1/2))-(1/2);
          float wNew=wBil*(k+(1/2))-(1/2);
          float v=bilinear_interpolate(im,wNew,hNew,i);
          set_pixel(img,k,j,i,v);
        }
      }
    }
    return img; //
}