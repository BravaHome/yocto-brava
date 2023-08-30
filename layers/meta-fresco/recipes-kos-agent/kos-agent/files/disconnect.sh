#!/bin/bash

SERVICE_NAME=com.kitchenos.Agent

disconnect() {
	BUS_ARG=$1
	dbus-send "$BUS_ARG" --print-reply=literal --dest=$SERVICE_NAME \
		/com/kitchenos/Agent \
		com.kitchenos.Agent.Connectivity1.RemoveCredentials
}

if dbus-send --print-reply --dest=org.freedesktop.DBus /org/freedesktop/DBus org.freedesktop.DBus.ListNames 2>/dev/null | grep -q $SERVICE_NAME; then
	disconnect --session
else
	disconnect --system
fi
