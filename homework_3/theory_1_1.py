from pprint import pprint
import networkx as nx
import matplotlib.pyplot as plt
import numpy as np

INF = 100000

G = nx.DiGraph()

infile = open('theory_1_1_edges')
for line in infile:
    s, e, w = line.split()
    w = int(w)
    G.add_edge(s, e, weight=w)
infile.close()

pos = nx.spring_layout(G)
nx.draw(G, pos, with_labels=True)
nx.draw_networkx_edge_labels(
    G, pos, edge_labels=nx.get_edge_attributes(G, 'weight')
)
plt.show()

A = nx.to_numpy_array(G, dtype=np.int64)
for i in range(A.shape[0]):
    for j in range(A.shape[1]):
        if i != j and A[i, j] == 0:
            A[i, j] = INF
print(A)

np.savetxt("theory_1_1", A, "%d")
