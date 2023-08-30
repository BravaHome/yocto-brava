#!/bin/bash

SERVICE_NAME=com.kitchenos.Agent

stop_provisioning() {
	BUS_ARG=$1
	dbus-send "$BUS_ARG" --print-reply=literal --dest=$SERVICE_NAME \
		/com/kitchenos/Agent \
		com.kitchenos.Agent.Connectivity1.StopProvisioning
}

if dbus-send --print-reply --dest=org.freedesktop.DBus /org/freedesktop/DBus org.freedesktop.DBus.ListNames 2>/dev/null | grep -q $SERVICE_NAME; then
	stop_provisioning --session
else
	stop_provisioning --system
fi
