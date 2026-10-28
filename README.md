# TheKeebLab Sofle, wireless

Firmware for the wireless Sofle hand-built by [TheKeebLab](https://www.thekeeblab.com): ZMK v0.3.0 on nice!nano v2.

> Generated from TheKeebLab's private source. Edit in your fork; pull requests are read but not merged.

## Download

Ready-to-flash files are in the [latest release](https://github.com/TheKeebLab/sofle-wireless/releases/latest).

| Layout | Keymap | Files |
|---|---|---|
| QWERTY (US) | `config/sofle.keymap` | `sofle-wireless-qwerty-us-left.uf2`, `sofle-wireless-qwerty-us-right.uf2` |
| AZERTY (French) | `config/sofle-azerty-fr.keymap` | `sofle-wireless-azerty-fr-left.uf2`, `sofle-wireless-azerty-fr-right.uf2` |
| QWERTZ (German) | `config/sofle-qwertz-de.keymap` | `sofle-wireless-qwertz-de-left.uf2`, `sofle-wireless-qwertz-de-right.uf2` |
| QWERTY (Italian) | `config/sofle-qwerty-it.keymap` | `sofle-wireless-qwerty-it-left.uf2`, `sofle-wireless-qwerty-it-right.uf2` |
| QWERTY (Spanish) | `config/sofle-qwerty-es.keymap` | `sofle-wireless-qwerty-es-left.uf2`, `sofle-wireless-qwerty-es-right.uf2` |

To flash a half, plug it in over USB and press its reset button twice quickly: it shows up as a USB drive. Copy
the `-left` file onto the left half, the `-right` file onto the right half; the half restarts on its own. The
[quick-start guide](https://www.thekeeblab.com/guides/sofle-wireless-quick-start) says where the reset button is.

`sofle-wireless-settings-reset.uf2` clears what the halves saved (Bluetooth pairings, ZMK Studio changes, screen
brightness): flash it on both halves, then the firmware again.

Set your computer's keyboard language to the layout you flash. The base layer's keys stay where they are; the
other layers type the same characters as the US keymap, and a few extra keys are listed at the top of each
keymap file.

## Change the keymap

- Without rebuilding: [ZMK Studio](https://zmk.studio), with the left half plugged in over USB.
- In the source: fork this repository, edit the keymap of your layout and push. GitHub Actions builds every file
  of the table (Actions tab, the run, Artifacts, `firmware`).
  GitHub turns workflows off in a fork: enable them once in your fork's Actions tab.

## What is in here

| Path | What |
|---|---|
| `config/` | The keymaps, `sofle.conf` (features), `tkl_power.dtsi` (switch-off and wake-up keys), `tkl_brightness.dtsi` (screen brightness key), `west.yml` (ZMK version) |
| `module/` | Behaviors the keymap uses: the screen brightness key, `wheel_step` (one mouse-wheel step per knob notch) |
| `build.yaml` | What GitHub Actions builds |

The release files show TheKeebLab's screens. Firmware built from this source shows ZMK's status screen instead:
TheKeebLab's screens aren't part of this repository.

## Licence

MIT, like ZMK: see `LICENSE`.
