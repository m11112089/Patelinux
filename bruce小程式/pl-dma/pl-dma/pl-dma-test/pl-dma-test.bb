#
# This file is the pl-dma-test recipe.
#

SUMMARY = "Simple pl-dma-test application"
SECTION = "PETALINUX/apps"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://pl-dma-test.c \
	   file://os_wrap.c \
	   file://os_wrap.h \
	   file://AF_AXI_DMA_ctrl.c \
	   file://AF_AXI_DMA_ctrl.h \
	   file://Makefile \
		  "

S = "${WORKDIR}"

do_compile() {
	     oe_runmake
}

do_install() {
	     install -d ${D}${bindir}
	     install -m 0755 pl-dma-test ${D}${bindir}
}
