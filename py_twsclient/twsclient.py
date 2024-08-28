#!/usr/bin/python3

from manager import Manager


def main():
    manager = Manager()

    manager.init()

    manager.stop()


if __name__ == "__main__":
    main()
