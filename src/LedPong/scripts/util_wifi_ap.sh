#!/usr/bin/env bash
set -e

AP_CON_NAME="LedPong-AP"
CLIENT_CON_NAME="FRITZBox-6340ab-Cable"
WIFI_IF="wlan0"
AP_PSK="HKA12345"   # <-- hier dein Passwort (mind. 8 Zeichen)

echo "[wifi_ap] Switching to AP mode (SSID: LedPong) on ${WIFI_IF} ..."

nmcli radio wifi on

# Stop client connection if present
if nmcli -t -f NAME connection show --active | grep -q "^${CLIENT_CON_NAME}$"; then
  echo "[wifi_ap] Deactivating client connection: ${CLIENT_CON_NAME}"
  nmcli connection down "${CLIENT_CON_NAME}" || true
fi

# Create AP connection if it doesn't exist yet
if ! nmcli -t -f NAME connection show | grep -q "^${AP_CON_NAME}$"; then
  echo "[wifi_ap] Creating AP connection: ${AP_CON_NAME}"
  nmcli connection add \
    type wifi \
    ifname "${WIFI_IF}" \
    mode ap \
    con-name "${AP_CON_NAME}" \
    ssid "LedPong"

  nmcli connection modify "${AP_CON_NAME}" \
    802-11-wireless.mode ap \
    802-11-wireless.band bg \
    ipv4.method shared \
    ipv6.method ignore \
    802-11-wireless-security.key-mgmt wpa-psk \
    802-11-wireless-security.proto rsn \
    802-11-wireless-security.group ccmp \
    802-11-wireless-security.pairwise ccmp \
    802-11-wireless-security.psk "${AP_PSK}"
else
  echo "[wifi_ap] AP connection ${AP_CON_NAME} already exists. Updating security settings..."
  nmcli connection modify "${AP_CON_NAME}" \
    802-11-wireless.mode ap \
    802-11-wireless.band bg \
    ipv4.method shared \
    ipv6.method ignore \
    802-11-wireless-security.key-mgmt wpa-psk \
    802-11-wireless-security.proto rsn \
    802-11-wireless-security.group ccmp \
    802-11-wireless-security.pairwise ccmp \
    802-11-wireless-security.psk "${AP_PSK}"
fi

echo "[wifi_ap] Bringing up AP connection: ${AP_CON_NAME}"
nmcli connection up "${AP_CON_NAME}"

echo "[wifi_ap] Done. The Pi now provides SSID 'LedPong' with WPA2-PSK."
