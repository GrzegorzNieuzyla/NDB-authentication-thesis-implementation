#!/usr/bin/python3
import sys


try:
    with open(sys.argv[1]) as f:
        ndb = sorted((filter(None, f.read().split('\n'))))

except:
    print(f"Usage {sys.argv[0]} ndbfile")
    exit(1)

length = len(ndb[0])


record = '0' * length


def print_records(ndb, lower, higher):
    l = int(lower, 2)
    h = int(higher, 2)
    for i in range(l+1, h):
        print("{0:b}".format(i).zfill(length))
        print()


for rec in ndb:
    lower = rec.replace('*', '0')
    higher = rec.replace('*', '1')
    print_records(ndb, record, lower)
    record = higher

print_records(ndb, record, '1'*length)