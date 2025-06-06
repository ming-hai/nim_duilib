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
// $hash=cd0fb3cc926fd2551378a107ccf92c4256f33269$
//

#include "libcef_dll/cpptoc/task_cpptoc.h"

namespace {

// MEMBER FUNCTIONS - Body may be edited by hand.

void CEF_CALLBACK task_execute(struct _cef_task_t* self) {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  DCHECK(self);
  if (!self) {
    return;
  }

  // Execute
  CefTaskCppToC::Get(self)->Execute();
}

}  // namespace

// CONSTRUCTOR - Do not edit by hand.

CefTaskCppToC::CefTaskCppToC() {
  GetStruct()->execute = task_execute;
}

// DESTRUCTOR - Do not edit by hand.

CefTaskCppToC::~CefTaskCppToC() {}

template <>
CefRefPtr<CefTask>
CefCppToCRefCounted<CefTaskCppToC, CefTask, cef_task_t>::UnwrapDerived(
    CefWrapperType type,
    cef_task_t* s) {
  CHECK(false) << __func__ << " called with unexpected class type " << type;
  return nullptr;
}

template <>
CefWrapperType
    CefCppToCRefCounted<CefTaskCppToC, CefTask, cef_task_t>::kWrapperType =
        WT_TASK;
