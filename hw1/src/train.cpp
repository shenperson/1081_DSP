#include "hmm.h"

#define TIME 50
#define DIM 6

using namespace std;

HMM hmm;
vector<vector<int>> Data;

void load_data(char const *seq_path);
void train(int iter);
void sub_train();

int main(int argc, char const *argv[]) {
  if (argc != 5) {
    cerr << "usage:\t./train <iter> <model_init_path> <seq_path> "
            "<output_model_path>\n";
    return 0;
  }
  int iter = atoi(argv[1]);
  const char *model_init_path = argv[2], *seq_path = argv[3],
             *output_model_path = argv[4];
  if (iter <= 0) cerr << "iter must be an integer\n";

  // load init model
  loadHMM(&hmm, model_init_path);

  // load training data
  load_data(seq_path);

  // train
  train(iter);

  // dump model
  FILE *fp = open_or_die(output_model_path, "w");
  dumpHMM(fp, &hmm);
}

void load_data(char const *seq_path) {
  ifstream ss(seq_path);
  string s;
  while (ss >> s) {
    vector<int> temp;
    temp.reserve(s.length());
    for (unsigned i = 0; i < s.length(); ++i) {
      temp.push_back(s[i] - 65);
    }
    Data.push_back(temp);
  }
  return;
}

// void dump_data(char const *output_model_path) {
//   ofstream ss(output_model_path);
//   ss << left << "initial: " << DIM << '\n';
//   for (int i = 0; i < DIM; ++i) ss << setw(16) << hmm.initial[i];
//   ss << "\n\ntransition: " << DIM << '\n';
//   for (int i = 0; i < DIM; ++i) {
//     for (int j = 0; j < DIM; ++j) ss << setw(16) << hmm.transition[i][j];
//     ss << '\n';
//   }
//   ss << "\nobservation: " << DIM << '\n';
//   for (int k = 0; k < DIM; ++k) {
//     for (int i = 0; i < DIM; ++i) ss << setw(16) << hmm.observation[k][i];
//     ss << '\n';
//   }
// }

void train(int iter) {
  for (int i = 0; i < iter; ++i) {
    sub_train();
  }
}
void sub_train() {
  double gamma_sum[DIM] = {0};
  double epsilon_sum[DIM][DIM] = {{0}};
  double init[DIM] = {0};
  double gamma_sum_k[DIM][DIM] = {{0}};

  for (const auto &seq : Data) {
    /* Forward algorithm */

    double alpha[TIME][DIM] = {{0}};
    // Init
    for (int d = 0; d < DIM; ++d) {
      alpha[0][d] = hmm.initial[d] * hmm.observation[seq[0]][d];
    }
    for (int t = 1; t < TIME; ++t) {
      // state d at time t
      for (int d = 0; d < DIM; ++d) {
        // alpha(t) = sum{ alpha(t-1) * a } * b(o)
        double sum = 0;
        for (int i = 0; i < DIM; ++i)
          sum += alpha[t - 1][i] * hmm.transition[i][d];
        alpha[t][d] = sum * hmm.observation[seq[t]][d];
      }
    }

    /* Backward algorithm */
    double beta[TIME][DIM] = {{0}};
    // Init
    for (int d = 0; d < DIM; ++d) {
      beta[TIME - 1][d] = 1;
    }
    for (int t = TIME - 2; t >= 0; --t) {
      for (int i = 0; i < DIM; ++i) {
        double sum = 0;
        // beta(t) = sum{ a * b(o+1) * beta(t+1) }
        for (int j = 0; j < DIM; ++j)
          sum += hmm.transition[i][j] * hmm.observation[seq[t + 1]][j] *
                 beta[t + 1][j];
        beta[t][i] = sum;
      }
    }

    /* Calculate gamma */
    double gamma[TIME][DIM] = {{0}};
    for (int t = 0; t < TIME; ++t) {
      double sum = 0;
      for (int d = 0; d < DIM; ++d) sum += alpha[t][d] * beta[t][d];
      for (int d = 0; d < DIM; ++d)
        gamma[t][d] = alpha[t][d] * beta[t][d] / sum;
    }

    /* Calculate epsilon */
    double epsilon[TIME - 1][DIM][DIM] = {{{0}}};
    for (int t = 0; t < TIME - 1; ++t) {
      double sum = 0;
      for (int i = 0; i < DIM; ++i) {
        for (int j = 0; j < DIM; ++j) {
          sum += alpha[t][i] * beta[t + 1][j] * hmm.transition[i][j] *
                 hmm.observation[seq[t + 1]][j];
        }
      }
      for (int i = 0; i < DIM; ++i) {
        for (int j = 0; j < DIM; ++j) {
          epsilon[t][i][j] = alpha[t][i] * beta[t + 1][j] *
                             hmm.transition[i][j] *
                             hmm.observation[seq[t + 1]][j] / sum;
        }
      }
    }

    /* Accumulate epsilon and gamma through all samples */
    for (int i = 0; i < DIM; ++i)
      for (int t = 0; t < TIME - 1; ++t) gamma_sum[i] += gamma[t][i];

    for (int i = 0; i < DIM; ++i)
      for (int j = 0; j < DIM; ++j)
        for (int t = 0; t < TIME - 1; ++t)
          epsilon_sum[i][j] += epsilon[t][i][j];

    for (int i = 0; i < DIM; ++i) init[i] += gamma[0][i];

    for (int i = 0; i < DIM; ++i)
      for (int t = 0; t < TIME - 1; ++t) gamma_sum_k[seq[t]][i] += gamma[t][i];
  }

  /* Update model */
  /* PI */
  // cout << left << "initial: " << DIM << '\n';
  for (int i = 0; i < DIM; ++i) {
    hmm.initial[i] = init[i] / Data.size();
    // cout << setw(16) << hmm.initial[i];
  }
  /* A */
  // cout << "\n\ntransition: " << DIM << '\n';
  for (int i = 0; i < DIM; ++i) {
    for (int j = 0; j < DIM; ++j) {
      hmm.transition[i][j] = epsilon_sum[i][j] / gamma_sum[i];
      // cout << setw(16) << hmm.transition[i][j];
    }
    // cout << '\n';
  }
  /* B */
  // cout << "\nobservation: " << DIM << '\n';
  for (int k = 0; k < DIM; ++k) {
    for (int i = 0; i < DIM; ++i) {
      hmm.observation[k][i] = gamma_sum_k[k][i] / gamma_sum[i];
      // cout << setw(16) << hmm.observation[k][i];
    }
    // cout << '\n';
  }
}
