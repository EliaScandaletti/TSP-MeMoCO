import math
import random


def generate(file, n, grid_size):
    grid = [[False for j in range(grid_size)] for i in range(grid_size)]
    points = []
    c = 0
    while c < n:
        i = random.choice(range(grid_size))
        j = random.choice(range(grid_size))
        if not grid[i][j]:
            grid[i][j] = True
            points.append((i, j))
            c += 1

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
    for n in range(10, 101, 5):
        generate("data/tsp"+str(n)+".dat", n, int(n/2))