#!/usr/bin/python3
import sys

def header(vars, claues):
    return f'p cnf {vars} {claues}\n'


def convert(line):
    result = ''
    line = line.strip()
    if not line: return
    var = 1
    for char in line:
        if char == '1':
            result += f'-{var} '
        elif char == '0':
            result += f'{var} '
        var += 1    
    result += '0'
    return result


def to_dimacs(lines, output):
    vars = len(lines[0].strip())
    clauses = len(lines)
    with open(output, 'w') as f:
        f.write(header(vars, clauses))
        for line in lines:
            f.write(convert(line))
            f.write('\n')
            



if __name__ == '__main__':
    if len(sys.argv) < 3:
        print(f'Usage: {sys.argv[0]} ndbfile outputfile')
        exit(1)
    with open(sys.argv[1]) as f:
        content = f.readlines()
    to_dimacs(content, sys.argv[2])