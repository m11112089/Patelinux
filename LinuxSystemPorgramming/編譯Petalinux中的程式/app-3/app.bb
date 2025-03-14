#
# This file is the app recipe.
#

SUMMARY = "Simple app application"
SECTION = "PETALINUX/apps"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://test.c \
		   file://test2.c \
		   file://testd.service \
		   file://Makefile \
	"

S = "${WORKDIR}"

FILESEXTRAPATHS:prepend := "${THISDIR}/files:"
  
inherit update-rc.d systemd
  
INITSCRIPT_NAME = "test2"
INITSCRIPT_PARAMS = "start 99 S ."
  
SYSTEMD_PACKAGES = "${PN}"
SYSTEMD_SERVICE:${PN} = "testd.service"
SYSTEMD_AUTO_ENABLE:${PN} = "enable"

do_install() {
		if ${@bb.utils.contains('DISTRO_FEATURES', 'sysvinit', 'true', 'false', d)}; then
			install -d ${D}${sysconfdir}/init.d/
			install -m 0755 ${WORKDIR}/test2 ${D}${sysconfdir}/init.d/
        fi
  
        install -d ${D}${systemd_system_unitdir}
        install -m 0644 ${WORKDIR}/testd.service ${D}${systemd_system_unitdir}

	     install -d ${D}/${bindir}
	     install -m 0755 test ${D}/${bindir}
		 install -m 0755 test2 ${D}/${bindir}
}
