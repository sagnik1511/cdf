
#include "include/cdf.hpp"

int main() {
    cdf::DataFrame df = cdf::io::read_csv("/Users/tensored/cdf/iris.csv");
    std::cout << df.shape().first << ", " << df.shape().second << "\n";
    df.head();

    cdf::DataFrame newDf = df.iloc(1, 17, "", "petal.length");

    newDf.head(15);

    float x = 3.7;

    newDf = newDf[newDf["sepal.width"] == x];

    newDf.head();

    return 0;
}
