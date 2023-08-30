#!/bin/bash

SERVICE_NAME=com.kitchenos.Agent

start_provisioning() {
	BUS_ARG=$1
	# Timeout is 24 hours (0 does not seem to work as timeout value, it stops provision immediately)
	dbus-send "$BUS_ARG" --print-reply=literal --dest=$SERVICE_NAME \
		/com/kitchenos/Agent \
		com.kitchenos.Agent.Connectivity1.StartProvisioning \
		uint32:86400
}

if dbus-send --print-reply --dest=org.freedesktop.DBus /org/freedesktop/DBus org.freedesktop.DBus.ListNames 2>/dev/null | grep -q $SERVICE_NAME; then
	start_provisioning --session
else
	start_provisioning --system
fi
