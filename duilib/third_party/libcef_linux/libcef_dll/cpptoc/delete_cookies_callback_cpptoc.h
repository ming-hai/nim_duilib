// Copyright (c) 2025 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.
//
// ---------------------------------------------------------------------------
//
// This file was generated by the CEF translator tool. If making changes by
// hand only do so within the body of existing method and function
// implementations. See the translator.README.txt file in the tools directory
// for more information.
//
// $hash=19f53df3fd7b4f237a6449078ea2e84e6c4663bc$
//

#ifndef CEF_LIBCEF_DLL_CPPTOC_DELETE_COOKIES_CALLBACK_CPPTOC_H_
#define CEF_LIBCEF_DLL_CPPTOC_DELETE_COOKIES_CALLBACK_CPPTOC_H_
#pragma once

#if !defined(WRAPPING_CEF_SHARED)
#error This file can be included wrapper-side only
#endif

#include "include/capi/cef_cookie_capi.h"
#include "include/cef_cookie.h"
#include "libcef_dll/cpptoc/cpptoc_ref_counted.h"

// Wrap a C++ class with a C structure.
// This class may be instantiated and accessed wrapper-side only.
class CefDeleteCookiesCallbackCppToC
    : public CefCppToCRefCounted<CefDeleteCookiesCallbackCppToC,
                                 CefDeleteCookiesCallback,
                                 cef_delete_cookies_callback_t> {
 public:
  CefDeleteCookiesCallbackCppToC();
  virtual ~CefDeleteCookiesCallbackCppToC();
};

constexpr auto CefDeleteCookiesCallbackCppToC_Wrap =
    CefDeleteCookiesCallbackCppToC::Wrap;
constexpr auto CefDeleteCookiesCallbackCppToC_Unwrap =
    CefDeleteCookiesCallbackCppToC::Unwrap;

#endif  // CEF_LIBCEF_DLL_CPPTOC_DELETE_COOKIES_CALLBACK_CPPTOC_H_
