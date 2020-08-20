

```sh

# fdisk output

Command (m for help): i
Partition number (1-3, default 3): 3

         Device: /dev/sdb3
          Start: 12884901928
            End: 15628053134
        Sectors: 2743151207
           Size: 1.3T
           Type: Linux filesystem
      Type-UUID: 0FC63DAF-8483-4772-8E79-3D69D8477DE4
           UUID: 38F194B8-359B-41BE-85B4-C8449614DF48
           Name: study

# blkid output

/dev/sdb1: UUID="ec3f624c-b7ab-4c4b-b6e9-13733be9e4e3" TYPE="ext4" PARTLABEL="entertainment" PARTUUID="8d209cef-fe7a-477d-83d4-30439c987a40"
/dev/sdb2: UUID="9b105a05-97ea-4c41-b26e-735e5592a012" TYPE="ext4" PARTLABEL="storage" PARTUUID="e831f73d-c5f6-48f7-9c56-8577d2669c2e"
/dev/sdb3: UUID="a1a11ecc-548e-4d42-a70a-4d1315fe7141" TYPE="ext4" PARTLABEL="study" PARTUUID="38f194b8-359b-41be-85b4-c8449614df48"

# webmin里mount "Partition with ID" 用的是 blkid output 的的 UUID= 这个。 这个是 file system uuid
```
