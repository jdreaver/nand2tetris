#!/usr/bin/env bash

set -eux

echo -n "Alice" > /dev/eudyptula
echo -n "Bob" > /dev/eudyptula
sleep 15
echo -n "Dave" > /dev/eudyptula
echo -n "Gena" > /dev/eudyptula
rmmod identity_queue
