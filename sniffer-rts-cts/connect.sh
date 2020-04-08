sleep 1
wpa_supplicant -B -D wext -i wlan0 -c /etc/wpa_supplicant.conf
sleep 2
dhclient wlan0