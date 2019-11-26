# HW2_1 Kaldi Speech Recognition Toolkit
### 電機三 沈哲瑋 B06901090
---
## Result:
* 最後選定的參數如下：
    * train:
        ```
        numiters=20
        maxiterinc=10
        numgauss=900
        totgauss=1500
        incgauss=$[($totgauss-$numgauss)/$maxiterinc]
        realign_iters="1 2 3 4 6 7 9 10 12 13 15 16 18 19 20";
        scale_opts="--transition-scale=1.0 --acoustic-scale=0.1 --self-loop-scale=0.1"
        ```
    * test:
        ```
        opt_acwt=0.195
        test_beam=25.0
        ```
    * acc:
        `accuracy -> [ 96.83 ] %`

## 觀察與發現：
1. iter: 
    iter 的大小會跟 acc 有很大關係，但是當 iter 越大，時間也會花越久，因此必須做出取捨，也可以適當的調整 realign_iters 讓時間壓低一點。
2. gauss:
    Gaussian 的數量也影響很大，自己實測是把 Gaussian 拉到 1000 上下時，得出的結果會最好，再更大反而 acc 下降，且所需的時間也拉長。
3. scale_opts:
    這個我自己嘗試改了幾次後，得出的結果都會變得更不好，因此維持原樣。
4. opt_acwt:
    降低到 0.15~0.25 之間的效果會最好。
5. test_beam:
    調大會影響一點執行時間，但因為是 test，原本所需的時間就較少，因此不必太擔心時間；而調小會使 acc 下降一點點。

## 完整數據
因篇幅限制，在這裡並沒有列出所有數據與作圖，附上自己實測的數據紀錄。
https://hackmd.io/@shenperson/S1RpkuN3r