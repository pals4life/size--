#include "pal.h"

int main(int argc, char** argv)
{
    pal::encode("../test/test.txt", "../test/test.pal", Algorithm::sequitur);
    //pal::encode("../test/beemovie.txt", "../test/beemovie.pal", Algorithm::sequitur);
    //pal::encode("../input/sphere.bmp", "../input/d.pal", Algorithm::sequitur);

}