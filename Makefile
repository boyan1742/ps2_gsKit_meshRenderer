# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright 2001-2004, ps2dev - http://www.ps2dev.org
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.

PS2DEV = /usr/local
PS2SDK = $(PS2DEV)/ps2sdk
GSKIT = $(PS2DEV)/gsKit
PS2EEBIN = $(PS2DEV)/ee/bin

ISO_TGT = prod_mesh.iso

# Where the obj files will be placed.
EE_DUMP_OBJ = obj/

EE_BIN = main.elf
EE_OBJS = main.o src/MyTypes.o src/Camera.o src/Mesh.o src/GamePad.o
# Where every obj file is located -- used for linking.
EE_OBJS_LOC = obj/main.o obj/src/MyTypes.o obj/src/Camera.o obj/src/Mesh.o obj/src/Gamepad.o
EE_INCLUDE = -I$(GSKIT)/include -Iinclude
EE_LIBS = -L. -L$(GSKIT)/lib -L$(GSKIT)/lib -ldma -lgraph -ldraw -lkernel -lm -lc -lgskit -ldmakit -lmath3d -lpad

all: $(ISO_TGT)

all_strip: $(EE_BIN)
	$(EE_STRIP) --strip-all $(EE_BIN)

$(ISO_TGT): $(EE_BIN)
	mkisofs -l -o $(ISO_TGT) $(EE_BIN) SYSTEM.CNF

clean:
	rm -f $(EE_BIN) $(EE_OBJS)
	rm -f $(ISO_TGT)

include Makefile.pref
include Makefile.eeglobal
