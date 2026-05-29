#!/usr/bin/env bash
set -e

AP_CON_NAME="LedPong-AP"
CLIENT_CON_NAME="Flugverbot"
WIFI_IF="wlan0"

echo "[wifi_client] Switching to WiFi client mode (SSID: Flugverbot) on ${WIFI_IF} ..."

nmcli radio wifi on

# Stop AP connection if active
if nmcli -t -f NAME connection show --active | grep -q "^${AP_CON_NAME}$"; then
  echo "[wifi_client] Deactivating AP connection: ${AP_CON_NAME}"
  nmcli connection down "${AP_CON_NAME}" || true
fi

# Create client connection if it doesn't exist yet
if ! nmcli -t -f NAME connection show | grep -q "^${CLIENT_CON_NAME}$"; then
  echo "[wifi_client] Creating client connection: ${CLIENT_CON_NAME}"
  nmcli connection add \
    type wifi \
    ifname "${WIFI_IF}" \
    mode infra \
    con-name "${CLIENT_CON_NAME}" \
    ssid "Flugverbot"

  nmcli connection modify "${CLIENT_CON_NAME}" \
    wifi-sec.key-mgmt wpa-psk \
    wifi-sec.psk "Zone1234" \
    ipv4.method auto \
    ipv6.method auto
else
  echo "[wifi_client] Client connection ${CLIENT_CON_NAME} already exists."
fi

echo "[wifi_client] Bringing up client connection: ${CLIENT_CON_NAME}"
nmcli connection up "${CLIENT_CON_NAME}"

echo "[wifi_client] Done. The Pi should now connect to 'Flugverbot'."
