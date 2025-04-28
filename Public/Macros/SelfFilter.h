// SPDX-License-Identifier: MPL-2.0

#pragma once

#if defined(WITH_SPDLOG)
#define _BCICORELOG_SELF_FILTER "[BCILOG] "
#else
#define _BCICORELOG_SELF_FILTER ""
#endif // WITH_SPDLOG
