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
    const void *data = NULL;
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


static struct PyModuleDef mountmodule = {
    PyModuleDef_HEAD_INIT,
    "mount",
    "Python extension for Linux mount(2)/umount(2)",
    -1,
    MountMethods
};


PyMODINIT_FUNC
PyInit_mount(void)
{
    PyObject *m;

    m = PyModule_Create(&mountmodule);
    if (m == NULL)
        return NULL;

    MountError = PyErr_NewException("mount.MountError", NULL, NULL);
    if (MountError == NULL) 
    {
        Py_DECREF(m);
        return NULL;
    }
    Py_INCREF(MountError);
    if (PyModule_AddObject(m, "MountError", MountError) < 0) 
    {
        Py_DECREF(MountError);
        goto error;
    }

#define ADD_INT_CONSTANT(name) \
    if (PyModule_AddIntConstant(m, #name, name) < 0) \
        goto error

    /* mount flags */
    ADD_INT_CONSTANT(MS_RDONLY);
    ADD_INT_CONSTANT(MS_NOSUID);
    ADD_INT_CONSTANT(MS_NODEV);
    ADD_INT_CONSTANT(MS_NOEXEC);
    ADD_INT_CONSTANT(MS_SYNCHRONOUS);
    ADD_INT_CONSTANT(MS_REMOUNT);
    ADD_INT_CONSTANT(MS_MANDLOCK);
    ADD_INT_CONSTANT(MS_NOATIME);
    ADD_INT_CONSTANT(MS_NODIRATIME);
    ADD_INT_CONSTANT(MS_BIND);
    ADD_INT_CONSTANT(MS_RMT_MASK);

    /* mount flags from linux/fs.h */
    ADD_INT_CONSTANT(MS_SILENT);
    ADD_INT_CONSTANT(MS_STRICTATIME);
    ADD_INT_CONSTANT(MS_DIRSYNC);
    ADD_INT_CONSTANT(MS_MOVE);
    ADD_INT_CONSTANT(MS_RELATIME);

    /* the magic numbers */
    ADD_INT_CONSTANT(MS_MGC_VAL);
    ADD_INT_CONSTANT(MS_MGC_MSK);

    /* flags for umount2 */
    ADD_INT_CONSTANT(MNT_FORCE);
    ADD_INT_CONSTANT(MNT_DETACH);
    ADD_INT_CONSTANT(MNT_EXPIRE);

#undef ADD_INT_CONSTANT

    return m;

error:
    Py_CLEAR(MountError);
    Py_DECREF(m);
    return NULL;
}
