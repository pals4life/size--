#include "pal.h"

int main(int argc, char** argv)
{
    std::vector<std::string> arguments(argv + 1, argv + argc);

//    pal::encode("pal.txt", "pal.pal", Algorithm::sequitur);
//    pal::decode("pal.pal", "nwww.txt");

    pal::encode("photo.bmp", "photo.pal", Algorithm::sequitur);
    pal::decode("photo.pal", "new.bmp");
}