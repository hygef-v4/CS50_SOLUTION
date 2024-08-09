from cs50 import get_int

# Get integer between 1-8 from user
while True:
    height = get_int("Height: ")
    if height > 0 and height < 9:
        break

for i in range(0, height, 1):
    for j in range(0, height, 1):
        if i + j < height - 1:
            print(" ", end="")
        else:
            print("#", end="")
    print()
