#include <vector>
#include <utility>
#include <algorithm>
#include <numeric>
#include <random>
#include <iostream>
#include <fstream>
#include <cassert>
#include <chrono>

#include "disjoint_set.h"

// Code taken from
// https://github.com/google/benchmark/blob/02a354f3f323ae8256948e1dc77ddcb1dfc297da/include/benchmark/benchmark.h#L468
#if defined(__GNUC__) || defined(__clang__)
#define BENCHMARK_ALWAYS_INLINE __attribute__((always_inline))
#elif defined(_MSC_VER) && !defined(__clang__)
#define BENCHMARK_ALWAYS_INLINE __forceinline
#define __func__ __FUNCTION__
#else
#define BENCHMARK_ALWAYS_INLINE
#endif

template <class Tp>
inline BENCHMARK_ALWAYS_INLINE void DoNotOptimize(Tp& value) {
#if defined(__clang__)
      asm volatile("" : "+r,m"(value) : : "memory");
#else
        asm volatile("" : "+m,r"(value) : : "memory");
#endif
}

// Generator of unordered pairs of 2 elements without repetition
// from a pool of N elements.
//
// Method next() yield the current pair and advance the generator
// to the next state; method has_next() returns true if the next
// call to next() will yield a valid pair.
//
// The pairs are *not* random.
//
// Example:
//
// CombinationsK2Iter({0, 1, 2, 3}) yields 01, 02, 03, 12, 13, 23
//
// To generate a random unordered pairs, the elements of the pair
// must be reordered at random (ex from 01 to either 01 or 10)
// and the order of pairs shuffled.
//
// Note: the algorithm is a reimplementation in C++ of the one
// shown in Python's documentation
//
// Ref: https://docs.python.org/3/library/itertools.html#itertools.combinations
class CombinationsK2Iter {
    private:
        const std::vector<unsigned>& pool;
        std::pair<unsigned, unsigned> next_pair;

        std::vector<unsigned> indices;

    public:
        CombinationsK2Iter(const std::vector<unsigned>& pool) : pool(pool), indices(2) {
            indices[0] = 0;
            indices[1] = 1;

            next_pair = std::make_pair(pool[indices[0]], pool[indices[1]]);
        }

        std::pair<unsigned, unsigned> next() {
            const std::pair<unsigned, unsigned> ret = next_pair; // copy

            unsigned found = (unsigned)-1;
            for (int i = 1; i >= 0; --i) {
                if (indices[i] != i + pool.size() - 2) {
                    found = i;
                    break;
                }
            }

            if (found == (unsigned)-1) {
                next_pair = std::make_pair(0, 0); // signal the end of the stream
            } else {
                indices[found] += 1;
                if (found == 0) {
                    indices[found + 1] = indices[found] + 1;
                }

                next_pair = std::make_pair(pool[indices[0]], pool[indices[1]]);
            }

            return ret;
        }

        bool has_next() const {
            return not (next_pair.first == 0 and next_pair.second == 0);
        }
};

class ExperimentResults {
    private:
        UnionStrategy U;
        CompressStrategy C;

        const unsigned seed;
        const unsigned elems_n;
        const std::vector<std::pair<unsigned, unsigned>>& pairs;

        std::vector<std::chrono::duration<double>> min_elapseds;

        std::ofstream out;
        bool is_experiment_active;

        const unsigned Rounds;

    public:
        ExperimentResults(unsigned Rounds, unsigned seed, unsigned elems_n, const std::vector<std::pair<unsigned, unsigned>>& pairs) : seed(seed), elems_n(elems_n), pairs(pairs), is_experiment_active(false), Rounds(Rounds) {}

        void initialize_experiment(UnionStrategy U, CompressStrategy C) {
            assert(not is_experiment_active);

            this->U = U;
            this->C = C;

            this->min_elapseds.clear();

            out.open("results.csv", std::ios_base::out | std::ios_base::app);
            is_experiment_active = true;
        }

        void finalize_experiment() {
            assert(is_experiment_active);
            out.close();
            is_experiment_active = false;
        }

        template<UnionStrategy U, CompressStrategy C>
        void do_measurement(unsigned round, unsigned measurement_i, unsigned& do_measure_mask, unsigned union_n, const std::chrono::duration<double>& elapsed, const DisjointSets<U, C>& sets) {
            assert(is_experiment_active);
            if (round == 0) {
                min_elapseds.push_back(elapsed);
            } else {
                auto min = min_elapseds[measurement_i];
                if (min > elapsed) {
                    min_elapseds[measurement_i] = elapsed;
                }
            }

            if (round == Rounds-1) {
                unsigned total_path_length = sets.total_path_length();
                unsigned total_pointer_updates = sets.total_pointer_updates();
                unsigned disjoint_set_cnt = sets.count_disjoint_sets();
                unsigned root_children_cnt = sets.count_root_children();
                double min_elapsed = min_elapseds[measurement_i].count();

                std::cout << "Measurement "<< measurement_i << ", union count so far: " << union_n << "\n";

                // Write into the results.csv file.
                //
                // Format:
                //   UnionStrategy CompressStrategy Seed ElemsN UnionN TPL TPU RootChildrenCount DisjointSetCnt Elapsed
                //
                out << union_strategy_name(U) << "," << compress_strategy_name(C) << ",";
                out << seed << "," << elems_n << "," << union_n << ",";
                out << total_path_length << "," << total_pointer_updates << ",";
                out << root_children_cnt << "," << disjoint_set_cnt << "," << min_elapsed << "\n";

                do_measure_mask <<= 1;
                do_measure_mask += 1;
            }
        }

