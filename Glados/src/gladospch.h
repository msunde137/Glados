#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <math.h>

#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <unordered_map>
#include <unordered_set>

#include "Glados/Core/Core.h"
#include "Glados/Core/KeyCodes.h"
#include "Glados/Core/MouseButtonCodes.h"

#ifdef GD_PLATFORM_WINDOWS
#include "Windows.h"
#endif // GD_PLATFORM_WINDOWS