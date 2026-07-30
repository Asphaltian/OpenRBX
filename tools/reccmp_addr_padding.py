#!/usr/bin/env python3

import logging
import sys
from pathlib import PurePath, Path
from typing import Iterable, Iterator, NamedTuple
import colorama
from reccmp.dir import source_code_search
from reccmp.formats import TextFile
from reccmp.parser.marker import markerRegex
from reccmp.project.detect import (
    RecCmpProject,
)


logger = logging.getLogger()

# The summary line ends in an emoji. A Windows console defaults to cp1252 and raises
# UnicodeEncodeError on it, which matters here because this is a Windows-only project.
if hasattr(sys.stdout, "reconfigure"):
    sys.stdout.reconfigure(encoding="utf-8", errors="replace")


class AddrProblem(NamedTuple):
    line_number: int
    line: str


def display_errors(alerts: Iterable[AddrProblem], filename: PurePath):
    sorted_alerts = sorted(alerts, key=lambda a: a.line_number)

    print(colorama.Fore.LIGHTWHITE_EX, end="")
    print(filename)

    for alert in sorted_alerts:
        components = [
            "  ",
            colorama.Fore.LIGHTWHITE_EX,
            f"{alert.line_number:4}",
            " : ",
        ]
        print("".join(components), end="")
        print(f"{colorama.Fore.WHITE}  {alert.line.strip()}")

    print()


def problem_markers(text: str) -> Iterator[AddrProblem]:
    for i, line in enumerate(text.splitlines(), start=1):
        match = markerRegex.match(line)
        if match is None:
            continue

        addr_text = match.group("offset")
        if len(addr_text) != 10:  # 0x12345678
            yield AddrProblem(i, line)


def main():
    project = RecCmpProject.from_directory(directory=Path.cwd())
    if not project:
        logger.error("Cannot find reccmp project")
        return 1

    reduced_file_list = set(
        path
        for target in project.targets.values()
        for path in source_code_search(target.source_paths)
    )

    count = 0

    codefiles = list(TextFile.from_files(sorted(reduced_file_list), allow_error=True))

    for file in codefiles:
        problems = list(problem_markers(file.text))
        count += len(problems)

        if problems:
            display_errors(problems, file.path)

    print(f"Problems: {count} {'❌' if count > 0 else '✅'}")
    return count > 0


if __name__ == "__main__":
    raise SystemExit(main())
