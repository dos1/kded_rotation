stdbuf -oL libinput debug-events | while read line; do
    if [[ $line == *"switch tablet-mode state 1"* ]]; then
      orientation-helper -u
    fi
    if [[ $line == *"switch tablet-mode state 0"* ]]; then
      orientation-helper -l
      orientation-helper -r normal
    fi
done
