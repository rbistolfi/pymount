#!/usr/bin/env python3
# coding: utf8


from setuptools import setup, Extension

mount = Extension('mount', sources=['mountmodule.c'])

setup(
        name='mount',
        version='1.1',
        description='Python interface for mount(2)',
        ext_modules=[mount])
