
f1 = './result.txt'
f2 = './data/test_lbl.txt'

mine = []
ans = []
with open(f1) as f:
    for line in f.readlines():
        mine.append(int(line[7]))
with open(f2) as f:
    for line in f.readlines():
        ans.append(int(line[7]))

total = len(mine)
correct = 0
for i in range(total):
    if mine[i] == ans[i]:
        correct += 1

print('acc :', correct, '/', total)
print(correct/total)
