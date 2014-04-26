#! /usr/bin/env python
# encoding: utf-8

# needed for wafs target "waf dist"
VERSION='0.0.1'
APPNAME='feign'

# ??
top='.'

from waflib import Build

def options(ctx):
    ctx.recurse('src')
    pass

def configure(ctx):
    ctx.recurse('src')
    pass

def build(ctx):
    ctx.recurse('src')
    #ctx.recurse('docs')
    pass

def docs(ctx):
    ctx.recurse('docs')
    pass

