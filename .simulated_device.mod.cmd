savedcmd_simulated_device.mod := printf '%s\n'   simulated_device.o | awk '!x[$$0]++ { print("./"$$0) }' > simulated_device.mod
