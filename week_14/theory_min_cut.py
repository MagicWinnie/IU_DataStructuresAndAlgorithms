import networkx as nx

#              A  B  C  D  E  F  G  H
graph_list = [[0, 0, 0, 0, 5, 0, 0, 0],  # A
              [2, 0, 9, 6, 7, 1, 0, 0],  # B
              [0, 0, 0, 5, 0, 0, 0, 0],  # C
              [0, 0, 0, 0, 0, 0, 0, 2],  # D
              [0, 0, 0, 0, 0, 6, 2, 0],  # E
              [0, 0, 0, 3, 0, 0, 0, 2],  # F
              [0, 0, 0, 0, 0, 4, 0, 7],  # G
              [0, 0, 0, 0, 0, 0, 0, 0]]  # H
n = len(graph_list)

G = nx.DiGraph()
for i in range(n):
    for j in range(n):
        if graph_list[i][j]:
            G.add_edge(i, j, capacity=graph_list[i][j])

source = 1  # B
sink = 7    # H

cut_value, partition = nx.minimum_cut(G, source, sink)
print(cut_value, partition)
