
Pymount
=======

Python C extension for Linux's mount


Usage
-----

    import mount
    mount.mount(source, target, filesystemtype, flags, data)
    mount.umount(target)
    mount.umount2(target, flags)

See man page for mount(2) for details.
