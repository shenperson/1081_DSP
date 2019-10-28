#include "hmm.h"

#define TIME 50
#define DIM 6
#define NUM 5
#define MAX(A, B) ((A >= B) ? A : B)

using namespace std;

HMM hmm[NUM];
int main(int argc, char const* argv[]) {
  if (argc != 4) {
    cerr << "./test <models_list_path> <seq_path> <output_result_path>\n";
    return 0;
  }
  const char *models_list_path = argv[1], *seq_path = argv[2],
             *output_result_path = argv[3];
  load_models(models_list_path, hmm, NUM);
  ifstream ss(seq_path);
  ofstream output(output_result_path);
  string s;
  while (ss >> s) {
    vector<int> seq;
    seq.reserve(s.length());
    for (unsigned i = 0; i < s.length(); ++i) {
      seq.push_back(s[i] - 65);
    }
    double max = 0;
    int max_idx = 0;
    for (int n = 0; n < NUM; ++n) {
      double delta[TIME][DIM] = {{0}};
      double prob = 0;

      /* Viterbi Algorithm */
      // initialize
      double temp_max = 0;  // max prob of hmm[n]
      for (int i = 0; i < DIM; ++i) {
        delta[0][i] = hmm[n].initial[i] * hmm[n].observation[seq[0]][i];
      }
      for (int t = 1; t < TIME; ++t) {
        for (int j = 0; j < DIM; ++j) {
          for (int i = 0; i < DIM; ++i) {
            delta[t][j] =
                MAX(delta[t][j], delta[t - 1][i] * hmm[n].transition[i][j]);
          }
          delta[t][j] *= hmm[n].observation[seq[t]][j];
        }
      }
      for (int i = 0; i < NUM; ++i) {
        temp_max =
            (temp_max >= delta[TIME - 1][i]) ? temp_max : delta[TIME - 1][i];
      }
      if (temp_max > max) {
        max_idx = n;
        max = temp_max;
      }
    }
    // model_01.txt 7.822367e-34
    output << "model_0" << (max_idx + 1) << ".txt " << max << '\n';
  }
}
