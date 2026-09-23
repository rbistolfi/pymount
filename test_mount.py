# coding: utf8


import unittest
import os, shutil, tempfile
import mount


@unittest.skipUnless(os.geteuid() == 0, "mount(2) requires CAP_SYS_ADMIN")
class MountTestCase(unittest.TestCase):

    def setUp(self):
        self.tmpdir = tempfile.mkdtemp(prefix="test_mount_")
        self.target = os.path.join(self.tmpdir, "target")
        self.source = os.path.join(self.tmpdir, "source")
        self.test_file = "test.txt"

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
        shutil.rmtree(self.tmpdir)


if __name__ == "__main__":
    unittest.main()
