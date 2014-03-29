#include <future>
#include <iostream>
#include <cmath>
#include <chrono>
#include <vector>
#include <string>
#include <sstream>
#include <functional>
#include "ppm.h"
#include "PerlinNoise.h"

// lttng-ust trace points
#define TRACEPOINT_DEFINE
#define TRACEPOINT_PROBE_DYNAMIC_LINKAGE
#include "tp_lttng_test.h"

void generateNoise(const PerlinNoise& pn, 
                   unsigned int width, 
                   unsigned int height,
                   unsigned int thread,
                   unsigned int nbrOfThreads,
                   double z, 
                   ppm& image) {

  
  auto workHeight = height / nbrOfThreads;
 
  if(thread == (nbrOfThreads-1))
  {
    workHeight += height % nbrOfThreads;
  }

  auto startHeight = (thread*workHeight);
  auto endHeight = startHeight+workHeight;

  auto px = startHeight*width;

  //std::cout << "Thread: " << thread << "(Nbr of threads: " << nbrOfThreads << "), workHeight: " \
  //  << workHeight << " startHeight: " << startHeight << " endHeight: " << endHeight << " px: " << px << std::endl;

  // Visit every pixel in the specified range and assign a color generated with Perlin noise
  for(unsigned int i = startHeight; i < endHeight; ++i) {  // y
    for(unsigned int j = 0; j < width; ++j) { // x
      double x = (double)j/((double)width);
      double y = (double)i/((double)height);

      // Wood like structure
      double n = 20 * pn.noise(x, y, z);
      n = n - floor(n);

      // Map the values to the [0, 255] interval, for simplicity we use
      // tones of grey
      image.r[px] = floor(255.0 * n);
      image.g[px] = floor(255.0 * n);
      image.b[px] = floor(255.0 * n);
      ++px;
    }
  }
}


// Given z as input generate a 2D image from Perlin noise
void createImage( unsigned int nbrOfThreads,
                  const std::string& fname, 
                  unsigned int width, 
                  unsigned int height, 
                  double z,
                  unsigned int seed = 237) {

  // Create an empty PPM image
  ppm image(width, height);

  // Create a PerlinNoise object with a random permutation vector generated with seed
  PerlinNoise pn(seed);

  // List of futures all performing a part of the work
  std::vector<std::future<void>> futures;
 
  //start the first part in a auto mode (either deffered or async)
  futures.push_back(std::async(generateNoise,pn,width,height,0,nbrOfThreads,z,std::ref(image)));

  //all other parts will be started in async mode (new thread)
  for(auto thread = 1; thread < nbrOfThreads; ++thread) {
    futures.push_back(std::async(std::launch::async,generateNoise,pn,width,height,thread,nbrOfThreads,z,std::ref(image)));
  }

  // wait for all the threads
  // Note that the first get call will execute a part of the work on the main thread
  for(auto &e : futures) {
    e.get();
  }

  // Save the image in a PPM file
  image.write(fname);	
}

int main ( int arc, char **argv ) {

  tracepoint(com_vanwinkeljan_lttng_test_main, info_message, "Entering main of lttng_test");

  const unsigned int MAX_NBR_OF_THREADS = 4;
  const double Z = 0.5;
  const unsigned int WIDTH  = 1280;
  const unsigned int HEIGHT = 720;

  for(unsigned int nbrOfThreads = 1; nbrOfThreads <= MAX_NBR_OF_THREADS; ++nbrOfThreads){
    std::stringstream fname;
    fname << "img_" << nbrOfThreads << ".ppm"; 
    auto start = std::chrono::steady_clock::now();
    tracepoint(com_vanwinkeljan_lttng_test_main, test_run_message, "Starting test",nbrOfThreads);
    createImage(nbrOfThreads,fname.str(), WIDTH, HEIGHT, Z);
    tracepoint(com_vanwinkeljan_lttng_test_main, test_run_message, "Test Done",nbrOfThreads);
    auto end = std::chrono::steady_clock::now();
    auto diff = end - start;
    std::cout << "Nbr. of threads " << nbrOfThreads << " duration = " << std::chrono::duration <double, std::milli> (diff).count() << " ms" << std::endl;
  }

  return 0;
}
