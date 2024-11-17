
#include "include/cdf.hpp"

int main() {
    // Load the dataframe from CSV File
    cdf::DataFrame df = cdf::io::read_csv("/Users/tensored/cdf/titanic.csv");

    // Perform filtration
    df = df[df["Age"] == 35];

    // Print the head of the dataframe
    df.head();

    return 0;
}
