import numpy as np
import networkx as nx
import matplotlib.pyplot as plt

term = []

nonterm = []

rules = []

dict_rules = {}

matrix = []

def get_terminate(string):
    for char in string:
        if char in term:
            return char

def next_state(nonterminate,states):
    result = []
    for state in states:
        if state in dict_rules:
            for rule in dict_rules[state]:
                if nonterminate in rule:
                    result.append(get_terminate(rule))
    result = set(result)
    if len(result) == 1:
        return result.pop()
    return result

def to_determinate(start_rule):
    dfsm = []
    for nt in nonterm:
        dfsm.append([])

    collumn = [start_rule]

    for t in collumn:
        for nt in nonterm:
            dfsm[nonterm.index(nt)].append(next_state(nt,t))


        for r in dfsm:
            for c in r:
                if c not in collumn:
                    if len(c) > 0:
                        collumn.append(c)

    term.clear()

    for t in collumn:
        if t not in term:
            term.append(t)

    return dfsm
        


file1 = open('t.txt')


#read terminals

line = file1.readline()
line.replace(" ", "")
line = line[:-1]
term = line.split(',')

#read nonterminals

line = file1.readline()
line.replace(" ", "")
line = line[:-1]
nonterm = line.split(',')


#read rules
line = file1.readline()
line.replace(" ", "")
line = line[:-1]
rules = line.split(';')



for rule in rules:
    dict_rules[rule[0]] = rule[3:].split('|')

line = file1.readline()
line.replace(" ", "")
startrule = line

#build nfsm

term.insert(0,'N')

for rules in dict_rules:
    for rule in dict_rules[rules]:
        if len(rule) > 2:
            raise Exception("Gramary isn't regular")
        if len(rule) == 1:
            if rule in nonterm:
                dict_rules[rules].remove(rule)
                dict_rules[rules].insert(0,rule+'N')
        for s in rule:
            if s in term:
                continue
            elif s in nonterm:
                continue
            else:
                raise Exception("There is unknown symbol")

#print(dict_rules)



for nt in nonterm:
    matrix.append([])
    for t in term:
        matrix[nonterm.index(nt)].append([])

#print(matrix)

for rules in dict_rules:
    for rule in dict_rules[rules]:
        print(rule)
        if len(rule) == 0:
            continue
        if rule[0] in nonterm:
            matrix[nonterm.index(rule[0])][term.index(rules)].append(rule[1])
        elif rule[1] in nonterm:
            matrix[nonterm.index(rule[1])][term.index(rules)].append(rule[0])

print("   ",term)
i = 0
for row in matrix:
    print(nonterm[i], end =" | ")
    print(row)
    i = i + 1


# build dfsm

dfsm = to_determinate(startrule)

print(term)
i = 0
for row in dfsm:
    print(nonterm[i], end =" | ")
    print(row)
    i = i + 1

i = 0
for t in term:
    if len(t) > 1:
        term.remove(t)
        term.insert(i,'A'+str(i))
        for row in dfsm:
            for el in row:
                if el == t:
                    row[row.index(el)] = 'A'+str(i)
    i = i + 1

print("   ",term)

i = 0
for row in dfsm:
    print(nonterm[i], end =" | ")
    print(row)
    i = i + 1

#Visualization

G = nx.Graph()

G.add_nodes_from(term)

i = 0
for t in term:
    for r in dfsm:
        if len(r[i]) == 0:
            continue
        else :
            G.add_edge(t, r[i])
    i = i + 1

nx.draw_networkx(G)

plt.show()