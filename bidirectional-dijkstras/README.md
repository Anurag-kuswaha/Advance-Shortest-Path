# impelement using bidirectional Dijkstras

## Problem: Friend Suggestion
## Problem Introduction
Social networks are live on the connections between people, so friend
suggestions is one of the most important features of Facebook. One of
the most important inputs of the algorithm for friend suggestion is most
probably the current distance between you and the suggested person
in the graph of friends connections. Your task is to implement efficient
computation of this distance. The grader will test your algorithm against
different real-world networks, such as a part of internet, a network of
scientific citations or coauthorship, a social network of jazz musicians or
even a social network of dolphins :) You need to compute the distance
between two nodes in such network. We took some of the graphs from
here to use in the grader, and you can play with more of them on your
own computer
## Problem Description
## Task.
   Compute the distance between several pairs of nodes in the network
## Input Format
The first line contains two integers π and π β the number of nodes and edges in the
network, respectively. The nodes are numbered from 1 to π. Each of the following π lines contains
three integers π’, π£ and π describing a directed edge (π’, π£) of length π from the node number π’to the
node number π£. (Note that some social networks are represented by directed graphs while some other
correspond naturally to undirected graphs. For example, Twitter is a directed graph (with a directed
edge (π’, π£) meaning that π’ follows π£), while Facebook is an undirected graph (where an undirected
edge {π’, π£} means that π’ and π£ are friends). In this problem, we work with directed graphs only for a
simple reason. It is easy to turn an undirected graph into a directed one: just replace each undirected
edge {π’, π£} with a pair of directed edges (π’, π£) and (π£, π’).)
The next line contains an integer π β the number of queries for computing the distance. Each of the
following π lines contains two integers π’ and π£ β the numbers of the two nodes to compute the distance
from π’ to v
## Constraints.
   1 β€ π β€ 1 000 000; 1 β€ π β€ 6 000 000; 1 β€ π’, π£ β€ π; 1 β€ π β€ 1 000; 1 β€ π β€ 1 000
## Output Format:
   For each query, output one integer on a separate line. If there is no path from π’ to π£,
   output β1. Otherwise, output the distance from π’ to π£.
