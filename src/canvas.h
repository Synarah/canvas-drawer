/*-----------------------------------------------
 * Author: Synarah Sitaf
 * Date: 2/22/2023
 * Description: It's the base class where all the
 * functions are created and then they are all 
 * used and filled in canvas.cpp
 ----------------------------------------------*/

#ifndef canvas_H_
#define canvas_H_

#include <string>
#include <vector>
#include <cmath>
#include "image.h"

namespace agl
{
   enum PrimitiveType {UNDEFINED, LINES, TRIANGLES, CIRCLE, BURST, HDNA, SOUT, CROUT};
   struct PonAndcol{
      int x;
      int y; 
      Pixel col;
   };
   class Canvas
   {
   public:
      Canvas(int w, int h);
      virtual ~Canvas();

      // Save to file
      void save(const std::string& filename);

      // Draw primitives with a given type (either LINES or TRIANGLES)
      // For example, the following draws a red line followed by a green line
      // begin(LINES);
      //    color(255,0,0);
      //    vertex(0,0);
      //    vertex(100,0);
      //    color(0,255,0);
      //    vertex(0, 0);
      //    vertex(0,100);
      // end();
      void begin(PrimitiveType type);
      void end();

      // Specifiy a vertex at raster position (x,y)
      // x corresponds to the column; y to the row
      void vertex(int x, int y);

      // Specify a color. Color components are in range [0,255]
      void color(unsigned char r, unsigned char g, unsigned char b);


      void starDraw(PonAndcol s);

      // Fill the canvas with the given background color
      void background(unsigned char r, unsigned char g, unsigned char b);

      void cir(PonAndcol p);

      void setRad(int r);

      void setInv(float i);

      void setK(float k);

      void burst(PonAndcol c);

      void DNA(PonAndcol l, PonAndcol r);

      void sqOutline(PonAndcol a, PonAndcol b, PonAndcol c, PonAndcol d);

      void cOut(PonAndcol p);

   private:
      Image _canvas;
      //int myh;
      //int myw;
      Pixel curCol;
      Pixel bg;
      PrimitiveType mtype;
      PonAndcol temp;
      std::vector<PonAndcol> vert;
      float k;
      float inv;
      int r;

      void drawLine(PonAndcol a, PonAndcol b);
      
      void posSlo(PonAndcol a, PonAndcol b);

      void negSlo(PonAndcol a, PonAndcol b);

      void triDraw(PonAndcol a, PonAndcol b, PonAndcol c);

      float implicit(PonAndcol p, PonAndcol p1, PonAndcol p2);

      

   };
}

#endif


