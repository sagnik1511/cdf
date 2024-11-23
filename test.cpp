
#include "include/cdf.hpp"

int main() {
    // Load the dataframe from CSV File
    cdf::DataFrame df = cdf::io::read_csv("/Users/tensored/cdf/titanic.csv");

    // Perform filtration
    df = df[df["Survived"] == 1];

    std::vector<std::string> fields = {"Sex", "PassengerId", "Age"};

    df = df[fields];

    // Print the head of the dataframe
    df.head();

    auto ans = df["Age"].sum();

    std::cout << ans << "\n";
    std::cout << df["Age"].mean() << "\n";

    std::cout << "Mode of Age is -> " << df["Sex"].mode() << "\n";

    double age = df["Age"].mode<double>();

    return 0;
}
