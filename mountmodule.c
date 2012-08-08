/* mountmodule.c - Python extension for Linux mount - by rbistolfi */


#include "Python.h"
#include <errno.h>
#include <string.h>
#include <sys/mount.h>
#include <linux/fs.h>


static PyObject *MountError;


static PyObject *
mount_mount(PyObject *self, PyObject *args)
{
    const char *source;
    const char *target;
    const char *filesystemtype;
    unsigned long mountflags;
    const void *data;
    int sts;

    if (!PyArg_ParseTuple(args, "sssl|s", &source, &target, &filesystemtype,
                &mountflags, &data))
        return NULL;

    sts = mount(source, target, filesystemtype, mountflags, data);

    if (sts == -1) 
    {
        PyErr_SetString(MountError, strerror(errno));
        return NULL;
    }

    return Py_BuildValue("");
}


static PyObject *
mount_umount(PyObject *self, PyObject *args)
{
    const char *target;
    int sts;

    if (!PyArg_ParseTuple(args, "s", &target))
        return NULL;

    sts = umount(target);

    if (sts == -1)
    {
        PyErr_SetString(MountError, strerror(errno));
        return NULL;
    }

    return Py_BuildValue("");
}


static PyObject *
mount_umount2(PyObject *self, PyObject *args)
{
    const char *target;
    int flags;
    int sts;

    if (!PyArg_ParseTuple(args, "si", &target, &flags))
        return NULL;

    sts = umount2(target, flags);

    if (sts == -1)
    {
        PyErr_SetString(MountError, strerror(errno));
        return NULL;
    }

    return Py_BuildValue("");
}


static PyMethodDef MountMethods[] = {
    {"mount", mount_mount, METH_VARARGS, "Mount a filesystem"},
    {"umount", mount_umount, METH_VARARGS, "Umount a filesystem"},
    {"umount2", mount_umount2, METH_VARARGS, "Umount a filesystem"},
    {NULL, NULL, 0, NULL}
};


/* Helper function for inserting a numeric constant in the Python module */

void
add_constant_to_module(PyObject *m, char * name, unsigned long clong)
{
    PyObject *Py_LONG_CONSTANT = PyLong_FromLong(clong);
    Py_INCREF(Py_LONG_CONSTANT);
    PyModule_AddObject(m, name, Py_LONG_CONSTANT);
}


PyMODINIT_FUNC
initmount(void)
{
    PyObject *m;

    m = Py_InitModule("mount", MountMethods);
    if (m == NULL)
        return;

    MountError = PyErr_NewException("mount.MountError", NULL, NULL);
    Py_INCREF(MountError);
    PyModule_AddObject(m, "MountError", MountError);

    /* mount flags */
    add_constant_to_module(m, "MS_RDONLY", MS_RDONLY);
    add_constant_to_module(m, "MS_NOSUID", MS_NOSUID);
    add_constant_to_module(m, "MS_NODEV", MS_NODEV);
    add_constant_to_module(m, "MS_NOEXEC", MS_NOEXEC);
    add_constant_to_module(m, "MS_SYNCHRONOUS", MS_SYNCHRONOUS);
    add_constant_to_module(m, "MS_REMOUNT", MS_REMOUNT);
    add_constant_to_module(m, "MS_MANDLOCK", MS_MANDLOCK);
    add_constant_to_module(m, "MS_NOATIME", MS_NOATIME);
    add_constant_to_module(m, "MS_NODIRATIME", MS_NODIRATIME);
    add_constant_to_module(m, "MS_BIND", MS_BIND);
    add_constant_to_module(m, "MS_RMT_MASK", MS_RMT_MASK);

    /* mount flags from linux/fs.h */
    add_constant_to_module(m, "MS_SILENT", MS_SILENT);
    add_constant_to_module(m, "MS_STRICTATIME", MS_STRICTATIME);
    add_constant_to_module(m, "MS_DIRSYNC", MS_DIRSYNC);
    add_constant_to_module(m, "MS_MOVE", MS_MOVE);
    add_constant_to_module(m, "MS_RELATIME", MS_RELATIME);

    /* the magic numbers */
    add_constant_to_module(m, "MS_MGC_VAL", MS_MGC_VAL);
    add_constant_to_module(m, "MS_MGC_MSK", MS_MGC_MSK);

    /* flags for umount2 */
    add_constant_to_module(m, "MNT_FORCE", MNT_FORCE);
    add_constant_to_module(m, "MNT_DETACH", MNT_DETACH);
    add_constant_to_module(m, "MNT_EXPIRE", MNT_EXPIRE);
}
