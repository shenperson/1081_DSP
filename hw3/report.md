# HW3 ZhuYin Decoding
### 電機三 沈哲瑋 B06901090
---

## what I observed
### disambig vs. MyDisambig
* 10筆測資執行結果 : 都一樣
* 執行10筆測資所需時間 :
    * disambig
        ```
        real    0m41.486s
        user    0m41.255s
        sys     0m0.199s
        ```
    * MyDisambig
        ```
        real    0m49.864s
        user    0m49.732s
        sys     0m0.124s
        ```
    * 用 `time` 來測的，但因為沒有工作站能用，只好用自己電腦來測，盡量在相同條件下測了，不過還是會有些不準。就結果而言我自己的執行時間差了點，不過兩者的時間複雜度應該是一樣的，也許是因為我用了不少 vector 來實作導致時間花的較多。

## what I have done
* mapping:  
用 python 實作，主要運用到 list，也就是 python 內的 map。把每行的注音跟文字本身當作 key，原本的中文當作 value，執行一遍就可以得到我們要的 mapping。
* Mydisambig:
    * I/O:
    因為這次讀入/寫出的檔案都用空格來分隔，所以用了 fstream 與 sstream 以方便操作。
    * Node :
    我創了一個物件 Node，主要表示候選的字，除了字本身也存了 bigram 的機率，以及在做 viterbi 時會用到的前一個 Node 是在第幾個。
    * Viterbi - init:
    因為 `lm.wordProb` 回傳的機率是取過 log 的，因此在這次的操作中都是用相加而非相乘，而機率大多在 -4 ~ -6 之間，因此一開始我都給 -1e10 以免累加太多次後超過。此外，在開頭及結尾分別加上 `<s>` 和 `</s>`。
    * Viterbi - cal_prob:
    前一個字的機率，再加上前一個字與自己的 bigram 機率，記錄下在所有候選字中的最大值以及在第幾個。
    * Viterbi - back_track:
    因為最後一個必是 `</s>`，因此只要從 `</s>` 照著原本就記下的前一個位置，一路往回推到第一個為止，就是我們做出來結果，也就完成了 viterbi。
