#include <iostream>
#include "canvas.h"

using namespace agl;
using namespace std;

void test_line(Canvas& drawer, int ax, int ay, int bx, int by, const std::string& savename)
{
   drawer.background(0, 0, 0);
   drawer.begin(LINES);
   drawer.vertex(ax, ay);
   drawer.vertex(bx, by);
   drawer.end();
   drawer.save(savename);
}

int main(int argc, char** argv)
{
   Canvas drawer(100, 100);

   drawer.color(255, 255, 255);
   test_line(drawer, 0, 50, 100, 50, "horizontal-line.png");
   test_line(drawer, 50, 0, 50, 100, "vertical-line.png");
   test_line(drawer, 0, 0, 100, 100, "diagonal-line-1.png"); // slope = 1
   test_line(drawer, 25, 10, 75, 25,  "h-lessthan-w-line-1.png"); // slope H < W
   test_line(drawer, 25, 25, 75, 75,  "w-lessthan-h-line-1.png"); // slope W < H
   test_line(drawer, 0, 100, 100, 0, "diagonal-line-2.png"); // slope H < W
   test_line(drawer, 25, 90, 75, 75,  "h-lessthan-w-line-2.png"); // slope H < W
   test_line(drawer, 25, 90, 75, 25,  "w-lessthan-h-line-2.png"); // slope W < H

   // test line interpolation
   drawer.background(0, 0, 0);
   drawer.begin(LINES);
   drawer.color(255, 0, 255);
   drawer.vertex(0, 0);
   drawer.color(0, 255, 255);
   drawer.vertex(100, 100);
   drawer.end();
   drawer.save("line-color-interpolation.png");

   // test triangle with interpolation
   drawer.background(0, 0, 0);
   drawer.begin(TRIANGLES);
   drawer.color(255, 0, 255);
   drawer.vertex(10, 0);
   drawer.color(0, 255, 255);
   drawer.vertex(90, 50);
   drawer.color(255, 255, 0);
   drawer.vertex(10, 90);
   drawer.end();
   drawer.save("triangle.png");

   // test triangle with interpolation
   drawer.background(0, 0, 0);
   drawer.begin(TRIANGLES);
   drawer.color(255, 0, 255);
   drawer.vertex(10, 10);
   drawer.vertex(10, 90);
   drawer.vertex(90, 90);

   drawer.color(255, 255, 0);
   drawer.vertex(90, 90);
   drawer.vertex(90, 10);
   drawer.vertex(10, 10);
   drawer.end();
   drawer.save("quad.png");
   
   Canvas c(100,100);
   c.begin(CIRCLE);
   c.setRad(25);
   c.color(200, 150, 100);
   c.vertex(50, 50);
   c.end();
   c.save("cir.png");

   Canvas r(500, 500);
   r.begin(BURST);
   r.color(100, 200, 200);
   r.setRad(200);
   r.setK(4);
   r.setInv(0.03);
   r.vertex(250,250);
   r.end();
   r.save("burst.png");

   Canvas dna(500, 500);
   dna.begin(HDNA);
   dna.color(200, 100, 100);
   dna.setRad(20);
   dna.vertex(100, 250);
   dna.vertex(400, 250);
   dna.end();
   dna.save("dna2.png");

   Canvas rect(500,500);
   rect.begin(SOUT);
   rect.color(100, 200, 100);
   rect.vertex(100, 100);
   rect.vertex(100, 400);
   rect.vertex(400, 400);
   rect.vertex(400, 100);
   rect.end();
   rect.save("rect.png");

   Canvas co(500, 500);
   co.begin(CROUT);
   co.vertex(250,250);
   co.setRad(75);
   co.end();
   co.save("cout.png");
   


   return 0;
}
