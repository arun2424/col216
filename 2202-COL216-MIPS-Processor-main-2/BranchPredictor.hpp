#ifndef __BRANCH_PREDICTOR_HPP__
#define __BRANCH_PREDICTOR_HPP__

#include <vector>
#include <bitset>

struct BranchPredictor {
    virtual bool predict(uint32_t pc) = 0;
    virtual void update(uint32_t pc, bool taken) = 0;
};

struct SaturatingBranchPredictor : public BranchPredictor {
    std::vector<std::bitset<2>> table;
    SaturatingBranchPredictor(int value) : table(1 << 14, value) {}

    bool predict(uint32_t pc) {
        std::bitset<2> counter = table[pc % table.size()];
        return counter[1] || counter[0];
    }

    void update(uint32_t pc, bool taken) {
        // your code here
         std::bitset<2>& counter = table[pc % table.size()];
        if (taken) {
            if (counter == 0) {
                counter = 1;
            } else if (counter == 1) {
                counter = 3;
            }
        } else {
            if (counter == 3) {
                counter = 2;
            } else if (counter == 2) {
                counter = 0;
            }
        }
    }
};

struct BHRBranchPredictor : public BranchPredictor {
    std::vector<std::bitset<2>> bhr;
    BHRBranchPredictor(int value) : bhr(1 << 2, value) {}

    bool predict(uint32_t pc) {
        uint32_t index = (pc >> 2) & 0b11;
        std::bitset<2> counter = bhr[index];
        return counter[1] || counter[0];
    }

    void update(uint32_t pc, bool taken) {
        // your code here
        uint32_t index = (pc >> 2) & 0b11;
        std::bitset<2>& counter = bhr[index];
        counter <<= 1;
        counter[0] = taken;
        if (taken) {
            if (counter == 0) {
                counter = 1;
            } else if (counter == 1) {
                counter = 3;
            }
        } else {
            if (counter == 3) {
                counter = 2;
            } else if (counter == 2) {
                counter = 0;
            }
        }
    }
};

struct SaturatingBHRBranchPredictor : public BranchPredictor {
    std::vector<std::bitset<2>> bhr;
    std::vector<std::bitset<2>> table;
    std::vector<std::bitset<2>> combination;
    SaturatingBHRBranchPredictor(int value, int size) : bhr(1 << 2, value), table(1 << 14, value), combination(size, value) {
        assert(size <= (1 << 16));
    }

    bool predict(uint32_t pc) {
       uint32_t bhr_index = (pc >> 2) & 0b11;
        std::bitset<2> bhr_counter = bhr[bhr_index];
        uint32_t table_index = (pc ^ (bhr_counter.to_ulong() << 14)) % table.size();
        std::bitset<2> table_counter = table[table_index];
        std::bitset<2> combined_counter = combination[table_counter.to_ulong() + bhr_counter.to_ulong()];
        return combined_counter[1] || combined_counter[0];
    }

    void update(uint32_t pc, bool taken) {
       uint32_t bhr_index = (pc >> 2) & 0b11;
        std::bitset<2> bhr_counter = bhr[bhr_index];
        uint32_t table_index = (pc ^ (bhr_counter.to_ulong() << 14)) % table.size();
        std::bitset<2> table_counter = table[table_index];
        std::bitset<2> combined_counter = combination[table_counter.to_ulong() + bhr_counter.to_ulong()];

        if (taken) {
            if (combined_counter.to_ulong() < 3) {
                combined_counter = combined_counter.to_ulong() + 1;
            }
        } else {
            if (combined_counter.to_ulong() > 0) {
                combined_counter = combined_counter.to_ulong() - 1;
            }
        }

        combination[table_counter.to_ulong() + bhr_counter.to_ulong()] = combined_counter;
        table[table_index] = combined_counter[1] ? std::bitset<2>(3) : std::bitset<2>(0);
        bhr[bhr_index] = (bhr_counter.to_ulong() << 1 | taken) & std::bitset<2>(3);
    }
};

#endif
