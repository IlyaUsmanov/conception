from collections import defaultdict

class Range:
    def __init__(self, source_iterable):
        self.source_iterable = source_iterable

    def __iter__(self):
        return iter(self.source_iterable)

    def Select(self, func):
        def _select_helper(iterable, func):
            for elem in iterable:
                yield func(elem)

        return Range(_select_helper(self.source_iterable, func))
    
    def Flatten(self):
        def _flatten_helper(iterable):
            for sequence in iterable:
                for elem in sequence:
                    yield elem

        return Range(_flatten_helper(self.source_iterable))
    
    def Where(self, filter):
        def _where_helper(iterable, filter):
            for elem in iterable:
                if filter(elem):
                    yield elem

        return Range(_where_helper(self.source_iterable, filter))
    
    def Take(self, elements_number):
        def _take_helper(iterable, elements_number):
            for elem_id, elem in enumerate(iterable):
                yield elem
                if elem_id == elements_number - 1:
                    return

        return Range(_take_helper(self.source_iterable, elements_number))
    
    def GroupBy(self, key_func):
        def _groupby_helper(iterable, key_func):
            groups = defaultdict(list)
            for elem in iterable:
                groups[key_func(elem)].append(elem)
            return groups.items()

        return Range(_groupby_helper(self.source_iterable, key_func))
    
    def OrderBy(self, key_func):
        def _orderby_helper(iterable, key_func):
            sequence = [elem for elem in iterable]
            return sorted(sequence, key=key_func)

        return Range(_orderby_helper(self.source_iterable, key_func))
    
    def ToList(self):
        return [elem for elem in self.source_iterable]