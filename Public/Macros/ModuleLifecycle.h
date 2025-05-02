// SPDX-License-Identifier: MPL-2.0

#pragma once

#include "Macros/Log.h"

#define BCILOG_STARTUP_MODULE(CATEGORY, NAME) BCILOG(CATEGORY, Log, "Starting module", {"module", STRINGIFY_MACRO(NAME)})
#define BCILOG_SHUTDOWN_MODULE(CATEGORY, NAME) BCILOG(CATEGORY, Log, "Shutting down module", {"module", STRINGIFY_MACRO(NAME)})
