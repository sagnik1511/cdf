#include <vector>
#include "include/cdf.hpp"


int main(){
    cdf::DataFrame df = cdf::io::read_csv("/Users/tensored/cdf/iris.csv");
    df.head(5);
    df.tail(5);
    std::cout << df.shape.first << "," << df.shape.second << "\n";
    df = df.loc("sepal.length", 100, 126);
    std::cout << df.shape.first << "," << df.shape.second << "\n";
    df.head(5);
    return 0;
}

