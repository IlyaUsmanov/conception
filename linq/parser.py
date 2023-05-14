from range import Range

def file_parser(filename):
    with open(filename) as f:
        for line in f:
            yield line

seq = Range(file_parser('text_example.txt'))\
      .Select(lambda line: Range(line.split(' ')))\
      .Flatten().GroupBy(lambda x: x)\
      .Select(lambda group_item: (group_item[0], len(group_item[1])))\
      .OrderBy(lambda group_item: group_item[1])\
      .ToList()

print(seq)