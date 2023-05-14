from range import Range

def fibbonachi():
    previous, current = 0, 1
    while True:
        print(current)
        yield current
        previous, current = current, previous + current

seq = Range(fibbonachi())\
      .Where(lambda x: x % 3 == 0)\
      .Select(lambda x: x**2 if x % 2 == 0 else x)\
      .Take(5)\
      .ToList()

print(seq)