#!/usr/bin/env sh
# Build the extension into a scratch dir and run the tests. mount(2) needs
# CAP_SYS_ADMIN: non-root users run inside a user + mount namespace, or
# fall back to sudo where unprivileged namespaces are blocked (e.g. the
# AppArmor restriction on Ubuntu 24.04). Every route runs in a private mount
# namespace so test mounts vanish with the process, even if it is killed.

set -e

PYTHON=$(command -v "${PYTHON:-python3}")
BUILDDIR=$(mktemp -d)
trap 'rm -rf "$BUILDDIR"' EXIT

$PYTHON -m pip install --quiet --target "$BUILDDIR" .

if [ "$(id -u)" -eq 0 ]; then
    RUN="unshare --mount --propagation private"
elif unshare --user --map-root-user --mount true 2>/dev/null; then
    RUN="unshare --user --map-root-user --mount"
else
    RUN="sudo unshare --mount --propagation private"
fi

$RUN env PYTHONPATH="$BUILDDIR" "$PYTHON" -m unittest -v test_mount
