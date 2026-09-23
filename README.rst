Pymount
=======

.. image:: https://github.com/rbistolfi/pymount/actions/workflows/test.yml/badge.svg
   :target: https://github.com/rbistolfi/pymount/actions/workflows/test.yml
   :alt: test

Python C extension for Linux's mount


Usage
-----

::

    import mount
    mount.mount(source, target, filesystemtype, flags, data)
    mount.umount(target)
    mount.umount2(target, flags)

See man page for mount(2) for details.
