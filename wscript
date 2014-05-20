#! /usr/bin/env python
# encoding: utf-8

# needed for wafs target "waf dist"
VERSION='0.2'
APPNAME='feign'

# ??
top='.'

from waflib import Build

def options(ctx):
    # options to work with other tools
    ctx.add_option('--siox', action='store', default=False, help='path to siox repository')
    # options for features
    #ctx.add_option('+mpi', action='store', dest="feat_mpi", default=False, help='compile with mpi support')
    #ctx.add_option('+threads', action='store', dest="feat_threads", default=False, help='compile with multi-threading support')
    # fetch options recursively
    ctx.recurse('src')
    pass

def configure(ctx): 
    ctx.env.SIOX = ctx.options.siox
    # recursivley configure subdirectories
    ctx.recurse('src')
    pass

def build(ctx):
    ctx.recurse('src')
    #ctx.recurse('docs')
    pass

def docs(ctx):
    ctx.recurse('docs')
    pass

