# omenkey — HP Omen keyboard RGB control for Linux

Control the 4-zone RGB backlight of an **HP Omen** laptop keyboard on Linux,
where HP's own software is Windows-only and the stock Linux driver exposes no
RGB control at all.

This project provides a small kernel module (`omen_rgb`) that talks to the HP
firmware over WMI, plus a command-line tool (`omenkey`) to set colors easily.

> **Tested on:** OMEN by HP Gaming Laptop 16-wd0xxx (board `8BA9`, BIOS F.26),
> Ubuntu 26.04, kernel 7.0. Other 4-zone Omen models with the same WMI BIOS
> interface are likely to work, but are untested.

---

## What works

- ✅ Set the color of each of the 4 keyboard zones (or all at once)
- ✅ Named colors and custom hex values
- ✅ "Brightness" via RGB scaling (e.g. `white 50` → dimmer white)
- ✅ Persistent colors at boot (systemd service)
- ✅ Survives kernel updates (DKMS)

## What does NOT work

- ❌ **Hardware brightness control.** This firmware accepts the brightness WMI
  command but ignores it — the keyboard stays at the same physical brightness.
  As a workaround, brightness is *emulated* by scaling the RGB value
  (`FFFFFF` = full white, `808080` = half-bright white, etc.).
- ❌ **Per-key RGB.** The hardware is 4-zone only; individual keys cannot be
  addressed.

---

## How it works (technical)

The keyboard lighting is **not** a USB HID device. Colors are set through HP's
WMI BIOS interface:

| Item | Value |
|---|---|
| WMI GUID | `5FB7F034-2C63-45E9-BE91-3D44E2C707E4` |
| Command (`command`) | `0x20009` (FOURZONE) |
| Sub-command (`commandtype`) | `2` = color GET, `3` = color SET |
| Data buffer | 128 bytes |
| Layout | byte 0 = `0x03` header; zone *N* color at offset `25 + N*3`, in R,G,B order |

Writing a color is a **read-modify-write**: the module reads the full 128-byte
state, changes only the 3 bytes of the target zone, and writes it back, so the
header and the other zones are preserved.

The module uses `wmi_evaluate_method()` and exposes four sysfs files under a
platform device.

---

## Installation

Requires `dkms`, `gcc`, `make`, and the kernel headers for your running kernel
(all present by default on Ubuntu).

### Quick install (recommended)

`install.sh` does the whole setup in one step (DKMS install, boot auto-load,
`omenkey` tool, and the systemd service) and is safe to re-run:

```bash
cd omen_rgb
sudo ./install.sh
```

### Manual install

If you prefer to run the steps yourself:

```bash
cd omen_rgb

# 1) Install the module via DKMS (auto-rebuilds on kernel updates)
sudo cp -r . /usr/src/omen_rgb-0.3
sudo dkms add     -m omen_rgb -v 0.3
sudo dkms build   -m omen_rgb -v 0.3
sudo dkms install -m omen_rgb -v 0.3

# 2) Load the module automatically at boot
echo omen_rgb | sudo tee /etc/modules-load.d/omen_rgb.conf

# 3) Install the command-line tool
sudo install -m 755 omenkey /usr/local/bin/omenkey

# Load it now (or reboot)
sudo modprobe omen_rgb
```

After installation the control files appear at:

```
/sys/devices/platform/omen_rgb/rgb_zones/zone0
/sys/devices/platform/omen_rgb/rgb_zones/zone1
/sys/devices/platform/omen_rgb/rgb_zones/zone2
/sys/devices/platform/omen_rgb/rgb_zones/zone3
```

---

## Usage

```
omenkey <color> [pct]        set all zones to a named color (optional brightness 0-100)
omenkey RRGGBB               set all zones to a hex color
omenkey Z0 Z1 Z2 Z3          set each zone to a hex color
omenkey get                  print current zone colors
omenkey list                 list available color names
omenkey help                 show help
```

**Named colors:** `white red green blue yellow cyan magenta orange purple pink off`

### Examples

```bash
omenkey white            # all white, full
omenkey white 50         # all white, ~50% brightness (scaled RGB)
omenkey red 30           # dim red
omenkey 808080           # custom hex
omenkey FF0000 00FF00 0000FF FFFFFF   # one color per zone
omenkey off              # turn the lighting off
omenkey get              # read current colors
```

You can also write directly to sysfs without the tool:

```bash
sudo bash -c 'echo 00FFFF > /sys/devices/platform/omen_rgb/rgb_zones/zone0'
```

---

## Persistent colors at boot

A systemd service re-applies your colors on every boot (the firmware does not
remember them across reboots, and the module reads whatever the firmware holds).

The service runs `omenkey restore`, which re-applies the **last color you
set**: every time you run `omenkey <color>` the choice is saved, and the next
boot restores it. So you don't edit the service — just set the color you want
once, e.g.:

```bash
omenkey blue     # applied now, and restored on every reboot
```

`install.sh` already installs and enables this service. If you did a manual
install, enable it with:

```bash
sudo install -m 644 omen-rgb-colors.service /etc/systemd/system/omen-rgb-colors.service
sudo systemctl daemon-reload
sudo systemctl enable --now omen-rgb-colors.service
sudo systemctl status omen-rgb-colors.service
```

The service waits for the `omen_rgb` sysfs interface to exist before writing,
so make sure the module is set to load at boot (step 2 of installation).

---

## Uninstall

```bash
sudo systemctl disable --now omen-rgb-colors.service
sudo rm /etc/systemd/system/omen-rgb-colors.service
sudo rm /etc/modules-load.d/omen_rgb.conf
sudo rm /usr/local/bin/omenkey
sudo dkms remove -m omen_rgb -v 0.3 --all
sudo rm -rf /usr/src/omen_rgb-0.3
sudo modprobe -r omen_rgb
```

---

## Project files

| File | Purpose |
|---|---|
| `install.sh` | One-step setup (DKMS, boot auto-load, tool, service) |
| `omen_rgb.c` | Kernel module: WMI access + sysfs zone files |
| `Makefile` | Out-of-tree module build |
| `dkms.conf` | DKMS packaging (persistence across kernel updates) |
| `omenkey` | Command-line tool to set colors |
| `omen-rgb-colors.service` | systemd service to apply colors at boot |

---

## Safety notes

- The module only ever uses the `0x20009` (4-zone lighting) WMI command. It
  does not touch fan, power, or other firmware functions.
- The module is unsigned, so loading it taints the kernel (harmless for local
  use; expected on Secure Boot setups you'd need to sign it yourself).
- **Use at your own risk.** This was built by reverse-engineering the firmware
  interface; it is not affiliated with or endorsed by HP.

## Credits

WMI protocol structure and the original 4-zone reference implementation come
from the Linux community work on `hp-wmi`, notably
[pelrun/hp-omen-linux-module](https://github.com/pelrun/hp-omen-linux-module).
