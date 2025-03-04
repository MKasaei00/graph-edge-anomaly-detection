#include <iostream>
#include <algorithm>
#include <limits>

#include "hcms.hpp"
#include "anoedgeglobal.hpp"
#include "anoedgelocal.hpp"
#include "utils.hpp"

using namespace std;

Hcms::Hcms(int r, int b) {
    num_rows = r;
    num_buckets = b;

    hash_a.resize(num_rows);
    hash_b.resize(num_rows);
    for (int i = 0; i < num_rows; i++) {
        hash_a[i] = rand() % (num_buckets - 1) + 1;
        hash_b[i] = rand() % num_buckets;
    }
    this->clear();
    sum = 0;
}

Hcms::Hcms(int r, int b, int d) {
    num_rows = r;
    num_buckets = b;
    num_dense_submatrices = d;

    hash_a.resize(num_rows);
    hash_b.resize(num_rows);
    for (int i = 0; i < num_rows; i++) {
        hash_a[i] = rand() % (num_buckets - 1) + 1;
        hash_b[i] = rand() % num_buckets;
    }
    this->clear();
    sum = 0;
}

int Hcms::hash(int elem, int i) {
    int resid = (elem * hash_a[i] + hash_b[i]) % num_buckets;
    return resid + (resid < 0 ? num_buckets : 0);
}

void Hcms::insert(int a, int b, double weight) {
    for (int i = 0; i < num_rows; i++) {
        int a_bucket = hash(a, i);
        int b_bucket = hash(b, i);
        count[i][a_bucket][b_bucket] += weight;
        sum += weight;
    }
}

double Hcms::getMin() {
    vector<double> values = getAllValues();
    return *min_element(values.begin(), values.end());
}

double Hcms::getMax() {
    vector<double> values = getAllValues();
    return *max_element(values.begin(), values.end());
}

double Hcms::getQ1() {
    vector<double> values = getAllValues();
    sort(values.begin(), values.end());
    return values[values.size() / 4];
}

double Hcms::getMedian() {
    vector<double> values = getAllValues();
    sort(values.begin(), values.end());
    return values[values.size() / 2];
}

double Hcms::getQ3() {
    vector<double> values = getAllValues();
    sort(values.begin(), values.end());
    return values[3 * values.size() / 4];
}

vector<double> Hcms::getAllValues() {
    vector<double> values;
    for (const auto& row : count) {
        for (const auto& bucket : row) {
            for (double value : bucket) {
                values.push_back(value);
            }
        }
    }
    return values;
}

void Hcms::clear() {
    count = vector<vector<vector<double>>>(num_rows, vector<vector<double>>(num_buckets, vector<double>(num_buckets, 0.0)));
}
