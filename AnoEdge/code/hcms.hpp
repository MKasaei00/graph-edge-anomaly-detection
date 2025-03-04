#ifndef hcms_hpp
#define hcms_hpp

#include <vector>

using namespace std;

class Hcms {
public:
    Hcms(int r, int b);

    Hcms(int r, int b, int d);

    void insert(int a, int b, double weight);

    void clear();

    void decay(double decay_factor);

    double getAnoedgeglobalScore(string algorithm, int src, int dst);

    double getValue(int src, int dst);

    double getSum();

    void intializeDenseSubmatrices();

    void decay(double decay_factor, bool flag);

    double getMin();

    double getQ1();

    double getMedian();

    double getQ3();

    double getMax();

private:
    int num_rows;
    int num_buckets;
    int num_dense_submatrices;
    double sum;

    vector<int> hash_a, hash_b;
    vector<vector<vector<double>>> count;

    int hash(int elem, int i);

    double getCount(int a, int b);
    vector<double> getAllValues();
};

#endif