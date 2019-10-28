./train 1000 ./model_init.txt ./data/train_seq_01.txt ./model_01.txt 
./train 1000 ./model_init.txt ./data/train_seq_02.txt ./model_02.txt 
./train 1000 ./model_init.txt ./data/train_seq_03.txt ./model_03.txt 
./train 1000 ./model_init.txt ./data/train_seq_04.txt ./model_04.txt 
./train 1000 ./model_init.txt ./data/train_seq_05.txt ./model_05.txt 
./test ./modellist.txt ./data/test_seq.txt ./result.txt
python3 check.py