# coding: utf8

from distutils.core import setup, Extension

mount = Extension('mount', sources = ['mountmodule.c'])

setup(
    name = 'mount',
    version = '1.0',
    description = 'Python interface for mount(2)',
    ext_modules = [mount]
)
