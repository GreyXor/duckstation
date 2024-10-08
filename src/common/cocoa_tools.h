// SPDX-FileCopyrightText: 2019-2024 Connor McLaughlin <stenzek@gmail.com>
// SPDX-License-Identifier: PolyForm-Strict-1.0.0

#include <span>
#include <string>
#include <string_view>
#include <optional>

class Error;

#ifdef __OBJC__
#import <AppKit/AppKit.h>
#import <Cocoa/Cocoa.h>

namespace CocoaTools {
NSString* StringViewToNSString(std::string_view str);

/// Converts NSError to a human-readable string.
std::string NSErrorToString(NSError* error);
}

#endif

namespace CocoaTools {
/// Add a handler to be run when macOS changes between dark and light themes
void AddThemeChangeHandler(void* ctx, void(handler)(void* ctx));

/// Remove a handler previously added using AddThemeChangeHandler with the given context
void RemoveThemeChangeHandler(void* ctx);

/// Moves a file from one location to another, using NSFileManager.
bool MoveFile(const char* source, const char* destination, Error* error);

/// Returns the bundle path.
std::optional<std::string> GetBundlePath();

/// Get the bundle path to the actual application without any translocation fun
std::optional<std::string> GetNonTranslocatedBundlePath();

/// Launch the given application once this one quits
bool DelayedLaunch(std::string_view file, std::span<const std::string_view> args = {});
} // namespace CocoaTools
