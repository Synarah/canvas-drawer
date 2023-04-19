// Copyright 2021, Aline Normoyle, alinen

/**
 *Creating functions that alter an image given a certian image. From a header file a constructor, 
 copy constructor, operator and destructor are created in order to get instances of images in
 order to alter them. After reading and altering images they are then rewritten into a new file 
 so that the altered image is saved. 
 * @author ssitaf
 * Feb 9, 2023
 */

#include "image.h"
#include "string.h"

#include <cassert>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

namespace agl {


Image::Image() {
   myw = 0;
   myh = 0;
   numchannels = 3;
   mydata = NULL;
}

Image::Image(int width, int height)  {
   myw = width;
   myh = height;
   numchannels = 3;

   mydata = new Pixel[width * height];  
}

Image::Image(const Image& orig) {
   myw = orig.myw;
   myh = orig.myh;
   numchannels = orig.numchannels;
   
   mydata = new Pixel[myw * myh];
   memcpy(mydata, orig.mydata, sizeof(Pixel)* myw * myh);
  
}

Image& Image::operator=(const Image& orig) {
  if (&orig == this) {
    return *this;
  }
  if(mydata != NULL){
     delete[] mydata;
     mydata = NULL;
  }
  myw = orig.myw;
  myh = orig.myh;
  mydata = new Pixel[myw * myh];
  memcpy(mydata, orig.mydata, sizeof(Pixel)* myw * myh);
  //myw = nw;
  //myh = nh;
  //mydata = ndata;
  return *this;
}

Image::~Image() {
   delete[] mydata;
}

int Image::width() const { 
   return myw;
}

int Image::height() const {
   return myh;
}

char* Image::data() const {
   return (char*)mydata;
}

void Image::set(int width, int height, unsigned char* data) {
   myw = width;
   myh = height;
   if(mydata != NULL){
     delete[] mydata;
     mydata = NULL;
  }
  mydata = new Pixel[myw * myh];
  memcpy(mydata, (Pixel*) data, sizeof(Pixel)* myw * myh);
}

bool Image::load(const std::string& filename, bool flip) {
   if (mydata != NULL) {
      delete[] mydata;
      mydata = NULL;
   }

  Pixel* temp = (Pixel*) stbi_load(filename.c_str(),&myw, &myh, &numchannels, 3);

  if(temp != NULL){
     mydata = new Pixel[myw * myh];
     memcpy(mydata, temp, sizeof(Pixel)* myw * myh);
     stbi_image_free(temp);
     return true;
  }
  else{
     return false;
  }
  
}


bool Image::save(const std::string& filename, bool flip) const {
   if(mydata == NULL){
      return false;
   }
   else{
      stbi_write_png(filename.c_str(), myw, myh, numchannels, mydata, myw*3);
      return true;
   }
  
}

Pixel Image::get(int row, int col) const {
   Pixel temp = mydata[row*myw + col];
   return temp;
}

void Image::set(int row, int col, const Pixel& color) {
   mydata[row*myw + col] = color;
}


Pixel Image::get(int i) const{
   Pixel temp = mydata[i];
   return temp;
}

void Image::set(int i, const Pixel& c){
   mydata[i] = c;
}

Image Image::resize(int w, int h) const {
   Image result(w, h);
   float propr, propc;
   int oldr, oldc;
   for(int i = 0; i < h; i++){
      for(int j = 0; j < w; j++){
         propr = (float) i / (h-1);
         oldr = floor(propr * (myh - 1));
         propc = (float) j / (w - 1);
         oldc = floor(propc * (myw - 1));
         result.set(i, j, get(oldr, oldc));
      }
   }
   return result;
}

Image Image::flipHorizontal() const {
   Image result(myw, myh);
   int newh = 0;
   int oldh = myh;
   int curCol = 0;
   while (newh < myh){
      while(curCol < myw){
         result.set(newh, curCol, get(oldh,curCol));
         curCol++;
      }
      newh++;
      oldh--;
      curCol = 0;
   }
   return result;

}



Image Image::flipVertical() const {
   Image result(myw, myh);
   for(int i = 0; i < myh; i++){
      for(int j = 0; j < myw; j++){
         result.set(i, myw- 1 - j, get(i,j));  
      }
   }
   return result;
}

Image Image::rotate90() const {
   Image result(myh, myw);
   for(int i = 0; i < myh; i++){
      for(int j = 0; j < myw; j++){
         result.set(myw - 1 - j, i, get(i,j));
      }
   }
   return result;
}

Image Image::blur() const{
   Image result(myw, myh);
   for(int i = 0; i < myw; i++){
      for(int j = 0; j < myh; j++){
         Pixel cen; 
         int ar = 0;
         int ag = 0;
         int ab = 0;
         for(int k = i-1; k <= i+1; k++){
            for(int l = j-1; l <= j+1; l++){
               Pixel sec = get(k,l);
               ar = ar + sec.r;
               ag = ag + sec.g;
               ab = ab + sec.b;
            }
         }
         cen.r = ar/9;
         cen.g = ag/9;
         cen.b = ab/9;
         result.set(i,j,cen);
      }
   }
   return result;
}

Image Image::extract() const{
   Image result(myw, myh);
   for(int i = 0; i < myw; i++){
      for(int j = 0; j < myh; j++){
         Pixel temp = get(i,j);
         temp.r = 0;
         temp.g = 0;
         result.set(i,j,temp);
      }
   }
   return result;
}

Image Image::boarder() const{
   Image result(myw + 20, myh + 20);
   for(int i = 0; i < myw + 20; i++){
      for(int j = 0; j < myh + 20; j++){
         Pixel temp = {255,255,255};
         result.set(i,j,temp);
      } 
   }
   result.replace(*this, 10,10);

   return result;
}


Image Image::subimage(int startx, int starty, int w, int h) const {
   Image sub(w, h);
   for(int i = starty; i < starty + h; i++){
      for(int j = startx; j < startx + w; j++){
         sub.set(i - starty, j - startx, get(i,j));
      }
   }
    return sub;
}

void Image::replace(const Image& image, int startx, int starty) {
   for(int i = starty; i < (starty + image.height()); i++){
      for(int j = startx; j < (startx + image.width()); j++){
         set(i, j, image.get(i - starty,j - startx));
      }
   }
}

Image Image::swirl() const {
   Image result(myw, myh);
   for(int i = 0; i < myw; i++){
      for(int j = 0; j < myh; j++){
         Pixel temp = get(i,j);
         Pixel sw;
         sw.r = temp.g;
         sw.g = temp.b;
         sw.b = temp.r;
         result.set(i,j,sw);
      }
   }
   
   return result;
}

Image Image::add(const Image& other) const {
   Image result(0, 0);
  
   return result;
}

Image Image::subtract(const Image& other) const {
   Image result(0, 0);
   
   return result;
}

Image Image::multiply(const Image& other) const {
   Image result(0, 0);
   
   return result;
}

Image Image::difference(const Image& other) const {
   Image result(0, 0);
  
   return result;
}

Image Image::lightest(const Image& other) const {
   Image result(0, 0);
  
   return result;
}

Image Image::darkest(const Image& other) const {
   Image result(0, 0);
  
   return result;
}

Image Image::gammaCorrect(float gamma) const {
   Image result(myw, myh);
   float red,green,blue;
   for(int i = 0; i < myh; i++){
      for(int j = 0; j < myw; j++){
         struct Pixel temp =  get(i,j);
            red = pow(((float)temp.r / 255), (1/gamma));
            temp.r = red * 255;
            green = pow(((float)temp.g / 255), (1/gamma));
            temp.g = green * 255;
            blue = pow(((float)temp.b / 255), (1/gamma));
            temp.b = blue * 255;
            result.set(i,j,temp);
      }
   }
   return result;
}

Image Image::alphaBlend(const Image& other, float alpha) const {
   Image result(myw, myh);
   for(int i = 0; i < myh; i++){
      for(int j = 0; j < myw; j++){
         Pixel thisIm = get(i,j);
         Pixel otherIm = other.get(i,j);
         Pixel temp;
         temp.r = (thisIm.r * alpha) + (otherIm.r * (1 - alpha));
         temp.g = (thisIm.g * alpha) + (otherIm.g * (1 - alpha));
         temp.b = (thisIm.b * alpha) + (otherIm.b * (1 - alpha));
         result.set(i,j,temp);
      }
   }
   return result;
}

Image Image::invert() const {
   Image image(myw, myh);
   for(int i = 0; i < myh; i++){
      for(int j = 0; j < myh; j++){
         Pixel inv = get(i,j);
         inv.r = 255 - inv.r;
         inv.g = 255 - inv.g;
         inv.b = 255 - inv.b;
         image.set(i, j, inv);
      }
   }
   return image;
}

Image Image::grayscale() const {
   Image result(myw, myh);
   for(int i = 0; i < myh; i++){
      for(int j = 0; j < myw; j++){
         Pixel temp = get(i,j);
         unsigned char inten = (temp.r * 0.3) + (temp.g * 0.59) + (temp.b * 0.11);
         Pixel correct = {inten,inten,inten};
         result.set(i,j,correct);
      }
   }
   return result;
}

Image Image::colorJitter(int size) const {
   Image image(myw, myw);
   for(int i = 0; i < myw; i++){
      for(int j = 0; j < myh; j++){
         Pixel temp = get(i,j);
         temp.r = (temp.r - size) + (rand() % size *2);
         temp.g = (temp.g - size) + (rand() % size *2);
         temp.b = (temp.b - size) + (rand() % size *2);
         image.set(i,j,temp);
      }
   }
   return image;
}

Image Image::bitmap(int size) const {
   Image image(0, 0);
   
   return image;
}

void Image::fill(const Pixel& c) {
    for(int i = 0; i < (width() * height()); i++){
       set(i, c);
    }
  }

}  // namespace agl

