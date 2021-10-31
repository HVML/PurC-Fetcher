#include <cstdlib>

extern "C" int NetworkProcessMain(int argc, char** argv);

int main(int argc, char** argv)
{
    return NetworkProcessMain(argc, argv);
}
