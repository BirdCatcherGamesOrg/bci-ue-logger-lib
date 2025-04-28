// SPDX-License-Identifier: MPL-2.0

#pragma once

#include "Macros/Log.h"

#define BCILOG_INITIALIZE_SUBSYSTEM(CATEGORY) BCILOG(CATEGORY, Log, "Initializing subsystem", {"subsystem", GetName()})
#define BCILOG_DEINITIALIZE_SUBSYSTEM(CATEGORY) BCILOG(CATEGORY, Log, "Deinitializing subsystem", {"subsystem", GetName()})
#define BCILOG_SHOULD_CREATE_SUBSYSTEM(CATEGORY, CREATE) BCILOG(CATEGORY, Log, "Should create subsystem", {"subsystem", GetName()}, {"create", static_cast<bool>(CREATE)})
