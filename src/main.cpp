#include "pal.h"

int main(int argc, char** argv)
{
    std::vector<std::string> arguments(argv + 1, argv + argc);

    pal::encode("photo.bmp", "photo.pal", Algorithm::none);
//    pal::decode("photo.pal", "new.bmp");
}