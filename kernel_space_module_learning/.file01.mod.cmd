savedcmd_file01.mod := printf '%s\n'   file01.o | awk '!x[$$0]++ { print("./"$$0) }' > file01.mod
