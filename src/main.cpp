#include "pal.h"

int main(int argc, char** argv)
{
    pal::encode("test/test.txt", "test/test.pal", Algorithm::sequitur);

}