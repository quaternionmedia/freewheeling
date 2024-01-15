#!/bin/env python3
from sys import argv


def help():
    print("Usage: python3 ardour-markers.py <usx file> <ardour session>")
    print("Output: <ardour session> with markers")
    exit()


def parse_usx(usx):
    for n, d in enumerate(usx):
        if d.startswith("[Markers"):
            return usx[n + 1 :]
    raise Exception("No markers found")


def add_markers(session, markers):
    return session.replace("<Locations>", "<Locations>\n" + "\n".join(markers))


def main():
    with open(argv[1], "r") as f:
        usx = f.read().splitlines()
    times = parse_usx(usx)
    marks = [t.split("=")[0] for t in times]
    markers = []
    for n, m in enumerate(marks):
        markers.append(
            f'<Location id="m{n}" name="{n}" start="{m}" end="{m}" flags="IsMark" />'
        )

    with open(argv[2], "r") as f:
        session = f.read()

    marked = add_markers(session, markers)
    with open(argv[2], "w") as f:
        f.write(marked)
    print('Markers added to "' + argv[2] + '"')


if __name__ == "__main__":
    if (
        not len(argv)
        == 3
        # or argv[1].endswith(".usx")
        # or not argv[2].endswith(".ardour")
    ):
        help()
    main()
