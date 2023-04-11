#include <fstream>
#include <iostream>
#include "branch_predictor.hpp"

int main() {
    // Read the trace file
    std::ifstream trace_file("trace.txt");
    if (!trace_file.is_open()) {
        std::cerr << "Error: Could not open trace file.\n";
        return 1;
    }

    // Initialize the branch predictors with different initial values
    SaturatingBranchPredictor sbp00(0);
    SaturatingBranchPredictor sbp01(1);
    SaturatingBranchPredictor sbp10(2);
    SaturatingBranchPredictor sbp11(3);

    BHRBranchPredictor bbp00(0);
    BHRBranchPredictor bbp01(1);
    BHRBranchPredictor bbp10(2);
    BHRBranchPredictor bbp11(3);

    SaturatingBHRBranchPredictor sbbp00(0, 1 << 10);
    SaturatingBHRBranchPredictor sbbp01(1, 1 << 10);
    SaturatingBHRBranchPredictor sbbp10(2, 1 << 10);
    SaturatingBHRBranchPredictor sbbp11(3, 1 << 10);

    // Counters for calculating accuracy
    int total_predictions = 0;
    int correct_predictions_sbp00 = 0, correct_predictions_sbp01 = 0, correct_predictions_sbp10 = 0, correct_predictions_sbp11 = 0;
    int correct_predictions_bbp00 = 0, correct_predictions_bbp01 = 0, correct_predictions_bbp10 = 0, correct_predictions_bbp11 = 0;
    int correct_predictions_sbbp00 = 0, correct_predictions_sbbp01 = 0, correct_predictions_sbbp10 = 0, correct_predictions_sbbp11 = 0;

    // Process the trace file
    uint32_t pc;
    bool outcome;
    while (trace_file >> std::hex >> pc >> outcome) {
        // Saturating Branch Predictor
        correct_predictions_sbp00 += sbp00.predict(pc) == outcome;
        correct_predictions_sbp01 += sbp01.predict(pc) == outcome;
        correct_predictions_sbp10 += sbp10.predict(pc) == outcome;
        correct_predictions_sbp11 += sbp11.predict(pc) == outcome;

        sbp00.update(pc, outcome);
        sbp01.update(pc, outcome);
        sbp10.update(pc, outcome);
        sbp11.update(pc, outcome);

        // BHR Branch Predictor
        correct_predictions_bbp00 += bbp00.predict(pc) == outcome;
        correct_predictions_bbp01 += bbp01.predict(pc) == outcome;
        correct_predictions_bbp10 += bbp10.predict(pc) == outcome;
        correct_predictions_bbp11 += bbp11.predict(pc) == outcome;

        bbp00.update(pc, outcome);
        bbp01.update(pc, outcome);
        bbp10.update(pc, outcome);
        bbp11.update(pc, outcome);

        // Saturating BHR Branch Predictor
        correct_predictions_sbbp00 += sbbp00.predict(pc) == outcome;
        correct_predictions_sbbp01 += sbbp01.predict(pc) == outcome;
        correct_predictions_sbbp10 += sbbp10.predict(pc) == outcome;
        correct_predictions_sbbp11 += sbbp11.predict(pc) == outcome;
        
        sbbp00.update(pc, outcome);
        sbbp01.update(pc, outcome);
        sbbp10.update(pc, outcome);
        sbbp11.update(pc, outcome);

        total_predictions++;
}

// Print the accuracy for each predictor
std::cout << "Saturating Branch Predictor\n";
std::cout << "Initial value 00: " << (double)correct_predictions_sbp00 / total_predictions << '\n';
std::cout << "Initial value 01: " << (double)correct_predictions_sbp01 / total_predictions << '\n';
std::cout << "Initial value 10: " << (double)correct_predictions_sbp10 / total_predictions << '\n';
std::cout << "Initial value 11: " << (double)correct_predictions_sbp11 / total_predictions << '\n';

std::cout << "BHR Branch Predictor\n";
std::cout << "Initial value 00: " << (double)correct_predictions_bbp00 / total_predictions << '\n';
std::cout << "Initial value 01: " << (double)correct_predictions_bbp01 / total_predictions << '\n';
std::cout << "Initial value 10: " << (double)correct_predictions_bbp10 / total_predictions << '\n';
std::cout << "Initial value 11: " << (double)correct_predictions_bbp11 / total_predictions << '\n';

std::cout << "Saturating BHR Branch Predictor\n";
std::cout << "Initial value 00: " << (double)correct_predictions_sbbp00 / total_predictions << '\n';
std::cout << "Initial value 01: " << (double)correct_predictions_sbbp01 / total_predictions << '\n';
std::cout << "Initial value 10: " << (double)correct_predictions_sbbp10 / total_predictions << '\n';
std::cout << "Initial value 11: " << (double)correct_predictions_sbbp11 / total_predictions << '\n';

return 0;

