#include "canvas.h"
#include <cassert>
#include "math.h"
#include <cmath>

#define _USE_MATH_DEFINES

using namespace std;
using namespace agl;

Canvas::Canvas(int w, int h) : _canvas(w, h){
   Image image(w,h);
   this->_canvas = image;
}

Canvas::~Canvas(){

}

void Canvas::save(const std::string& filename){
   _canvas.save(filename);
}

void Canvas::begin(PrimitiveType type){
   mtype = type;
   vert.clear();
}

void Canvas::end(){
   if(mtype == LINES && vert.size() % 2 == 0){
      for(int i = 0; i < vert.size(); i += 2){
         drawLine(vert[i], vert[i + 1]);
      }
   }
   else if(mtype == TRIANGLES && vert.size() % 3 == 0){
      for(int i = 0; i < vert.size(); i += 3){
         triDraw(vert[i], vert[i + 1], vert[i + 2]);
      }
   }
   else if(mtype == CIRCLE && vert.size() == 1){
      cir(vert[0]);
   }
   else if(mtype == BURST && vert.size() > 0){
      burst(vert[0]);
   }
   else if(mtype == HDNA && vert.size() % 2 == 0){
      DNA(vert[0], vert[1]);
   }else if(mtype == SOUT && vert.size() % 4 == 0 && vert.size() > 0){
      sqOutline(vert[0], vert[1], vert[2], vert[3]);
   }else if(mtype == CROUT && vert.size() > 0){
      cOut(vert[0]);
   }
   mtype = UNDEFINED;
   vert.clear();
}

void Canvas::vertex(int x, int y){
   temp.x = x;
   temp.y = y;
   temp.col = curCol;
   vert.push_back(temp);
}

void Canvas::color(unsigned char r, unsigned char g, unsigned char b){
   curCol.r = r;
   curCol.g = g;
   curCol.b = b;
}

void Canvas::background(unsigned char r, unsigned char g, unsigned char b){
   bg.r = r;
   bg.g = g;
   bg.b = b;

   _canvas.fill(bg);
}

void Canvas::drawLine(PonAndcol a, PonAndcol b){
   int W = b.x - a.x;
   int H = b.y -a.y;

   if(abs(H) < abs(W)){
      if(a.x > b.x){
         negSlo(b,a);
      }
      else{
         negSlo(a,b);
      }
   }
   else{
      if(a.y > b.y){
         posSlo(b,a);
      }
      else{
         posSlo(a,b);
      }
   }
}

void Canvas::posSlo(PonAndcol a, PonAndcol b){
   int x = a.x;
   int W = b.x -a.x;
   int H = b.y -a.y;
   int dx = 1;

   if(W < 0){
      dx = -1;
      W = -1 * W;
   }

   int F = 2 * W - H;

   for(int y = a.y; y <= b.y; y++){
      float t = sqrt(pow(vert[0].x - x, 2) + pow(vert[0].y - y, 2))/sqrt(pow(vert[1].x - vert[0].x, 2) + pow(vert[1].y - vert[0].y, 2));
      Pixel temps; 
      temps.r = vert[0].col.r * (1 - t) + vert[1].col.r * t;
      temps.g = vert[0].col.g * (1 - t) + vert[1].col.g * t;
      temps.b = vert[0].col.b * (1 - t) + vert[1].col.b * t;
      _canvas.set(y,x,temps);

      if(F > 0){
         x += dx;
         F += 2 * (W - H);
      }
      else{
         F += 2 * W;
      }
   }
}

void Canvas::negSlo(PonAndcol a, PonAndcol b){
   int y = a.y;
   int W = b.x - a.x;
   int H = b.y - a.y;
   int dy = 1;

   if(H < 0){
      dy = -1;
      H = -1 * H;
   }

   int F = 2 * H - W;
   for(int x = a.x; x <= b.x; x++){
      float t = sqrt(pow(vert[0].x - x, 2) + pow(vert[0].y - y, 2))/sqrt(pow(vert[1].x - vert[0].x, 2) + pow(vert[1].y - vert[0].y, 2));
      Pixel temps; 
      temps.r = vert[0].col.r * (1 - t) + vert[1].col.r * t;
      temps.g = vert[0].col.g * (1 - t) + vert[1].col.g * t;
      temps.b = vert[0].col.b * (1 - t) + vert[1].col.b * t;
      _canvas.set(y,x,temps);

      if(F > 0){
         y += dy;
         F += 2 * (H - W);
      }
      else {
         F += 2 * H;
      }
   }
}

