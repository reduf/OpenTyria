# OpenTyria

OpenTyria intend to be a fairly simple server in order to run all the stack
required to support connection from a Guild Wars client to this server possible.
Consequently, there is several design decisions that limit the scaling, but they
are chosen, because it simpliefied the final solution.

## Generating Diffie-Hellman key params:

1. Use the Python script `tools/gen-dhm-keys.py` to generate Diffie-Hellman params.
2. Use the Python script `tools/patch-gw.py` to update an executable with the
   appropriate patches.
