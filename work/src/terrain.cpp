#include <string>
#include <stdexcept>
#include <vector>

#include "cgra_math.hpp"
#include "terrain.hpp"
#include "imageLoader.hpp"
#include "opengl.hpp"

using namespace std;
using namespace cgra;

Terrain::Terrain(){

}

float Terrain::noise3(vec3 point){
  return 0;
}

/* Ridged multifractal terrain model.
 *
 * Some good parameter values to start with:
 *
 *  H:      1.0
 *  offset: 1.0
 *  gain:   2.0
 *
 * Taken from the book "Texturing and modeling: aprocedural approach"
 * by D.S.Ebert, F K. Musgrave, D. Peachey, K. Perlin, S. Worley
 */

 float Terrain::ridgedMultifractal(vec3 point, float H, float lacunarity,
                          float octaves, float offset, float gain )
 {
   float result, frequency, sig, weight;
    //Terrain::noise3();
   static int first = true;
   static float *exponent_array;

   /* precompute and store spectral weights */
   if(first){
     /* seize required memory for exponent_array */
     exponent_array = (float *)malloc((octaves+1) * sizeof(float));
     frequency = 1.0;
     for(int i = 0; i<=octaves; i++){
       /* computer wight for each frequency */
       exponent_array[i] = pow(frequency, -H);
       frequency *= lacunarity;
     }
     first = false;
   }

   /* get first octave */
   sig = Terrain::noise3(point);
   /* get absolute value of signal (this creates the  ridges) */
   if(sig <= 0.0) sig = -sig;
   /* invert and translate (note that "offset" should be ~= 1.0) */
   sig = offset - sig;
   /* square the signal, to increase "sharpness" of ridges */
   sig *= sig;
   /* assign initial values */
   result = sig;
   weight = 1.0;

   for(int i = 1; i<octaves; i++){
     /* increase the frequency */
     point.x *= lacunarity;
     point.y *= lacunarity;
     point.z *= lacunarity;

     /* weight successive contributions by previous signal */
     weight = sig * gain;
     if(weight > 1.0) weight = 1.0;
     if(weight < 0.0) weight = 0.0;
     sig = Terrain::noise3(point);
     if(sig < 0.0) sig = -sig;
     sig = offset - sig;
     sig *= sig;
     /* weight the contribution */
     sig *= weight;
     result += sig * exponent_array[i];
   }
   return result;
 } /* RidgedMultifractal() */
