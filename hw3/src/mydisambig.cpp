// #include "File.h"
// #include "Vocab.h"
#include "Ngram.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#define ORDER 2  // 2-gram

using namespace std;

/*  global variables  */
unordered_map<string, vector<string>> zy2big5;  // Zhuyin to Big5
Vocab voc;
Ngram lm(voc, ORDER);

/*  class definition  */
struct Node {
    string s;
    double prob;
    int idx;
    Node(string _s) : s(_s), prob(-100), idx(0) {}
    Node(string _s, double _prob) : s(_s), prob(_prob), idx(0) {}
};

/*  function prototype  */
double prob_of_bigram(const char *a, const char *b);
vector<string> viterbi(string &line);

/*  main function  */
int main(int argc, char *argv[]) {
    if (argc != 5) {
        cout << "usage:\n"
             << "    ./mydisambig $1 $2 $3 $4\n"
             << "\t$1 segemented file to be decoded\n"
             << "\t$2 ZhuYin-Big5 mapping\n"
             << "\t$3 language model\n"
             << "\t$4 output file\n";
        exit(EXIT_FAILURE);
    }

    // read ZhuYin-Big5 mapping
    ifstream mapping(argv[2]);
    string zy, big5, line;
    stringstream ss;
    while (getline(mapping, line)) {
        // ex: £t ¼· ´¬ ÍI ¦B
        ss << line;
        ss >> zy;
        while (ss >> big5) {
            zy2big5[zy].push_back(big5);
        }
        ss.str("");
        ss.clear();
    }

    // read language model
    File f(argv[3], "r");
    lm.read(f);
    f.close();

    // read testing data & do Viterbi algorithm & output
    ifstream testing_data(argv[1]);
    ofstream out(argv[4]);
    while (getline(testing_data, line)) {
        vector<string> output = viterbi(line);
        for (int i = output.size() - 1; i > 0; --i) {
            out << output[i] << ' ';
        }
        out << output[0] << "\n";
    }
}
/*  detailed function definition  */

double prob_of_bigram(const char *a, const char *b) {
    VocabIndex wid1 = voc.getIndex(a);
    VocabIndex wid2 = voc.getIndex(b);
    // oov
    if (wid1 == Vocab_None) wid1 = voc.getIndex(Vocab_Unknown);
    if (wid2 == Vocab_None) wid2 = voc.getIndex(Vocab_Unknown);

    VocabIndex context[] = {wid1, Vocab_None};
    return lm.wordProb(wid2, context);
}

vector<string> viterbi(string &line) {
    // ex: £~ µø ££ »D ¶} £¥

    vector<vector<Node>> nodes;
    vector<Node> word_str;

    // build node table first
    word_str.push_back(Node("<s>", 1.0));  // bos
    nodes.push_back(word_str);
    word_str.clear();

    stringstream ss(line);
    string zy;
    while (ss >> zy) {
        for (const auto &i : zy2big5[zy]) {
            word_str.push_back(Node(i));
        }
        nodes.push_back(word_str);
        word_str.clear();
    }
    word_str.push_back(Node("</s>"));  // eos
    nodes.push_back(word_str);
    word_str.clear();

    // assign probability
    for (int i = 1; i < nodes.size(); ++i) {
        for (int j = 0; j < nodes[i].size(); ++j) {
            for (int k = 0; k < nodes[i - 1].size(); ++k) {
                double prob = nodes[i - 1][k].prob + prob_of_bigram(nodes[i - 1][k].s.c_str(), nodes[i][j].s.c_str());
                if (prob > nodes[i][j].prob) {
                    nodes[i][j].prob = prob;
                    nodes[i][j].idx = k;
                }
            }
        }
    }
    vector<Node> tmp = nodes[nodes.size() - 2];
    for (int k = 0; k < tmp.size(); ++k) {
        double prob = tmp[k].prob;
        if (prob > nodes.back()[0].prob) {
            nodes.back()[0].prob = prob;
            nodes.back()[0].idx = k;
        }
    }
    vector<string> output;
    output.reserve(nodes.size());
    Node temp = nodes.back()[0];
    output.push_back(temp.s);
    for (int i = nodes.size() - 1; i > 0; --i) {
        temp = nodes[i - 1][temp.idx];
        output.push_back(temp.s);
    }

    return output;
}