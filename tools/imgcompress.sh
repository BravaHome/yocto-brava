#!/usr/bin/env bash
# Purpose: batch compression of wic images

directory=$1
[ ! -n ${directory} ] && [ -d ${directory} ] && echo "Directory ${directory} does not exist." && exit 1

[ -z ${directory} ] && directory=`pwd`

directory="${directory}/"
current=`pwd`

pushd ${directory} >/dev/null 2>&1

for filename in *.wic; do
    if [ -f ${filename} ] && [ ! -L ${filename} ]; then
        basename=`basename ${filename}`
	printf "Compressing ${basename} ... "
	/usr/bin/gzip -c ${basename} > ${current}/${basename}.gz
	/usr/bin/md5sum ${basename} > ${current}/${basename}.md5
	/usr/bin/md5sum ${current}/${basename}.gz > ${current}/${basename}.gz.md5
	printf "done.\n"
    fi
done

popd >/dev/null 2>&1

exit 0
