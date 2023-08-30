# Recipe created by recipetool
# This is the basis of a recipe and may need further editing in order to be fully functional.
# (Feel free to remove these comments when editing.)

SUMMARY = "A fully featured modbus protocol stack in python"
HOMEPAGE = "https://github.com/riptideio/pymodbus/"
# WARNING: the following LICENSE and LIC_FILES_CHKSUM values are best guesses - it is
# your responsibility to verify that the values are complete and correct.
#
# The following license files were not able to be identified and are
# represented as "Unknown" below, you will need to check them yourself:
#   LICENSE
#   examples/tools/reference/LICENSE_FREE
# NOTE: Original package / source metadata indicates license is: BSD
#
# NOTE: multiple licenses have been detected; they have been separated with &
# in the LICENSE value for now since it is a reasonable assumption that all
# of the licenses apply. If instead there is a choice between the multiple
# licenses then you should change the value to separate the licenses with |
# instead of &. If there is any doubt, check the accompanying documentation
# to determine which situation is applicable.
LICENSE = "Unknown & GPLv2 & BSD"
LIC_FILES_CHKSUM = "file://LICENSE;md5=2c2223d66c7e674b40527b5a4c35bd76 \
                    file://examples/tools/nullmodem/linux/COPYING;md5=94d55d512a9ba36caa9b7df079bae19f \
                    file://examples/tools/reference/LICENSE_FREE;md5=05018d6237f3f611b1b4be0f65fb2a59"

SRC_URI = "git://github.com/riptideio/pymodbus;protocol=https"

# Modify these as desired
PV = "1.0+git${SRCPV}"
SRCREV = "ce5d83b67928bf651cacaee4be2bb4bb212494bd"

S = "${WORKDIR}/git"

inherit setuptools3

DEPENDS += "python3-six-native"

# The following configs & dependencies are from setuptools extras_require.
# These dependencies are optional, hence can be controlled via PACKAGECONFIG.
# The upstream names may not correspond exactly to bitbake package names.
#
# Uncomment this line to enable all the optional features.
#PACKAGECONFIG ?= "quality documents twisted tornado repl:python_version <= "2.7" repl:python_version >= "3.6""
PACKAGECONFIG[quality] = ",,,python3-coverage python3-mock python3-nose python3-pep8"
PACKAGECONFIG[documents] = ",,,python3-humanfriendly python3-sphinx python3-sphinx_rtd_theme"
PACKAGECONFIG[twisted] = ",,,python3-pyasn1 python3-twisted[serial]"
PACKAGECONFIG[tornado] = ",,,python3-tornado"
PACKAGECONFIG[repl] = ",,,python3-aiohttp python3-click python3-prompt-toolkit python3-pygments python3-pyserial-asyncio"

# WARNING: the following rdepends are determined through basic analysis of the
# python sources, and might not be 100% accurate.
RDEPENDS_${PN} += "python3-2to3 python3-asyncio python3-core python3-crypt python3-distutils python3-io python3-json python3-logging python3-math python3-netserver python3-setuptools python3-pyserial python3-pytest python3-setuptools python3-six python3-unittest"

# WARNING: We were unable to map the following python package/module
# dependencies to the bitbake packages which include them:
#    SocketServer
#    asynctest
#    asynctest.mock
#    md5
#    prompt_toolkit
#    prompt_toolkit.application.current
#    prompt_toolkit.auto_suggest
#    prompt_toolkit.completion
#    prompt_toolkit.filters
#    prompt_toolkit.formatted_text
#    prompt_toolkit.history
#    prompt_toolkit.key_binding
#    prompt_toolkit.lexers
#    prompt_toolkit.patch_stdout
#    prompt_toolkit.shortcuts
#    prompt_toolkit.shortcuts.progress_bar
#    prompt_toolkit.styles
#    pychecker
#    pyflakes.scripts.pyflakes
#    pygments.lexers.data
#    pygments.lexers.python
#    pylint
#    redis
#    serial_asyncio
#    sqlalchemy
#    sqlalchemy.schema
#    sqlalchemy.sql
#    sqlalchemy.sql.expression
#    sqlalchemy.types
#    tornado.concurrent
#    tornado.ioloop
#    tornado.iostream
#    twisted.conch
#    twisted.conch.insults
#    twisted.cred
#    twisted.internet
#    twisted.internet.protocol
#    twisted.internet.serialport
#    twisted.python.failure