void Canvas::triDraw(PonAndcol a, PonAndcol b, PonAndcol c){
   int xmax = max(a.x,b.x);
   xmax = max(xmax, c.x);
   int xmin = min(a.x,b.x);
   xmin = min(xmin, c.x);
   int ymax = max(a.y,b.y);
   ymax = max(ymax, c.y);
   int ymin = min(a.y,b.y);
   ymin = min(ymin, c.y);

   for(int y = ymin; y < ymax; y++){
      for(int x = xmin; x < xmax; x++){
         temp.x = x;
         temp.y = y;
         temp.col = curCol;
         float al =  implicit(temp, b, c) / implicit(a, b, c);
         float be = implicit(temp, c, a) / implicit(b, c, a);
         float ga = implicit(temp, a, b) / implicit(c, a, b);

         if(al > 0 && be > 0 && ga > 0){
            Pixel temps; 
            temps.r = a.col.r * al + b.col.r * be + c.col.r * ga;
            temps.g = a.col.g * al + b.col.g * be + c.col.g * ga;
            temps.b = a.col.b * al + b.col.b * be + c.col.b * ga;
            _canvas.set(y,x,temps);
         }
      }
   }
}

float Canvas::implicit(PonAndcol p, PonAndcol p1, PonAndcol p2){
   float im = ((float)((p1.y - p2.y) * p.x) + (float)((p2.x - p1.x) * p.y) + (float)(p1.x * p2.y) - (float)(p2.x * p1.y));
   return im;
}

void Canvas::starDraw(PonAndcol s){

}

void Canvas::setRad(int x){
   r = x;
}

void Canvas::cir(PonAndcol p){
   int s = r/5;
   int ymin = p.y - r;
   int xmin = p.x - r;
   int ymax = p.y + r;
   int xmax = p.x + r;
   if(ymin <0){
      ymin = 0;
   }
   if(xmin < 0){
      xmin = 0;
   }
   if(ymax >= _canvas.height()){
      ymax = _canvas.height() - 1;
   }
   if(xmax >= _canvas.width()){
      xmax = _canvas.width() -1;
   }

   for(int i = ymin; i < ymax; i++){
      for(int j = xmin; j < xmax; j++){
         float d = sqrt(pow(i-p.y,2) + pow(j-p.x,2));
         if(d <= r){
            if(d <= (r-s)){
               if(d <= (r-(2*s))){
                  PonAndcol t;
                  t.col.r = p.col.r;
                  t.col.g = p.col.b;
                  t.col.b = p.col.g;
                  _canvas.set(i, j, t.col);
               }else{
                  PonAndcol t;
                  t.col.r = p.col.b;
                  t.col.g = p.col.g;
                  t.col.b = p.col.r;
                  _canvas.set(i, j, t.col);
               }
            }else{
               _canvas.set(i, j, p.col);
            }
         }
      }
   }
}

void Canvas::setInv(float i){
   inv = i;
}

void Canvas::setK(float x){
   k = x;
}

void Canvas::burst(PonAndcol c){

   float r2 = (float)r;
   float x, y;

      for(float a = 0; a < 4 * M_PI ; a +=inv){
         begin(LINES);
         x = r2 * cos(k*a) * cos(a);
         vertex(c.x + x, c.y + y);
         y = r2 * sin(k*a) * sin(a);
         vertex(c.x -x, c.y -y);
         end();
      }
}

void Canvas::DNA(PonAndcol l, PonAndcol b){

   float x = l.x;
   float y = l.y;
   float x2 = l.x;
   float y2 = l.y;
   float lf = 0;
   for(float f = l.x; f < b.x; f+= 0.5){
      x = f;
      x2 = f;
      y2 = l.y +(r  *(float)sin(f * 5 * (M_PI / 180))); // *10
      y = l.y +(r * (float)cos(f * 5 * (M_PI / 180))); // * 10
      
      if((f - lf) > 3.0){
         begin(LINES);
         vertex(x, y);
         vertex(x2, y2);
         end();
         lf = f;
      }
      _canvas.set(y, x, l.col);
      _canvas.set(y2, x2, l.col);
   }
}

void Canvas::sqOutline(PonAndcol a, PonAndcol b, PonAndcol c, PonAndcol d){

   begin(LINES);
   vertex(a.x, a.y);
   vertex(b.x, b.y);
   end();
   
   begin(LINES);
   vertex(b.x, b.y);
   vertex(c.x, c.y);
   end();
   
   begin(LINES);
   vertex(c.x, c.y);
   vertex(d.x, d.y);
   end();
   
   begin(LINES);
   vertex(d.x, d.y);
   vertex(a.x, a.y);
   end();
}

void Canvas::cOut(PonAndcol p){
   int ymin = p.y - r;
   int xmin = p.x - r;
   int ymax = p.y + r;
   int xmax = p.x + r;
   if(ymin <0){
      ymin = 0;
   }
   if(xmin < 0){
      xmin = 0;
   }
   if(ymax >= _canvas.height()){
      ymax = _canvas.height() - 1;
   }
   if(xmax >= _canvas.width()){
      xmax = _canvas.width() -1;
   }

   for(int i = ymin; i < ymax; i++){
      for(int j = xmin; j < xmax; j++){
         float d = sqrt(pow(i-p.y,2) + pow(j-p.x,2));
         if(d <= r && d > (r-1)){
            _canvas.set(i, j, p.col);
         }
      }
   }
}