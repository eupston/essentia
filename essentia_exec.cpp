//
// Created by eugene upston on 2/14/21.
//
#include <essentia/algorithmfactory.h>
#include <essentia/essentiamath.h>
#include <essentia/pool.h>

using namespace essentia;
using namespace essentia::standard;

int main(int argc, char* argv[]) {
    essentia::init();
    Pool pool;
    AlgorithmFactory& factory = standard::AlgorithmFactory::instance();
    std::cout << "hellloo essentia" << std::endl;
    return 0;
}