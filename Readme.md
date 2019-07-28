# crc-verifier
This lightweight utility allows you to track changes in executable files in your system by comparing their original
CRC values stored in a database with current values. You can create such database by using the `crc_collector.sh`
script or with any other valid method. Just be sure that database data stored in the correct form:
`filename<DB_ENTRY_DELIM>crc`, where `DB_ENTRY_DELIM` is a delimeter ('space' by default).

This can be used, for example, in embedded devices before the launch stage (e.g.: in initramfs).
