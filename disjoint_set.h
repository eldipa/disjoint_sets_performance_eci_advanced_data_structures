#include <iostream>

enum CompressStrategy {
    None,
    Full,
    Splitting,
    Halving,
    ReversalK2,
    _LastCompressStrategy
};

enum UnionStrategy {
    LeftIntoRight,
    ByWeight,
    ByRank,
    _LastUnionStrategy
};

template<UnionStrategy U, CompressStrategy C>
class DisjointSets {
    private:
        struct Set {
            unsigned parent;
            unsigned weight;
            unsigned rank;
        };

        Set* sets;
        unsigned vector_size;

    public:

        DisjointSets(unsigned n) {
            sets = new Set[n];
            for (unsigned i = 0; i < n; ++i) {
                sets[i] = {i, 1, 0};
            }

            vector_size = n;
        }

        ~DisjointSets() {
            delete sets;
        }

        DisjointSets& operator=(const DisjointSets&) = delete;
        DisjointSets(const DisjointSets&) = delete;

        unsigned Find(unsigned i) {
            if constexpr (C == None) {
                return find_without_compress(i);
            } else if constexpr (C == Full) {
                return find_and_full_compress(i);
            } else if constexpr (C == Splitting) {
                return find_and_compress_splitting(i);
            } else if constexpr (C == Halving) {
                return find_and_compress_halving(i);
            } else if constexpr (C == ReversalK2) {
                return find_and_compress_k2_reversal(i);
            }
        }

        unsigned Union(unsigned i, unsigned j) {
            i = Find(i);
            j = Find(j);

            if (i != j) {
                // By default, the set i is linked/merged into set j
                //
                // Swap sets i by j if the set i is actually "too big"
                // The definition of "too big" is based on the strategy
                if constexpr (U == ByWeight) {
                    if (sets[i].weight > sets[j].weight) {
                        std::swap(i, j);
                    }
                } else if constexpr (U == ByRank) {
                    if (sets[i].rank > sets[j].rank) {
                        std::swap(i, j);
                    }
                } else if constexpr (U == LeftIntoRight) {
                    // Do nothing.
                }

                // Merge set i into set j
                sets[i].parent = j;

                // Update the weight/rank based on the strategy
                if constexpr (U == ByWeight) {
                    sets[j].weight += sets[i].weight;
                }

                if constexpr (U == ByRank) {
                    if (sets[i].rank == sets[j].rank)
                        ++sets[j].rank;
                }
            }

            return j;
        }

        unsigned total_path_length() const {
            unsigned l = 0;
            for (unsigned i = 0; i < vector_size; ++i) {
                l += path_length(i);
            }

            return l;
        }

        unsigned count_disjoint_sets() const {
            unsigned cnt = 0;
            for (unsigned i = 0; i < vector_size; ++i) {
                cnt += is_root(i);
            }

            return cnt;
        }

        unsigned count_root_children() const {
            unsigned cnt = 0;
            for (unsigned i = 0; i < vector_size; ++i) {
                cnt += is_root_child(i) and not is_root(i);
            }

            return cnt;
        }

        unsigned total_pointer_updates() const {
            if constexpr (C == None) {
                return 0;
            } else {
                unsigned tpu = total_path_length() - count_root_children();
                if constexpr (C == Halving) {
                    return tpu / 2;
                }

                return tpu;
            }
        }

        void as_dot_graph(std::ostream& out) const {
            out << "digraph {\n  rankdir=\"BT\";\n";
            for (unsigned i = 0; i < vector_size; ++i) {
                out << "  x" << i << " -> x" << sets[i].parent << ";\n";
            }
            out << "}\n";
        }

    private:
        unsigned find_without_compress(unsigned i) {
            while (not is_root(i)) {
                i = sets[i].parent;
            }

            return i;
        }

        unsigned find_and_full_compress(unsigned i) {
            unsigned root = find_without_compress(i);
            unsigned tmp;

            while (root != i) {
                tmp = sets[i].parent;
                sets[i].parent = root;
                i = tmp;
            }

            return i;
        }

        unsigned find_and_compress_splitting(unsigned i) {
            unsigned child = i;
            unsigned parent = sets[i].parent;

            while (not is_root(parent)) {
                unsigned grandparent = sets[parent].parent;

                sets[child].parent = grandparent;
                child = parent;
                parent = grandparent;
            }

            return parent;
        }

        unsigned find_and_compress_halving(unsigned i) {
            unsigned child = i;
            unsigned parent = sets[i].parent;

            unsigned iter = 0;
            while (not is_root(parent)) {
                unsigned grandparent = sets[parent].parent;

                sets[child].parent = (iter % 2 == 0) ? grandparent : parent;
                child = parent;
                parent = grandparent;
                ++iter;
            }

            return parent;
        }

        unsigned find_and_compress_k2_reversal(unsigned i) {
            unsigned start = i;
            unsigned tmp;

            i = sets[i].parent;
            while (not is_root_child(i)) {
                tmp = sets[i].parent;
                sets[i].parent = start;
                i = tmp;
            }

            // Until this point, node i is a child of the
            // root, we do *not* update its parent
            unsigned root = sets[i].parent;

            // Update start's parent to point the root
            sets[start].parent = root;

            return root;
        }


        bool is_root(unsigned i) const {
            return sets[i].parent == i;
        }

        bool is_root_child(unsigned i) const {
            return is_root(sets[i].parent);
        }

        unsigned path_length(unsigned i) const {
            unsigned l = 0;
            while (not is_root(i)) {
                i = sets[i].parent;
                ++l;
            }

            return l;
        }
};
