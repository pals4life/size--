#include "pal.h"

int main(int argc, char** argv)
{
    std::vector<std::string> arguments(argv + 1, argv + argc);

    pal::encode("beemovie.txt", "beemovie.pal", Algorithm::sequitur);
//    pal::decode("photo.pal", "new.bmp");
}