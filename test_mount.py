# coding: utf8


import unittest
import os, tempfile
import mount


class MountTestCase(unittest.TestCase):

    tmpdir = "/tmp/test_mount/"

    def setUp(self):
        self.target = os.path.join(self.tmpdir, "target")
        self.source = os.path.join(self.tmpdir, "source")
        self.test_file = "test.txt"

        os.mkdir(self.tmpdir)
        os.mkdir(self.target)
        os.mkdir(self.source)

        with open(os.path.join(self.source, self.test_file), "w") as f:
            f.write("test")

    def test_mount(self):
        mount.mount(self.source, self.target, "auto", mount.MS_BIND)
        self.assertTrue(os.path.exists(os.path.join(self.target,
            self.test_file)))

    def test_umount(self):
        self.test_mount()
        mount.umount(self.target)
        self.assertFalse(os.path.exists(os.path.join(self.target,
            self.test_file)))

    def test_umount2(self):
        self.test_mount()
        mount.umount2(self.target, mount.MNT_FORCE)
        self.assertFalse(os.path.exists(os.path.join(self.target,
            self.test_file)))

    def tearDown(self):
        try:
            mount.umount2(self.target, mount.MNT_FORCE)
        except mount.MountError:
            pass
        os.unlink(os.path.join(self.source, self.test_file))
        os.rmdir(self.source)
        os.rmdir(self.target)
        os.rmdir(self.tmpdir)


if __name__ == "__main__":
    unittest.main()
