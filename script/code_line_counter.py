import os

line_counter = 0


def file_counter(file_path):
    with open(file_path, "r") as file:
        for _ in file:
            global line_counter
            line_counter += 1


def main():
    for element in os.scandir("./"):
        if element.is_file() and (element.name.endswith(".c") or element.name.endswith(".h")):
            file_counter(element.path)
            
    global line_counter
    print(line_counter)


if __name__ == "__main__":
    main()