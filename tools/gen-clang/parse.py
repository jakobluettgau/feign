#!/usr/bin/env python
""" Usage: call with <filename>
"""

import sys
import clang.cindex

level = 0
count = 0

def visit(node):
    global level
    global count
    count += 1
    print count,
    print " "*level,
    print '%s [line=%s, col=%s]' % (
             node.displayname, node.location.line, node.location.column),
    print node.type.kind
    #print ""
    #print dir(node)
    #print ""
    #print dir(node.get_definition())
    #print "\n\n\n"
    
    # Recurse fo^r children of this node
    children = node.get_children()
    if children:
        level += 1
        for c in children:
            visit(c)
        level -= 1

index = clang.cindex.Index.create()
tu = index.parse(sys.argv[1])
print 'Translation unit:', tu.spelling

print dir(tu)
visit(tu.cursor)
