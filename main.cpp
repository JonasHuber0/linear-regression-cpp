#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <numeric> // required for accumulate
#include <cmath>   // required for pow

using namespace std; // no risk of name collision in small project

// simple structure to hold x,y data points
struct Point {
    double x;
    double y;
};

// function to read the CSV file and return a vector of Points
vector<Point> read_csv(const string& filename) {
    vector<Point> points;
    ifstream file(filename);
    string line;

    // check if the file was opened successfully
    if (!file.is_open()) {
        cerr << "Error: Could not open the file " << filename << endl;
        return points; // return an empty vector
    }

    // read the file line by line
    while (getline(file, line)) {
        stringstream ss(line);
        string value;
        Point p;

        // get the x-value (before the comma)
        if (getline(ss, value, ',')) {
            try {
                p.x = stod(value);
            } catch (const invalid_argument& e) {
                cerr << "Warning: Skipping invalid x-value '" << value << "' on line: " << line << endl;
                continue;
            }
        }

        // get the y-value (after the comma)
        if (getline(ss, value, ',')) {
             try {
                p.y = stod(value);
            } catch (const invalid_argument& e) {
                cerr << "Warning: Skipping invalid y-value '" << value << "' on line: " << line << endl;
                continue;
            }
        }

        points.push_back(p);
    }

    file.close();
    return points;
}


int main() {
    cout << "--- C++ Linear Regression Calculator ---" << endl;

    // read data points from the CSV file
    auto points = read_csv("data.csv");

    if (points.size() < 2) {
        cerr << "Error: Not enough data points to calculate a regression line. Need at least 2." << endl;
        return 1; // exit with an error code
    }

    // --- calculate the sums ---
    double sum_x = 0.0, sum_y = 0.0, sum_xy = 0.0, sum_x_squared = 0.0;
    int n = points.size();

    for (const auto& p : points) {
        sum_x += p.x;
        sum_y += p.y;
        sum_xy += p.x * p.y;
        sum_x_squared += p.x * p.x; // or pow(p.x, 2)
    }

    // --- Calculate slope (m) and y-intercept (b) ---
    // Using the formulas from Step 1, applying the standard regression equations
    
    double m_numerator = n * sum_xy - sum_x * sum_y;
    double m_denominator = n * sum_x_squared - sum_x * sum_x;

    // Check for division by zero to avoid errors
    if (m_denominator == 0) {
        cerr << "Error: Cannot calculate slope." << endl;
        return 1;
    }

    double m = m_numerator / m_denominator;
    double b = (sum_y - m * sum_x) / n;

    // --- Output the results ---
    cout << "Analyzed " << n << " data points." << endl;
    cout << "The best-fitting regression line is:" << endl;
    cout << "y = " << m << "x + " << b << endl;

    return 0; // Success
}
