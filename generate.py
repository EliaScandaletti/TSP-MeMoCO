import math
import random


def generate(n, grid_size):
    grid = [[False for j in range(grid_size)] for i in range(grid_size)]
    points = []
    c = 0
    while c < n:
        x1 = random.choice(range(grid_size))
        y1 = random.choice(range(grid_size))
        x2 = random.choice(range(grid_size))
        y2 = random.choice(range(grid_size))
        if not grid[x1][y1]:
            grid[x1][y1] = True
            points.append((x1, y1))
            c += 1
        if not grid[x1][y2]:
            grid[x1][y2] = True
            points.append((x1, y2))
            c += 1
        if not grid[x2][y1]:
            grid[x2][y1] = True
            points.append((x2, y1))
            c += 1
        if not grid[x2][y2]:
            grid[x2][y2] = True
            points.append((x2, y2))
            c += 1
    random.shuffle(points)
    while n < len(points):
        points.pop()
    # print("\n".join(["".join(["#" if e else "_" for e in r]) for r in grid]))
    return points

def store(file, points, n):
    with open(file, "w") as f:
        f.write(str(len(points))+"\n")
        for i in range(len(points)):
            (x1, y1) = points[i]
            for j in range(len(points)):
                (x2, y2) = points[j]
                dist = math.sqrt((x1-x2)**2 + (y1-y2)**2)
                dist = round(dist, 2)
                f.write(str(dist) + "\t")
            f.write("\n")
    return

if __name__ == "__main__":
    for c in range(10):
        for n in range(60, 251, 10):
            grid = generate(n, int(n/2))
            store("data/tsp"+str(n)+"_"+chr(65+c)+".dat", grid, int(n/2))