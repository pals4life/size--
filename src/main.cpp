#include "pal.h"


int main(int argc, char** argv)
{
    pal::encode("input/chessboard.bmp", "compare/chessboard.pal", Algorithm::repair);
    pal::decode("compare/chessboard.pal", "output/chessboard.bmp");

    pal::encode("input/shrek.bmp", "compare/shrek.pal", Algorithm::repair);
    pal::decode("compare/shrek.pal", "output/shrek.bmp");

    pal::encode("input/blue.bmp", "compare/blue.pal", Algorithm::repair);
    pal::decode("compare/blue.pal", "output/blue.bmp");

    pal::encode("input/sphere.bmp", "compare/sphere.pal", Algorithm::repair);
    pal::decode("compare/sphere.pal", "output/sphere.bmp");

    pal::encode("input/text.txt", "compare/text.pal", Algorithm::repair);
    pal::decode("compare/text.pal", "output/text.txt");
}