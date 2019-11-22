#include "pal.h"

int main(int argc, char** argv)
{
    pal::encode("input/chessboard.bmp", "compressed/chessboard.pal", Algorithm::bisection);
    pal::decode("compressed/chessboard.pal", "output/chessboard.bmp");

    pal::encode("input/shrek.bmp", "compressed/shrek.pal", Algorithm::bisection);
    pal::decode("compressed/shrek.pal", "output/shrek.bmp");

    pal::encode("input/blue.bmp", "compressed/blue.pal", Algorithm::bisection);
    pal::decode("compressed/blue.pal", "output/blue.bmp");

    pal::encode("input/sphere.bmp", "compressed/sphere.pal", Algorithm::optimisedBisection);
    pal::decode("compressed/sphere.pal", "output/sphere.bmp");

    pal::encode("input/text.txt", "compressed/text.pal", Algorithm::bisection);
    pal::decode("compressed/text.pal", "output/text.txt");
}