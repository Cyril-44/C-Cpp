
def perm(x: int, y: int):
    vis = set[tuple[int, int]]()
    for i in range(0, 100000):
        x, y = x+y - abs(x-y), abs(x-y)
        print(x, y)
        if (x,y) in vis or x == y:
            break
        vis.add((x, y))

if __name__=="__main__":
    x, y = map(int, input().split())
    print(f"Calculating f({x}, {y})")
    perm(x, y)