
```sh
xattr -h
usage: xattr [-lz] file [file ...]
       xattr -p [-lz] attr_name file [file ...]
       xattr -w [-z] attr_name attr_value file [file ...]
       xattr -d attr_name file [file ...]

The first form lists the names of all xattrs on the given file(s).
The second form (-p) prints the value of the xattr attr_name.
The third form (-w) sets the value of the xattr attr_name to attr_value.
The fourth form (-d) deletes the xattr attr_name.

options:
  -h: print this help
  -l: print long format (attr_name: attr_value)
  -z: compress or decompress (if compressed) attribute value in zip format
```



