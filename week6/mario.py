def main():

    height = validity()
    blank = height - 1
    bricks = 1

    for _ in range(height):
        print_line(blank, bricks)
        blank -= 1
        bricks += 1


def validity():
    while True:
        try:
            height = int(input("Enter pyramid height (1 - 8): "))
            if 1 <= height <= 8:
                return height
        except ValueError:
            print("Enter an integer between 1 and 8.")


def print_line(blank, bricks):
    for _ in range(blank):
        print(" ", end="")

    for _ in range(bricks):
        print("#", end="")

    print("  ", end="")

    for _ in range(bricks):
        print("#", end="")

    print("")


main()
