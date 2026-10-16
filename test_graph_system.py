ribs = {(1, 3), (2, 3), (3, 4), (2, 6), (5, 6)}
dots  = [1, 6, 2, 4, 5, 3]

layers = [[]]
sorted_dots = sorted(dots)
for i in range(len(sorted_dots)):
    d1 = sorted_dots[i]
    layers[-1].append(d1)
    if i < len(sorted_dots) - 1:
        d2 = sorted_dots[i+1]
        for d in layers[-1]:
            if (d, d2) in ribs or (d2, d) in ribs:
               layers.append([])
               break

print(layers)