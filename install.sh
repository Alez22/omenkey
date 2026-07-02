#!/usr/bin/env bash
# install.sh - Full setup for omen_rgb + omenkey.
#
# Does everything in one shot:
#   1. Install the kernel module via DKMS (auto-rebuild on new kernels)
#   2. Load the module at boot
#   3. Install the omenkey command-line tool
#   4. Install and enable the systemd service that restores colors at boot
#
# Usage:  sudo ./install.sh
# Requires: dkms, gcc, make, kernel headers (present by default on Ubuntu).

set -euo pipefail

VERSION="0.3"                       # must match dkms.conf
PKG="omen_rgb"
SRC_DEST="/usr/src/${PKG}-${VERSION}"
# Directory this script lives in (the sources sit next to it).
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Must run as root: it writes to /usr/src, /etc, /usr/local/bin.
if [ "$(id -u)" -ne 0 ]; then
	echo "install.sh: run me with sudo:  sudo ./install.sh" >&2
	exit 1
fi

echo "==> 1/4  Copying sources into ${SRC_DEST}"
rm -rf "$SRC_DEST"
mkdir -p "$SRC_DEST"
# Sources only: DKMS builds from scratch, build artifacts are not needed.
cp "$HERE/omen_rgb.c" "$HERE/Makefile" "$HERE/dkms.conf" "$SRC_DEST/"

echo "==> 2/4  Installing the module via DKMS"
# Remove any previous install of the same version so the script is
# re-runnable without "already added" errors.
dkms remove -m "$PKG" -v "$VERSION" --all 2>/dev/null || true
dkms add     -m "$PKG" -v "$VERSION"
dkms build   -m "$PKG" -v "$VERSION"
dkms install -m "$PKG" -v "$VERSION"

echo "==> 3/4  Loading the module (now and at every boot)"
echo "$PKG" > "/etc/modules-load.d/${PKG}.conf"
# Load it now if not already in the kernel (e.g. from an earlier manual insmod).
modprobe "$PKG" 2>/dev/null || true

echo "==> 4/4  Installing omenkey and the systemd service"
install -m 755 "$HERE/omenkey" /usr/local/bin/omenkey
install -m 644 "$HERE/omen-rgb-colors.service" /etc/systemd/system/omen-rgb-colors.service
systemctl daemon-reload
systemctl enable --now omen-rgb-colors.service

echo
echo "Done. Try now:  omenkey blue"
echo "The color you set is saved and restored at every reboot."
