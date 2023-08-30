replace_or_add_variable () {
   # $1: variable name
   # $2: variable value
   # $3: file
   sed -i "/^\#*${1}\s*=\s*/{h;s/^\#*${1}\s*=.*/${1} = ${2}/};\${x;/^$/{s//${1} = ${2}/;H};x}" ${3}
}

do_install:append () {
   replace_or_add_variable "net.ipv6.conf.all.disable_ipv6" "1" ${D}${sysconfdir}/sysctl.conf
   replace_or_add_variable "net.ipv6.conf.default.disable_ipv6" "1" ${D}${sysconfdir}/sysctl.conf
}
