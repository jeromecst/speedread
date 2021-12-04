# USAGE

[![asciicast](https://asciinema.org/a/453696.svg)](https://asciinema.org/a/453696)
```
speedread [-w wpm] [-c color] [-n num] [-p pos] [-h] file
```
or
```
cat text.txt | speedread [-w wpm] [-c color] [-n num] [-p pos] [-h] [file]
```

## Options
* [-w wpm] where **wpm** is the number of word displayed per minute
* [-c color] where **color** can be {black, blue, gree, cyan, red, purple, brown, gray, yellow, white}
* [-n pos] where **pos** jumps to the pos-ith word of the input

# BUILD

**Install**
Now requires `ncurses`. To compile, type:
```
sudo make install
```

**Uninstall**
```
sudo make uninstall
```
