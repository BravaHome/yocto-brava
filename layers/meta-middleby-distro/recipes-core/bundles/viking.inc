DESCRITPION = "RAUC Bundle for Viking MLUI"

inherit bundle


RAUC_KEY_FILE = "${THISDIR}/files/rauc-mlui-viking.key.pem"
RAUC_CERT_FILE = "${THISDIR}/files/rauc-mlui-viking.cert.pem"

RAUC_BUNDLE_COMPATIBLE = "mlui-viking"

RAUC_BUNDLE_FORMAT = "verity"
RAUC_BUNDLE_VERSION ?= "undefined"

