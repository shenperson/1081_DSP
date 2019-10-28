# Discrete Hidden Markov Model
### Digital Speech Processing (DSP), HW1
### 電機三 沈哲瑋 B06901090
---
## Result:
* after training 1000 epoch
    ```
    acc : 2151 / 2500
    0.8604
    ```

## How to run my program
* compile:
    * training:
        ```
        make train
        ```
    * testing:
        ```
        make test
        ```
    * clean folder:
        ```
        make clean
        ```
* excute:
    * training:
        ```
        ./train <iter> <model_init_path> <seq_path> <output_model_path>
        ```
    * testing:
        ```
        ./test <models_list_path> <seq_path> <output_result_path>
        ```