yambar-decorations(5)

# NAME
yambar-decorations - configuration file

# DESCRIPTION

Decorations are optional additions to particles, enabling you to
e.g. change the background color of the entire particle, draw an
underline and so on.

Decorations are applied to the particle's margins as well.

All decorations define their own configuration attributes, and there
are no common attributes.

# BACKGROUND

This decoration sets the particles background color. Optionally the
background is drawn as a rounded rectangle, inset from the top and
bottom of the bar, with an outline and a soft shadow (yambar-wl).

## CONFIGURATION

[[ *Name*
:[ *Type*
:[ *Req*
:< *Description*
|  color
:  color
:  yes
:  The background color. See *yambar*(5) for format.
|  radius
:  int
:  no
:  Corner radius, in pixels. Default 0 (square corners). Clamped to half the height/width.
|  margin
:  int
:  no
:  Vertical inset from the top and bottom of the bar, in pixels. Default 0.
|  border-width
:  int
:  no
:  Width of an outline drawn inside the shape, in pixels. Default 0. Requires *border-color*.
|  border-color
:  color
:  no
:  Color of the outline.
|  shadow-size
:  int
:  no
:  Length of a soft shadow around the shape, in pixels. Default 0. Requires *shadow-color*. Parts outside the bar are clipped.
|  shadow-color
:  color
:  no
:  Shadow color at the shape edge; fades to transparent at *shadow-size*.
|  shadow-offset
:  int
:  no
:  Shadow shift downwards, in pixels. Default 1.
|  hover-color
:  color
:  no
:  Fill color while the pointer is over the shape (yambar-wl).

## EXAMPLES

```
content:
  string:
    deco:
      background:
        color: 00ff00ff
```

Rounded "pill" with outline and shadow, applied to a whole list:

```
content:
  list:
    left-margin: 8
    right-margin: 8
    deco:
      background:
        color: 232731ff
        radius: 5
        margin: 4
        border-width: 1
        border-color: 88c0d080
        shadow-size: 8
        shadow-color: 16191fff
    items:
      - string: {text: "{cpu}%"}
```

# UNDERLINE

This decoration renders a line of configurable size and color at the
bottom of the particle.

## CONFIGURATION

[[ *Name*
:[ *Type*
:[ *Req*
:< *Description*
|  size
:  int
:  yes
:  The size (height/thickness) of the line, in pixels
|  inset
:  int
:  no
:  Shrink the line by this many pixels on both sides (yambar-wl). Default 0.
|  color
:  color
:  yes
:  The color of the line. See *yambar*(5) for format.

## EXAMPLES

```
content:
  string:
    deco:
      underline:
        size: 2
        color: ff0000ff
```


# OVERLINE

Similar to _underline_, this decoration renders a line of configurable
size and color at the top of the particle.

## CONFIGURATION

[[ *Name*
:[ *Type*
:[ *Req*
:< *Description*
|  size
:  int
:  yes
:  The size (height/thickness) of the line, in pixels
|  color
:  color
:  yes
:  The color of the line. See *yambar*(5) for format.

## EXAMPLES

```
content:
  string:
    deco:
      overline:
        size: 2
        color: ff0000ff
```


# BORDER

This decoration renders a border of configurable size (i.e border
width) around the particle.

## CONFIGURATION

[[ *Name*
:[ *Type*
:[ *Req*
:< *Description*
|  color
:  color
:  yes
:  The color of the line. See *yambar*(5) for format.
|  size
:  int
:  no
:  Border width, in pixels. Defaults to 1px.

## EXAMPLES

```
content:
  string:
    deco:
      border:
        size: 2
        color: ff0000ff
```


# STACK

This particle combines multiple decorations.

## CONFIGURATION

No configuration attributes available. The _stack_ is just a *list* of
the decorations that should be combined.

## EXAMPLES

```
content:
  string:
    deco:
      stack:
        - background: ...
        - underline: ...
```

# SEE ALSO

*yambar-particles*(5)
