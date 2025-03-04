#include <iostream>
#include <algorithm>
#include <limits>

#include "hcms.hpp"
#include "anoedgeglobal.hpp"
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
    for (const auto &row: count) {
        for (const auto &bucket: row) {
            for (double value: bucket) {
                values.push_back(value);
            }
        }
    }
    return values;
}

void Hcms::clear() {
    count = vector<vector<vector<double>>>(num_rows,
                                           vector<vector<double>>(num_buckets, vector<double>(num_buckets, 0.0)));
}

void Hcms::decay(double decay_factor, bool flag) {
    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_buckets; j++) {
            for (int k = 0; k < num_buckets; k++) {
                count[i][j][k] = count[i][j][k] * decay_factor;
            }
        }
    }
}


double Hcms::getAnoedgeglobalScore(string algorithm, int src, int dst) {
    double min_dsubgraph = numeric_limits<double>::max();
    for (int i = 0; i < num_rows; i++) {
        int src_bucket = hash(src, i);
        int dst_bucket = hash(dst, i);
        double cur_dsubgraph = AnoedgeGlobal::getAnoedgeglobalDensity(count[i], src_bucket, dst_bucket);
        min_dsubgraph = MIN(min_dsubgraph, cur_dsubgraph);
    }
    return min_dsubgraph;
}

double Hcms::getSum() {
    double sum = 0;
    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_buckets; ++j) {
            for (int k = 0; k < num_buckets; ++k) {
                double cur_value = count[i][j][k];
                sum += cur_value;
            }
        }
    }
    return sum / num_rows;
}


double Hcms::getValue(int src, int dst) {
    double min_value = numeric_limits<double>::max();
    for (int i = 0; i < num_rows; i++) {
        int src_bucket = hash(src, i);
        int dst_bucket = hash(dst, i);
        double cur_value = count[i][src_bucket][dst_bucket];
        min_value = MIN(min_value, cur_value);
    }
    return min_value;
}


void Hcms::decay(double decay_factor) {
    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_buckets; j++) {
            for (int k = 0; k < num_buckets; k++) {
                count[i][j][k] = count[i][j][k] * decay_factor;
            }
        }
    }
}


double Hcms::getCount(int a, int b) {
    double min_count = numeric_limits<double>::max();
    for (int i = 0; i < num_rows; i++) {
        int a_bucket = hash(a, i);
        int b_bucket = hash(b, i);
        min_count = MIN(min_count, count[i][a_bucket][b_bucket]);
    }
    return min_count;
}

