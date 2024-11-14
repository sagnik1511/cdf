#include <vector>
#include "include/cdf.hpp"


int main(){
    cdf::DataFrame df = cdf::io::read_csv("/Users/tensored/cdf/iris.csv");
    std::cout << df.shape().first << ", " << df.shape().second << "\n";
    df.head();
    
    cdf::DataFrame newDf = df.iloc(1, 10);

    newDf.head(7);

    return 0;
}

