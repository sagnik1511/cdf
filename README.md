# CDF - C++ DataFrame

CDF is a lightweight data manipulation library, created to handle multiple functionalities of tabular data-manipulation and miscellaneous operations. 


 ## INSTALLATION

It is a header only cpp-library supported for C++17 and above.

Install C++ on [linux](https://stackoverflow.com/questions/46254629/how-can-install-cpp-compiler-on-ubuntu-terminal), you an find alternative solutions for windows and MacOS similarly on the web.

[1] To start, just clone the repository - 

 ```shell
 git clone https://github.com/sagnik1511/cdf.git
 ```
[2] Add the binaries to your include folder. Do a simple cp

```shell
cp CDF/include/*hpp <PROJECT_INCLUDE_DIR>
```


 ---

## QUICKSTART

The cdf API documentation can be accessed from - [CDF-Documentation](https://sagnik1511.github.io/cdf/)

The API comprises of simple data objects to use and manipulate data - 

### Example - 1 : Loading Data into DataFrame

#### From Raw Data

To Load Raw Data we have to pass row-wise information, column names and the data-types of the columns.

```cpp
#include "include/cdf.hpp"

int main() {
    std::vector<std::vector<_cdfVal>> data({{1, 25.5, "Alice"}, {2, "22.7", "Bob"}});
    std::vector<std::string> columnNames = {"ID", "Age", "Name"};
    std::vector<cdf::cdfDTypes> dTypes = {cdf::cdfDTypes::Integer, cdf::cdfDTypes::Double, cdf::cdfDTypes::String};

    cdf::DataFrame df(data, columnNames, dTypes);
    df.head();

    return 0;
}
```

Expected Output - 

```bash
+----+------+-------+
| ID | Age  | Name  |
+----+------+-------+
| 1  | 25.5 | Alice |
+----+------+-------+
| 2  | 22.7 | Bob   |
+----+------+-------+
```

#### From CSV Files

 Please Note, we have to pass the absolute path of the CSV file.

 ```cpp
#include "include/cdf.hpp"

int main() {
    cdf::DataFrame df = cdf::io::read_csv("sample_data.csv");
    df.head();

    return 0;
}
``` 

Expected Output - 

```shell
+----+-------+-----+---------------+---------+-------------+--------+
| ID | Name  | Age | City          | Country | Department  | Salary |
+----+-------+-----+---------------+---------+-------------+--------+
| 1  | Alice | 25  | New York      | USA     | Engineering | 75000  |
+----+-------+-----+---------------+---------+-------------+--------+
| 2  | Bob   | 30  | Los Angeles   | USA     | Marketing   |        |
+----+-------+-----+---------------+---------+-------------+--------+
| 3  | Carol | 22  | Chicago       |         | HR          | 50000  |
+----+-------+-----+---------------+---------+-------------+--------+
| 4  | David | 28  | San Francisco | USA     | Engineering | 95000  |
+----+-------+-----+---------------+---------+-------------+--------+
| 5  | Eve   | 35  |               | USA     | Sales       | 65000  |
+----+-------+-----+---------------+---------+-------------+--------+
```

#### Check Data Shape

```cpp
std::pair<int, int> shape = df.shape();
std::cout << "DataFrame Shape -> " << shape.first << ", " << shape.second << "\n";
```

Expected Output - 

```bash
DataFrame Shape -> 20, 7
```

#### Show top N rows

cdf::DataFrame::head() shows top N rows, examples are shown above only

```cpp
df.head();
```

#### Show Last N rows

While head method shows first N rows, `cdf::DataFrame::tail()` shows last N rows

```cpp
df.tail();
```

Expected Output - 

```bash
+----+--------+-----+---------+---------+-------------+--------+
| ID | Name   | Age | City    | Country | Department  | Salary |
+----+--------+-----+---------+---------+-------------+--------+
| 16 | Paul   | 34  | Atlanta | USA     | Sales       | 69000  |
+----+--------+-----+---------+---------+-------------+--------+
| 17 | Quincy | 38  | Denver  | USA     | Engineering | 90000  |
+----+--------+-----+---------+---------+-------------+--------+
| 18 | Rachel |     | Boston  | USA     | HR          | 52000  |
+----+--------+-----+---------+---------+-------------+--------+
| 19 | Samuel | 28  | Seattle | USA     | Marketing   | 68000  |
+----+--------+-----+---------+---------+-------------+--------+
| 20 | Tina   | 33  | Chicago | USA     | Sales       | 73000  |
+----+--------+-----+---------+---------+-------------+--------+
```

---

### Example - 2 : Select required columns or rows


#### Select Random Rows by their Indexes

```cpp
// Filter Selected Rows
std::vector<int> indexes = {2, 4, 6, 5};
df = df.filter(indexes);
df.head();
```

Expected Output - 

```bash
+----+-------+-----+---------+---------+-------------+--------+
| ID | Name  | Age | City    | Country | Department  | Salary |
+----+-------+-----+---------+---------+-------------+--------+
| 3  | Carol | 22  | Chicago |         | HR          | 50000  |
+----+-------+-----+---------+---------+-------------+--------+
| 5  | Eve   | 35  |         | USA     | Sales       | 65000  |
+----+-------+-----+---------+---------+-------------+--------+
| 7  | Grace | 26  | Miami   | USA     | Engineering | 85000  |
+----+-------+-----+---------+---------+-------------+--------+
| 6  | Frank | 40  | Boston  | USA     |             | 70000  |
+----+-------+-----+---------+---------+-------------+--------+
```

#### Select Random Columns

```cpp
// Filter Selected Columns
std::vector<std::string> filteredColumns = {"ID", "Age", "Name"};
df = df[filteredColumns];
df.head();
```

Expected Output - 

```shell
+----+-----+-------+
| ID | Age | Name  |
+----+-----+-------+
| 1  | 25  | Alice |
+----+-----+-------+
| 2  | 30  | Bob   |
+----+-----+-------+
| 3  | 22  | Carol |
+----+-----+-------+
| 4  | 28  | David |
+----+-----+-------+
| 5  | 35  | Eve   |
+----+-----+-------+
```

#### Select a range of Rows and Columns

```cpp
// Filter Selected Row and Column Ranges
df = df.iloc(10, 13, "City", "Department");
df.head();
```

Expected Output - 

```bash
+----------+---------+-------------+
| City     | Country | Department  |
+----------+---------+-------------+
| Austin   | USA     | Engineering |
+----------+---------+-------------+
| Dallas   | USA     | Sales       |
+----------+---------+-------------+
| Orlando  | USA     | HR          |
+----------+---------+-------------+
| San Jose | USA     | Marketing   |
+----------+---------+-------------+
```