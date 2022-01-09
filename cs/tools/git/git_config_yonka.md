



### color

ref: https://git-scm.com/docs/git-config



color

The value for a variable that takes a color is a list of colors (at most two, one for foreground and one for background) and attributes (as many as you want), separated by spaces.

The basic colors accepted are `normal`, `black`, `red`, `green`, `yellow`, `blue`, `magenta`, `cyan` and `white`. The first color given is the foreground; the second is the background. All the basic colors except `normal` have a bright variant that can be specified by prefixing the color with `bright`, like `brightred`.

Colors may also be given as numbers between 0 and 255; these use ANSI 256-color mode (but note that not all terminals may support this). If your terminal supports it, you may also specify 24-bit RGB values as hex, like `#ff0ab3`.

The accepted attributes are `bold`, `dim`, `ul`, `blink`, `reverse`, `italic`, and `strike` (for crossed-out or "strikethrough" letters). The position of any attributes with respect to the colors (before, after, or in between), doesn’t matter. Specific attributes may be turned off by prefixing them with `no` or `no-` (e.g., `noreverse`, `no-ul`, etc).

An empty color string produces no color effect at all. This can be used to avoid coloring specific elements without disabling color entirely.

For git’s pre-defined color slots, the attributes are meant to be reset at the beginning of each item in the colored output. So setting `color.decorate.branch` to `black` will paint that branch name in a plain `black`, even if the previous thing on the same output line (e.g. opening parenthesis before the list of branch names in `log --decorate` output) is set to be painted with `bold` or some other attribute. However, custom log formats may do more complicated and layered coloring, and the negated forms may be useful there.



