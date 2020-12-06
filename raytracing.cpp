//this is a c++ version of a lab in my parallel computing course in c
#include <stdio.h>

#include <math.h>

#include <stdlib.h>

#include <vector>
// 640 by 480 pixel ppm image file
#define M 640
#define N 480

 //used to represent vectors, cartesian coordinates, and rgb color
 struct triple{
  double x;
  double y;
  double z;
};
 //sphere with center at c, radius r, and rgb color h
 struct sphere {
  triple c;
  double r;
  triple h;
};

// position of the eye
triple e = {
  0.50,
  0.50,
  -1.00
};
// position of the light
triple g = {
  0.00,
  1.25,
  -0.50
};
// return dot product of two vectors represented by triples
double dotp(triple & t, triple & u) {
  if (t.x * u.x + t.y * u.y + t.z * u.z >= 0) return t.x * u.x + t.y * u.y + t.z * u.z;
  return 0;

}
//take difference between two triples
void diff(triple * t, triple & u, triple & v)
{
  t -> x = u.x - v.x;
  t -> y = u.y - v.y;
  t -> z = u.z - v.z;
}

std::vector<sphere> s;

// create spheres to be displayed with raytracing
void init() {
  // large Peru-colored sphere acting as the floor
  s.push_back(sphere());
  s[0].c.x = 0.50;
  s[0].c.y = -20000.00;
  s[0].c.z = 0.50;
  s[0].r = 20000.25;
  s[0].h.x = 205;
  s[0].h.y = 133;
  s[0].h.z = 63;

  // blue sphere
  s.push_back(sphere());
  s[1].c.x = 0.50;
  s[1].c.y = 0.50;
  s[1].c.z = 0.50;
  s[1].r = 0.25;
  s[1].h.x = 0;
  s[1].h.y = 0;
  s[1].h.z = 255;

  // green sphere
  s.push_back(sphere());
  s[2].c.x = 1.00;
  s[2].c.y = 0.50;
  s[2].c.z = 1.00;
  s[2].r = 0.25;
  s[2].h.x = 0;
  s[2].h.y = 255;
  s[2].h.z = 0;

  // red sphere
  s.push_back(sphere());
  s[3].c.x = 0.00;
  s[3].c.y = 0.75;
  s[3].c.z = 1.25;
  s[3].r = 0.50;
  s[3].h.x = 255;
  s[3].h.y = 0;
  s[3].h.z = 0;

}

//helper method for finding the closest sphere on a path from the eye/light
double closestsphere(double & solution, double & solution1, double & min) {
  if (solution1 >= 0 && solution >= 0) {
    if (solution < solution1 && solution < min) return solution;

    else if (solution1 < solution && solution1 < min) return solution1;
  }
  else if (solution >= 0 && solution < min) return solution;

  else if (solution1 >= 0 && solution1 < min) return solution1;

  return -1;
}