        template<UnionStrategy U, CompressStrategy C>
        void run() {
            assert(is_experiment_active);

            std::cout << "Executing experiment:\n";
            std::cout << "- union strategy: " << ExperimentResults::union_strategy_name(U) << "\n";
            std::cout << "- compress strategy: " << ExperimentResults::compress_strategy_name(C) << "\n";

            std::vector<std::chrono::duration<double>> min_elapseds;

            for (unsigned round = 0; round < Rounds; ++round) {
                DisjointSets<U, C> sets(elems_n);

                std::chrono::duration<double> elapsed;

                // 4 bits set -> first measure at union_n = 16
                unsigned union_n = 0;
                unsigned do_measure_mask = 0x0000000f;

                unsigned measurement_i = 0;
                for (unsigned i = 0; i < pairs.size(); ++i) {
                    auto p = pairs[i];

                    auto begin = std::chrono::steady_clock::now();
                    DoNotOptimize(p);
                    auto o = sets.Union(p.first, p.second);
                    DoNotOptimize(o);
                    elapsed += std::chrono::steady_clock::now() - begin;

                    ++union_n;

                    if ((union_n & do_measure_mask) == 0) {
                        do_measurement(round, measurement_i, do_measure_mask, union_n, elapsed, sets);
                        ++measurement_i;
                    }
                }

                if (round == Rounds-1) {
                    do_measurement(round, measurement_i, do_measure_mask, union_n, elapsed, sets);
                } else {
                    std::cout << "." << std::flush;
                }
            }

            std::cout << "Experiment done.\n";
        }

    template<CompressStrategy C>
    void run_all_union_experiments() {
        initialize_experiment(LeftIntoRight, C);
        run<LeftIntoRight, C>();
        finalize_experiment();

        initialize_experiment(ByWeight, C);
        run<ByWeight, C>();
        finalize_experiment();

        initialize_experiment(ByRank, C);
        run<ByRank, C>();
        finalize_experiment();
    }

    private:
        static const char* union_strategy_name(UnionStrategy U) {
            switch (U) {
                case LeftIntoRight:
                    return "QU";
                    break;
                case ByWeight:
                    return "UW";
                    break;
                case ByRank:
                    return "UR";
                    break;
                default:
                    assert(false);
            }
        }

        static const char* compress_strategy_name(CompressStrategy C) {
            switch (C) {
                case None:
                    return "NC";
                    break;
                case Full:
                    return "FC";
                    break;
                case Splitting:
                    return "PS";
                    break;
                case Halving:
                    return "PH";
                    break;
                case ReversalK2:
                    return "R2";
                    break;
                default:
                    assert(false);
            }
        }
};

#define ROUNDS 35

// Possible seeds to initialize the Ma
//
// The numbers are the first digits of the PI number
//
// References:
// http://www.math.com/tables/constants/pi.htm
static const unsigned SEEDS[] = {314159, 26535, 89793, 23846, 26433, 83279, 5028, 841971, 69399, 37510, 58209, 74944, 59230, 7816, 406286, 20899, 862803, 4825, 34211, 70679};

static const unsigned ELEM_N_S[] = {1 << 10, 1 << 12, 1 << 13};

int main(int argc, char* argv[]) {

    for (unsigned elems_n_i = 0; elems_n_i < sizeof(ELEM_N_S)/sizeof(ELEM_N_S[0]); ++elems_n_i) {
        const unsigned elems_n = ELEM_N_S[elems_n_i];

        std::vector<std::pair<unsigned, unsigned>> pairs;

        std::cout << "Generating 2-of-n pairs (with n=" << elems_n << ")...\n";
        {
            std::vector<unsigned> vec(elems_n);
            std::iota(vec.begin(), vec.end(), 0);

            CombinationsK2Iter it(vec);

            while (it.has_next()) {
                pairs.push_back(it.next());
            }

            std::cout << "Generated " << pairs.size() << " pairs\n";
        }

        for (unsigned seed_i = 0; seed_i < sizeof(SEEDS)/sizeof(SEEDS[0]); ++seed_i) {
            const unsigned seed = SEEDS[seed_i];

            std::cout << "Shuffling pairs (seed=" << seed << ")...\n";
            {
                auto randgen = std::mt19937(seed);

                std::shuffle(pairs.begin(), pairs.end(), randgen);

                unsigned swap_bit = 0;
                for (unsigned i = 0; i < pairs.size(); ++i) {
                    if (i % 32 == 0) {
                        swap_bit = randgen();
                    }

                    if ((swap_bit & 1) == 1) {
                        unsigned tmp = pairs[i].first;
                        pairs[i].first = pairs[i].second;
                        pairs[i].second = tmp;
                    }

                    swap_bit >>= 1;
                }
            }
            std::cout << "Shuffle done\n";

            std::cout << "Take a cup of coffee, we begin running the experiments...\n";
            ExperimentResults exp(ROUNDS, seed, elems_n, pairs);

            exp.run_all_union_experiments<None>();
            exp.run_all_union_experiments<Full>();
            exp.run_all_union_experiments<Splitting>();
            exp.run_all_union_experiments<Halving>();
            exp.run_all_union_experiments<ReversalK2>();
        }
    }

    return 0;
}
