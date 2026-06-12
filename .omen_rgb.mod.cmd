savedcmd_omen_rgb.mod := printf '%s\n'   omen_rgb.o | awk '!x[$$0]++ { print("./"$$0) }' > omen_rgb.mod
