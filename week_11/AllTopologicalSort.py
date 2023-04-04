import networkx as nx

DG = nx.DiGraph([('B', 'A'), ('E', 'B'), ('B', 'C'), ('B', 'D'), ('C', 'D'), ('F', 'D'),
                 ('F', 'E'), ('G', 'E'), ('G', 'F'), ('G', 'H'), ('D', 'H')])

topSorts = list(nx.all_topological_sorts(DG))
topSorts = list(map(lambda x: ''.join(x), topSorts))

for topSort in topSorts:
    print(topSort)
