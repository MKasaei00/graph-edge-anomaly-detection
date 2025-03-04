#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <queue>
#include <sstream>

#include "anoedgeglobal.hpp"
#include "hcms.hpp"
#include "utils.hpp"

using namespace std;

AnoedgeGlobal::AnoedgeGlobal(string _algorithm, string _dataset_name, int _rows, int _buckets, double _decay_factor) {
    algorithm = _algorithm;
    dataset_name = _dataset_name;
    rows = _rows;
    buckets = _buckets;
    decay_factor = _decay_factor;

    ReadUtils::loadEdgeData(src, dst, times, dataset_name);
    ReadUtils::loadEdgeLabelData(labels, dataset_name);
}

vector<double> AnoedgeGlobal::getScores() {

    bool enableSumLogging = false;
    bool enableQLogging = false;

    vector<double> scores;
    Hcms count(rows, buckets);

    vector<double> statsSum;
    vector<double> statsMin;
    vector<double> statsMax;
    vector<double> statsQ1;
    vector<double> statsQ2;
    vector<double> statsQ3;

    size_t num_records = src.size();
    int last_time = 0;

    double current_factor = 0.9;

    for (size_t i = 0; i < num_records; i++) {
        if (times[i] - last_time > 0) {
            count.decay(current_factor);
        }

        count.insert(src[i], dst[i], 1);

        double score = count.getAnoedgeglobalScore(algorithm, src[i], dst[i]);

        double final_score = score;
        scores.push_back(final_score);

        if (enableSumLogging) {
            double sum = count.getSum();

            statsSum.push_back(sum);
        }

        if (enableQLogging) {
            vector<double> boxes = count.getRangeValues();
            statsMin.push_back(boxes[0]);
            statsQ1.push_back(boxes[1]);
            statsQ2.push_back(boxes[2]);
            statsQ3.push_back(boxes[3]);
            statsMax.push_back(boxes[4]);
        }

        last_time = times[i];
    }

    if (enableSumLogging) {
        writeIntoFile(statsSum, "sum values.txt");
    }

    if(enableQLogging){
        writeIntoFile(statsMin, "min values.txt");
        writeIntoFile(statsQ1, "q1 values.txt");
        writeIntoFile(statsQ2, "q2 values.txt");
        writeIntoFile(statsQ3, "q3 values.txt");
        writeIntoFile(statsMax, "max values.txt");
    }

    return scores;
}

void AnoedgeGlobal::writeIntoFile(const vector<double>& values, const string& name) {
    std::cout << "Vector to file started => " << name << std::endl;

    std::ofstream outFile(name);
    if (!outFile) {
        std::cerr << "Error opening file for writing!" << std::endl;
        return;
    }

    // Use a large buffer to reduce I/O operations
    std::ostringstream buffer;
    for (const auto& value : values) {
        buffer << value << '\n';
    }
    outFile << buffer.str(); // Write everything in one go

    outFile.close();
    std::cout << "Vector written to file successfully! => " << name << std::endl;
}

void AnoedgeGlobal::run() {
    clock_t start_time = clock();
    vector<double> scores = getScores();
    double total_time = ((double) (clock() - start_time)) / CLOCKS_PER_SEC;

    string output_base_path = RESULT_BASE_PATH + algorithm + "_" + dataset_name;
    WriteUtils::writeScoresAndLabels(scores, labels, output_base_path + SCORE_FILE_SUFFIX);
    WriteUtils::writeTime(total_time, scores.size(), output_base_path + TIME_FILE_SUFFIX);
}

double AnoedgeGlobal::getAnoedgeglobalDensity(vector<vector<double>> &mat, int src, int dst) {
    size_t num_rows = mat.size();
    size_t num_cols = mat[0].size();

    bool row_flag[num_rows];
    bool col_flag[num_cols];

    double row_slice_sum[num_rows];
    double col_slice_sum[num_cols];

    for (size_t i = 0; i < num_rows; i++) {
        row_flag[i] = false;
        row_slice_sum[i] = mat[i][dst];
    }
    for (size_t i = 0; i < num_cols; i++) {
        col_flag[i] = false;
        col_slice_sum[i] = mat[src][i];
    }

    row_flag[src] = true;
    col_flag[dst] = true;
    row_slice_sum[src] = mat[src][dst];
    col_slice_sum[dst] = mat[src][dst];

    pair<int, double> max_row = {-1, -1.0};
    for (size_t i = 0; i < num_rows; i++) {
        if (!row_flag[i] && (row_slice_sum[i] >= max_row.second)) {
            max_row = {i, row_slice_sum[i]};
        }
    }

    pair<int, double> max_col = {-1, -1.0};
    for (size_t i = 0; i < num_cols; i++) {
        if (!col_flag[i] && (col_slice_sum[i] >= max_col.second)) {
            max_col = {i, col_slice_sum[i]};
        }
    }

    int marked_rows = 1;
    int marked_cols = 1;

    double cur_mat_sum = mat[src][dst];
    double output = cur_mat_sum / sqrt(marked_rows * marked_cols);

    size_t ctr = num_rows + num_cols - 2;
    while (ctr--) {
        if (max_row.second >= max_col.second) {
            row_flag[max_row.first] = true;
            marked_rows++;

            max_col = {-1, -1.0};
            for (size_t i = 0; i < num_cols; i++) {
                if (col_flag[i]) {
                    cur_mat_sum = cur_mat_sum + mat[max_row.first][i];
                } else {
                    col_slice_sum[i] = col_slice_sum[i] + mat[max_row.first][i];
                    if (col_slice_sum[i] >= max_col.second) {
                        max_col = {i, col_slice_sum[i]};
                    }
                }
            }

            max_row = {-1, -1.0};
            for (size_t i = 0; i < num_rows; i++) {
                if (!row_flag[i] && (row_slice_sum[i] >= max_row.second)) {
                    max_row = {i, row_slice_sum[i]};
                }
            }
        } else {
            col_flag[max_col.first] = true;
            marked_cols++;

            max_row = {-1, -1.0};
            for (size_t i = 0; i < num_rows; i++) {
                if (row_flag[i]) {
                    cur_mat_sum = cur_mat_sum + mat[i][max_col.first];
                } else {
                    row_slice_sum[i] = row_slice_sum[i] + mat[i][max_col.first];
                    if (row_slice_sum[i] >= max_row.second) {
                        max_row = {i, row_slice_sum[i]};
                    }
                }
            }

            max_col = {-1, -1.0};
            for (size_t i = 0; i < num_cols; i++) {
                if (!col_flag[i] && (col_slice_sum[i] >= max_col.second)) {
                    max_col = {i, col_slice_sum[i]};
                }
            }
        }
        output = MAX(output, cur_mat_sum / sqrt(marked_rows * marked_cols));
    }
    return output;
}