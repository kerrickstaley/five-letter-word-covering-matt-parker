#include <string>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int popcount(int x) {
    int ret = 0;
    while (x != 0) {
        ret += 1;
        x &= x - 1;
    }
    return ret;
}

int to_bits(const string& word) {
    int ret = 0;
    for (const auto& c : word) {
        ret |= (1 << (c - 'a'));
    }
    return ret;
}

vector<string> read_words() {
    std::ifstream inf("words.txt");
    std::string word;
    vector<int> words_seen(1 << 26);
    vector<string> ret;
    while (!inf.eof()) {
        inf >> word;
        int word_bits = to_bits(word);
        if (word.size() == 5 && popcount(word_bits) == 5 && !words_seen[word_bits]) {
            ret.push_back(word);
            words_seen[word_bits] = true;
        }
    }
    return ret;
}

int main() {
    const auto word_strs = read_words();
    vector<int> words;
    for (const auto& word_str : word_strs) {
        words.push_back(to_bits(word_str));
    }

    vector<bool> two_words(1 << 26);
    vector<bool> two_words_small(1 << 13);
    for (int i = 0; i < words.size(); i++) {
        for (int j = i + 1; j < words.size(); j++) {
            if (words[i] & words[j]) {
                continue;
            }

            int comb = words[i] | words[j];
            for (int k = 0; k < 26; k++) {
                if (comb & (1 << k)) {
                    continue;
                }
                int comb_plus = comb | (1 << k);
                two_words[comb_plus] = true;
                two_words_small[comb_plus >> 13 | comb_plus & (1 << 13) - 1] = true;
            }
        }
    }

    for (int i = 0; i < words.size(); i++) {
        for (int j = i + 1; j < words.size(); j++) {
            if (words[i] & words[j]) {
                continue;
            }
            for (int k = j + 1; k < words.size(); k++) {
                if (words[i] & words[k] || words[j] & words[k]) {
                    continue;
                }
                int comb = words[i] | words[j] | words[k];
                int rest = ((1 << 26) - 1) - comb;
                if (two_words_small[rest >> 13 | rest & (1 << 13) - 1] && two_words[rest]) {
                    for (int l = k + 1; l < words.size(); l++) {
                        if (words[l] & ~rest) {
                            continue;
                        }
                        for (int m = l + 1; m < words.size(); m++) {
                            if (words[m] & words[l] || words[m] & ~rest) {
                                continue;
                            }
                            cout << word_strs[i] << ' ' << word_strs[j] << ' ' << word_strs[k] << ' ' << word_strs[l] << ' ' << word_strs[m] << '\n';
                        }
                    }
                }
            }
        }
    }
}
