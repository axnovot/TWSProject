#!/usr/bin/python3

from tcmanager import TCManager


def main():
    manager = TCManager()

    manager.init()

    manager.reqClosingPrices()

    manager.stop()


if __name__ == "__main__":
    main()
