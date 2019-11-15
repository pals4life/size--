#include <iostream>
#include "pal/pal.h"
#include "pal/io.h"

int main()
{
    const auto temp = pal::readFile("photo.bmp");
    std::vector<uint32_t> string(temp.size());
    std::transform(temp.begin(), temp.end(), string.begin(), [](uint8_t c) -> uint32_t { return c; });

    pal::encode("test.pal", string, {});
    pal::decode("test.pal", "new.bmp");
}