int main() {
  // red-green-blue for each pixel
  int rgb[N][M][3];

  init();
  //generate image pixel by pixel
  for (int y = 0; y < N; y++) {
    //line of code run code with openmp in parallel
    //#pragma omp parallel for private(x)
    for (int x = 0; x < M; x++) {

      //each pixel has a unique line through it traced from the eye
      triple terminal = {
        (x + .5) / 480.0 - 1.0 / 6,
        1.0 - (y + .5) / 480.0,
        0
      };


      // set magn as a vector representing the straight line between the eye and the terminal
      triple magn;
      diff( & magn, terminal, e);
      double distance = sqrt(pow(magn.x, 2) + pow(magn.y, 2) + pow(magn.z, 2));
      magn.x = magn.x / distance;
      magn.y = magn.y / distance;
      magn.z = magn.z / distance;



      //find the sphere if one is in the line of sight from the eye to the pixel
      bool hasSphere = false;
      double min = 9999999;
      int sphere = -1;
      for (int d = 0; d < s.size(); d++) {
        double a = 1;
        double b = 2 * ((e.x - s[d].c.x) * magn.x + (e.y - s[d].c.y) * magn.y + (e.z - s[d].c.z) * magn.z);
        double c = pow(e.x - s[d].c.x, 2) + pow(e.y - s[d].c.y, 2) + pow(e.z - s[d].c.z, 2) - pow(s[d].r, 2);
        if (pow(b, 2) - 4 * a * c >= 0) {
          double solution = (-1 * b + sqrt(pow(b, 2) - 4 * a * c)) / (2 * a);
          double solution1 = (-1 * b - sqrt(pow(b, 2) - 4 * a * c)) / (2 * a);

          if (closestsphere(solution, solution1, min) >= 0) {
            min = closestsphere(solution, solution1, min);
            rgb[y][x][0] = s[d].h.x;
            rgb[y][x][1] = s[d].h.y;
            rgb[y][x][2] = s[d].h.z;
            sphere = d;
            hasSphere = true;
          }

          if (sphere == 0) {
            int checkx = (int)((e.x + magn.x * min) / .12);
            if (e.x + magn.x * min < 0) checkx = checkx - 1;

            int checkz = (int)((e.z + magn.z * min) / .12);
            if (e.z + magn.z * min < 0) checkz = checkz - 1;

            if ((abs(checkx) + abs(checkz)) % 2 == 0) {
              rgb[y][x][0] = 120;
              rgb[y][x][1] = 81;
              rgb[y][x][2] = 169;
            }

          }

        }

      }
      //Determine the shading of that sphere pixel based on the light's position if a sphere is in the line of sight for the eye
      if (hasSphere == true) {
        triple gterminal = {
          e.x + magn.x * min,
          e.y + magn.y * min,
          e.z + magn.z * min
        };
        triple magnlight;
        diff( & magnlight, g, gterminal);
        triple center = {
          s[sphere].c.x,
          s[sphere].c.y,
          s[sphere].c.z
        };

        distance = sqrt(pow(magnlight.x, 2) + pow(magnlight.y, 2) + pow(magnlight.z, 2));
        magnlight.x = magnlight.x / distance;
        magnlight.y = magnlight.y / distance;
        magnlight.z = magnlight.z / distance;
        //search for spheres obstructing the light's line of sight to the sphere pixel, which creates shadow
        bool inShadow = false;
        for (int d = 0; d < s.size(); d++) {
          double p = 1;
          double q = 2 * ((gterminal.x - s[d].c.x) * magnlight.x + (gterminal.y - s[d].c.y) * magnlight.y + (gterminal.z - s[d].c.z) * magnlight.z);
          double r = pow(gterminal.x - s[d].c.x, 2) + pow(gterminal.y - s[d].c.y, 2) + pow(gterminal.z - s[d].c.z, 2) - pow(s[d].r, 2);


          int c1 = rgb[y][x][0];
          int c2 = rgb[y][x][1];
          int c3 = rgb[y][x][2];

          if (pow(q, 2) - 4 * p * r >= 0) {
            double solution = (-1 * q + sqrt(pow(q, 2) - 4 * p * r)) / (2 * p);
            double solution1 = (-1 * q - sqrt(pow(q, 2) - 4 * p * r)) / (2 * p);
            if ((solution > 0.0001 || solution1 > 0.0001) && inShadow == false) {
              rgb[y][x][0] = .5 * rgb[y][x][0];
              rgb[y][x][1] = .5 * rgb[y][x][1];
              rgb[y][x][2] = .5 * rgb[y][x][2];
              inShadow = true;

            }
          }
        }
        //determine the shading of the sphere pixel if it isn't in a shadow using the normal vector at that pixel (lambert's law)
        triple normal;
        diff( & normal, gterminal, center);
        double distance1 = sqrt(pow(normal.x, 2) + pow(normal.y, 2) + pow(normal.z, 2));
        normal.x = normal.x / distance1;
        normal.y = normal.y / distance1;
        normal.z = normal.z / distance1;
        if (inShadow == false && dotp(normal, magnlight) >= 0) {
          rgb[y][x][0] = (int)(.5 * rgb[y][x][0] + .5 * rgb[y][x][0] * dotp(normal, magnlight));
          rgb[y][x][1] = (int)(.5 * rgb[y][x][1] + .5 * rgb[y][x][1] * dotp(normal, magnlight));
          rgb[y][x][2] = (int)(.5 * rgb[y][x][2] + .5 * rgb[y][x][2] * dotp(normal, magnlight));

        }

      }

    }
  }
  // create a ppm file of the generated image
  FILE* fout = fopen("raytracing.ppm", "w");

  fprintf(fout, "P3\n");
  fprintf(fout, "%d %d\n", M, N);
  fprintf(fout, "255\n");
  //
  for (int y = 0; y < N; y++) {
    for (int x = 0; x < M; x++) {

      fprintf(fout, "%d %d %d\n",
        rgb[y][x][0], rgb[y][x][1], rgb[y][x][2]);
    }
  }
  fclose(fout);

  return 0;
}
//
// end of file
//
