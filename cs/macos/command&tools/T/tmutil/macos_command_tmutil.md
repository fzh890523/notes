
```
tmutil
Usage: tmutil version

Usage: tmutil enable

Usage: tmutil disable

Usage: tmutil startbackup [-a | --auto] [-b | --block] [-r | --rotation] [-d | --destination dest_id]

Usage: tmutil stopbackup

Usage: tmutil enablelocal

Usage: tmutil disablelocal

Usage: tmutil snapshot

Usage: tmutil delete snapshot_path ...

Usage: tmutil restore [-v] src ... dst

Usage: tmutil compare [-@acdefghlmnstuEX] [-D depth] [-I name]
       tmutil compare [-@acdefghlmnstuEX] [-D depth] [-I name] snapshot_path
       tmutil compare [-@acdefghlmnstuEUX] [-D depth] [-I name] path1 path2

Usage: tmutil setdestination [-a]  mount_point
       tmutil setdestination [-ap] afp://user[:pass]@host/share

Usage: tmutil removedestination destination_id

Usage: tmutil destinationinfo [-X]

Usage: tmutil addexclusion [-p|-v] item ...

Usage: tmutil removeexclusion [-p|-v] item ...

Usage: tmutil isexcluded item ...

Usage: tmutil inheritbackup machine_directory
       tmutil inheritbackup sparse_bundle

Usage: tmutil associatedisk [-a] mount_point volume_backup_directory

Usage: tmutil latestbackup

Usage: tmutil listbackups

Usage: tmutil machinedirectory

Usage: tmutil calculatedrift machine_directory

Usage: tmutil uniquesize path ...

Usage: tmutil verifychecksums path ...
```
0()