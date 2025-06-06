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
// $hash=9a1e6d27173525bca7ebee7f8f755c3efc7fd2fa$
//

#ifndef CEF_LIBCEF_DLL_CTOCPP_TEST_TRANSLATOR_TEST_REF_PTR_LIBRARY_CHILD_CHILD_CTOCPP_H_
#define CEF_LIBCEF_DLL_CTOCPP_TEST_TRANSLATOR_TEST_REF_PTR_LIBRARY_CHILD_CHILD_CTOCPP_H_
#pragma once

#if !defined(WRAPPING_CEF_SHARED)
#error This file can be included wrapper-side only
#endif

#include "include/capi/test/cef_translator_test_capi.h"
#include "include/test/cef_translator_test.h"
#include "libcef_dll/ctocpp/ctocpp_ref_counted.h"

// Wrap a C structure with a C++ class.
// This class may be instantiated and accessed wrapper-side only.
class CefTranslatorTestRefPtrLibraryChildChildCToCpp
    : public CefCToCppRefCounted<
          CefTranslatorTestRefPtrLibraryChildChildCToCpp,
          CefTranslatorTestRefPtrLibraryChildChild,
          cef_translator_test_ref_ptr_library_child_child_t> {
 public:
  CefTranslatorTestRefPtrLibraryChildChildCToCpp();
  virtual ~CefTranslatorTestRefPtrLibraryChildChildCToCpp();

  // CefTranslatorTestRefPtrLibraryChildChild methods.
  int GetOtherOtherValue() override;
  void SetOtherOtherValue(int value) override;

  // CefTranslatorTestRefPtrLibraryChild methods.
  int GetOtherValue() override;
  void SetOtherValue(int value) override;

  // CefTranslatorTestRefPtrLibrary methods.
  int GetValue() override;
  void SetValue(int value) override;
};

constexpr auto CefTranslatorTestRefPtrLibraryChildChildCToCpp_Wrap =
    CefTranslatorTestRefPtrLibraryChildChildCToCpp::Wrap;
constexpr auto CefTranslatorTestRefPtrLibraryChildChildCToCpp_Unwrap =
    CefTranslatorTestRefPtrLibraryChildChildCToCpp::Unwrap;

#endif  // CEF_LIBCEF_DLL_CTOCPP_TEST_TRANSLATOR_TEST_REF_PTR_LIBRARY_CHILD_CHILD_CTOCPP_H_
