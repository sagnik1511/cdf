
#include "include/cdf.hpp"

int main() {
    // Load the dataframe from CSV File
    cdf::DataFrame df = cdf::io::read_csv("/Users/tensored/cdf/titanic.csv");

    // Perform filtration
    df = df[df["Survived"] == 1];

    std::vector<std::string> fields = {"Sex", "PassengerId", "Age", "Fare", "Name", "Embarked"};

    df = df[fields];

    // Print the head of the dataframe
    df.head();

    auto ans = df["Age"].sum();

    std::cout << ans << "\n";
    std::cout << df["Age"].mean() << "\n";

    std::cout << "Mode of Sex is -> " << df["Sex"].mode() << "\n";

    double age = df["Age"].mode<double>();

    std::cout << "Mode of Age is -> " << age << "\n";

    std::vector<int> pid = {2, 20, 4, 5};
    df = df[df["PassengerId"].isin(pid)];

    df.head(10);

    return 0;
}
