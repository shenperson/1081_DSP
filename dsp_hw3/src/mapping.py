import sys
try:
    input_path = sys.argv[1]
    print('hi')
except:
    input_path = '../Big5-ZhuYin.map'
try:
    output_path = sys.argv[2]
    print('hihi')
except:
    output_path = '../output_data/ZhuYin-Big5.map'
zunyin_map = {}
with open(input_path,'r',encoding='big5hkscs') as file:
    lines = file.readlines()
    for i in lines:
        a = i.split()
        zunyin_map[a[0]]=set(a[0])
        for j in a[1].split('/'):
            if j[0] in zunyin_map:
                zunyin_map[j[0]].add(a[0])
            else:
                zunyin_map[j[0]] = set(a[0])
# print(zunyin_map)
# for i in zunyin_map.keys():
#     print(i,end=' ')
#     for j in zunyin_map[i]:
#         print(j,end=' ')
#     print()

with open(output_path,'w',encoding='big5hkscs') as file:
    for i in zunyin_map.keys():
        temp = i + ' '
        for j in zunyin_map[i]:
            temp = temp + j + ' '
        file.write(temp + '\n')