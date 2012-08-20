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
    PyModule_AddIntConstant(m, "MS_RDONLY", MS_RDONLY);
    PyModule_AddIntConstant(m, "MS_NOSUID", MS_NOSUID);
    PyModule_AddIntConstant(m, "MS_NODEV", MS_NODEV);
    PyModule_AddIntConstant(m, "MS_NOEXEC", MS_NOEXEC);
    PyModule_AddIntConstant(m, "MS_SYNCHRONOUS", MS_SYNCHRONOUS);
    PyModule_AddIntConstant(m, "MS_REMOUNT", MS_REMOUNT);
    PyModule_AddIntConstant(m, "MS_MANDLOCK", MS_MANDLOCK);
    PyModule_AddIntConstant(m, "MS_NOATIME", MS_NOATIME);
    PyModule_AddIntConstant(m, "MS_NODIRATIME", MS_NODIRATIME);
    PyModule_AddIntConstant(m, "MS_BIND", MS_BIND);
    PyModule_AddIntConstant(m, "MS_RMT_MASK", MS_RMT_MASK);

    /* mount flags from linux/fs.h */
    PyModule_AddIntConstant(m, "MS_SILENT", MS_SILENT);
    PyModule_AddIntConstant(m, "MS_STRICTATIME", MS_STRICTATIME);
    PyModule_AddIntConstant(m, "MS_DIRSYNC", MS_DIRSYNC);
    PyModule_AddIntConstant(m, "MS_MOVE", MS_MOVE);
    PyModule_AddIntConstant(m, "MS_RELATIME", MS_RELATIME);

    /* the magic numbers */
    PyModule_AddIntConstant(m, "MS_MGC_VAL", MS_MGC_VAL);
    PyModule_AddIntConstant(m, "MS_MGC_MSK", MS_MGC_MSK);

    /* flags for umount2 */
    PyModule_AddIntConstant(m, "MNT_FORCE", MNT_FORCE);
    PyModule_AddIntConstant(m, "MNT_DETACH", MNT_DETACH);
    PyModule_AddIntConstant(m, "MNT_EXPIRE", MNT_EXPIRE);
}
