#!/bin/bash
set -e

BOARD=lacunaspace:stm32l4:Lacuna-LS200
PROJECT=example-lacuna-ls200
COMMAND=$1
if [ -z "$ARDUINO_CLI" ]; then
    ARDUINO_CLI=$(which arduino-cli || true)
fi
DIRNAME="$(basename "$SCRIPTPATH")"
EXPECTED_CLI_MAJOR=0
EXPECTED_CLI_MINOR=13

if [ ! -x "$ARDUINO_CLI" ]; then
    echo "Cannot find 'arduino-cli' in your PATH. Install the Arduino CLI before you continue."
    echo "Installation instructions: https://arduino.github.io/arduino-cli/latest/"
    exit 1
fi

CLI_MAJOR=$($ARDUINO_CLI version | cut -d. -f1 | rev | cut -d ' '  -f1)
CLI_MINOR=$($ARDUINO_CLI version | cut -d. -f2)
CLI_REV=$($ARDUINO_CLI version | cut -d. -f3 | cut -d ' '  -f1)

if (( CLI_MINOR < EXPECTED_CLI_MINOR)); then
    echo "You need to upgrade your Arduino CLI version (now: $CLI_MAJOR.$CLI_MINOR.$CLI_REV, but required: $EXPECTED_CLI_MAJOR.$EXPECTED_CLI_MINOR.x or higher)"
    echo "See https://arduino.github.io/arduino-cli/installation/ for upgrade instructions"
    exit 1
fi

if (( CLI_MAJOR != EXPECTED_CLI_MAJOR || CLI_MINOR != EXPECTED_CLI_MINOR )); then
    echo "You're using an untested version of Arduino CLI, this might cause issues (found: $CLI_MAJOR.$CLI_MINOR.$CLI_REV, expected: $EXPECTED_CLI_MAJOR.$EXPECTED_CLI_MINOR.x)"
fi

if [ "$COMMAND" = "--build" ];
then
    echo "Building $PROJECT"
    $ARDUINO_CLI compile --fqbn $BOARD --output-dir . $PROJECT &
    pid=$! # Process Id of the previous running command
    while kill -0 $pid 2>/dev/null
    do
        echo "Still building..."
        sleep 2
    done
    wait $pid
    ret=$?
    if [ $ret -eq 0 ]; then
        echo "Building $PROJECT done"
    else
        exit "Building $PROJECT failed"
    fi
elif [ "$COMMAND" = "--flash" ];
then
    $ARDUINO_CLI upload -p $($ARDUINO_CLI board list | grep lacunaspace | cut -d ' ' -f1) --fqbn $BOARD --input-dir .
elif [ "$COMMAND" = "--all" ];
then
    $ARDUINO_CLI compile --fqbn $BOARD $PROJECT
    status=$?
    [ $status -eq 0 ] && $ARDUINO_CLI upload -p $($ARDUINO_CLI board list | grep lacunaspace | cut -d ' ' -f1) --fqbn $BOARD --input-dir . $PROJECT
else
    echo "Nothing to do for target"
fi
