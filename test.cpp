
#include "include/cdf.hpp"

int main() {
    cdf::DataFrame df = cdf::io::read_csv("/Users/tensored/cdf/titanic.csv");
    std::cout << df.shape().first << ", " << df.shape().second << "\n";

    df.head();

    cdf::DataFrame newDf = df.iloc(1, 10, "", "Fare");

    newDf = newDf[newDf["Fare"] > 30.0];

    newDf.head();

    return 0;
}
