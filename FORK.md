# yambar-wl

Wayland-only fork of [yambar](https://codeberg.org/dnkl/yambar) (upstream archived, last commit
`abeffbd` "readme: goodbye!", May 2025). MIT, as upstream.

Goal: a status bar with no X11, no D-Bus, no xdg-desktop-portal, no systemd/elogind at runtime.

## Differences from upstream

Build defaults (`meson_options.txt`):

| option          | upstream  | here       | why                                      |
|-----------------|-----------|------------|------------------------------------------|
| backend-x11     | enabled   | disabled   | Wayland only                              |
| plugin-mpris    | enabled   | disabled   | needs D-Bus via libsystemd/elogind/basu   |
| plugin-xkb      | auto      | disabled   | X11                                       |
| plugin-xwindow  | auto      | disabled   | X11                                       |

No code path in yambar talks to xdg-desktop-portal.

Decorations:

- `background`: `radius`, `margin` (vertical inset), `border-width`, `border-color`,
  `shadow-size`, `shadow-color`, `shadow-offset` (soft shadow, distance falloff)
- `underline`: `inset` (horizontal shrink)

Bar:

- clip region includes the side margins, so decoration shadows of the outermost
  particles are not cut off

Example config: `examples/configurations/pill.yml`.

## Build

```sh
meson setup build --buildtype=release --wrap-mode=nofallback
ninja -C build
install -m755 build/yambar ~/.local/bin/
```

The binary keeps the name `yambar`; config stays in `~/.config/yambar/`.
