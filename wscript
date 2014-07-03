#! /usr/bin/env python
# encoding: utf-8

# needed for wafs target "waf dist"
VERSION='0.2'
APPNAME='feign'

# ??
top='.'

from waflib import Build

dirs = [
    "src",
    "plugins",
    ]


def options(ctx):
    # options to work with other tools
    ctx.add_option('--siox', action='store', default=False, help='path to siox repository')
    # options for features
    #ctx.add_option('+mpi', action='store', dest="feat_mpi", default=False, help='compile with mpi support')
    #ctx.add_option('+threads', action='store', dest="feat_threads", default=False, help='compile with multi-threading support')
    # fetch options recursively
    for d in dirs:
        ctx.recurse(d)
    pass

def configure(ctx): 
    # add feign include dir to environment
    ctx.env.append_value('INCLUDES', [ctx.path.abspath()+'/include'])
    # set siox environment based on --siox option
    ctx.env.SIOX = ctx.options.siox
    # recursivley configure subdirectories
    for d in dirs:
        ctx.recurse(d)
    pass

def build(ctx):
    for d in dirs:
        ctx.recurse(d)
    #ctx.recurse('docs')
    pass

def docs(ctx):
    for d in dirs:
        ctx.recurse(d)
    pass

