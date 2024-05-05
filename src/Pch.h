#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

// Detours
#include <detours.h>
#undef CreateService

// Standard library
#include <cstdint>
#include <cstdio>
#include <d3d11.h>

#include <rangers-sdk.h>

#include <utilities/Helpers.h>
#undef max
