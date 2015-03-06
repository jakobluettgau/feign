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
#    "plugins",
#    "demos",
#    "doc",
    ]

feign_plugins = [
    "tools/gen/layers/posix",
]


def options(ctx):
    # options to work with other tools
    ctx.add_option('--debug', action='store', default=False, help='enable debugging')
    ctx.add_option('--doc', action='store', default='', help='enable documentation')
    ctx.add_option('--production', action='store', default=False, help='enable debugging')
    ctx.add_option('--siox', action='store', default=False, help='path to siox repository')
    # options for features (probably not going to work)
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

    if ctx.options.doc:
        dirs.append("doc")

    # recursivley configure subdirectories
    for d in dirs:
        ctx.recurse(d)
    pass

def build(ctx):
    if ctx.options.doc:
        dirs.append("doc")

    for d in dirs:
        ctx.recurse(d)
    #ctx.recurse('docs')
    pass


def plugins(ctx):
    if ctx.options.doc:
        dirs.append("plugins")
        dirs.append("demos")

    ctx(features="build")

    build(ctx)
    #ctx.recurse('docs')
    pass


def doc(ctx):
    for d in dirs:
        ctx.recurse(d)
    pass


def generate_plugins(ctx):
    # generate plugins
    print "To generate plugins copy and run the following commands:"
    for p in feign_plugins:
       # ctx.recurse(d)
       cmd = "export ORIGIN=$PWD && cd %s && make siox && cd $ORIGIN" % (p)
       print cmd
    pass
