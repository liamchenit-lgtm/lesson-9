#!/usr/bin/env bash

set -u

# field.jerryio.com holds the VEX Controller connection. Closing only this
# Chrome tab releases the controller so PROS can upload through it.
FIELD_URL="https://field.jerryio.com/"

close_field_tabs() {
  osascript - "$FIELD_URL" <<'APPLESCRIPT'
on run argv
    set targetUrl to item 1 of argv

    tell application "System Events"
        set chromeIsRunning to exists process "Google Chrome"
    end tell

    if chromeIsRunning then
        tell application "Google Chrome"
            repeat with windowIndex from (count of windows) to 1 by -1
                repeat with tabIndex from (count of tabs of window windowIndex) to 1 by -1
                    set currentUrl to URL of tab tabIndex of window windowIndex
                    if currentUrl starts with targetUrl then
                        close tab tabIndex of window windowIndex
                    end if
                end repeat
            end repeat
        end tell
    end if
end run
APPLESCRIPT
}

reopen_field() {
  open -a "Google Chrome" "$FIELD_URL"
}

close_field_tabs

# Give Chrome a moment to release the USB/HID connection.
sleep 1

# Reopen Field Control even if compilation or upload fails.
trap reopen_field EXIT

if ! command -v pros >/dev/null 2>&1; then
  echo "Error: 'pros' was not found in PATH." >&2
  echo "Run this script from the VS Code PROS terminal where 'pros mu' works." >&2
  exit 127
fi

pros mu "$@"

