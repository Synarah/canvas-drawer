/*-----------------------------------------------
 * Author:
 * Date:
 * Description:
 ----------------------------------------------*/

#ifndef canvas_H_
#define canvas_H_

#include <string>
#include <vector>
#include <cmath>
#include "image.h"

namespace agl
{
   enum PrimitiveType {UNDEFINED, LINES, TRIANGLES};
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

      // Fill the canvas with the given background color
      void background(unsigned char r, unsigned char g, unsigned char b);

   private:
      Image _canvas;
      //int myh;
      //int myw;
      Pixel curCol;
      Pixel bg;
      PrimitiveType mtype;
      PonAndcol temp;
      std::vector<PonAndcol> vert;

      void drawLine(PonAndcol a, PonAndcol b);
      
      void posSlo(PonAndcol a, PonAndcol b);

      void negSlo(PonAndcol a, PonAndcol b);

      void triDraw(PonAndcol a, PonAndcol b, PonAndcol c);

      float implicit(PonAndcol p, PonAndcol p1, PonAndcol p2);

   };
}

#endif


