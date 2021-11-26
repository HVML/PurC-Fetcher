#!/bin/bash

URL=https://www.fmsoft.cn
build/Source/samples/control/sync_req $URL 2>&1 | tee /tmp/sync.log
build/Source/samples/control/async_req $URL 2>&1 | tee /tmp/async.log
