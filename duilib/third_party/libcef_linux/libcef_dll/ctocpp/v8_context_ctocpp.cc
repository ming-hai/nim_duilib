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
// $hash=3722c398a32be688ba93ea7112a82b5beb448b95$
//

#include "libcef_dll/ctocpp/v8_context_ctocpp.h"

#include "libcef_dll/ctocpp/browser_ctocpp.h"
#include "libcef_dll/ctocpp/frame_ctocpp.h"
#include "libcef_dll/ctocpp/task_runner_ctocpp.h"
#include "libcef_dll/ctocpp/v8_exception_ctocpp.h"
#include "libcef_dll/ctocpp/v8_value_ctocpp.h"

// STATIC METHODS - Body may be edited by hand.

NO_SANITIZE("cfi-icall")
CefRefPtr<CefV8Context> CefV8Context::GetCurrentContext() {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  auto* _retval = cef_v8_context_get_current_context();

  // Return type: refptr_same
  return CefV8ContextCToCpp_Wrap(_retval);
}

NO_SANITIZE("cfi-icall")
CefRefPtr<CefV8Context> CefV8Context::GetEnteredContext() {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  auto* _retval = cef_v8_context_get_entered_context();

  // Return type: refptr_same
  return CefV8ContextCToCpp_Wrap(_retval);
}

NO_SANITIZE("cfi-icall") bool CefV8Context::InContext() {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  int _retval = cef_v8_context_in_context();

  // Return type: bool
  return _retval ? true : false;
}

// VIRTUAL METHODS - Body may be edited by hand.

NO_SANITIZE("cfi-icall")
CefRefPtr<CefTaskRunner> CefV8ContextCToCpp::GetTaskRunner() {
  auto* _struct = GetStruct();
  if (!_struct->get_task_runner) {
    return nullptr;
  }

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  auto* _retval = _struct->get_task_runner(_struct);

  // Return type: refptr_same
  return CefTaskRunnerCToCpp_Wrap(_retval);
}

NO_SANITIZE("cfi-icall") bool CefV8ContextCToCpp::IsValid() {
  auto* _struct = GetStruct();
  if (!_struct->is_valid) {
    return false;
  }

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  int _retval = _struct->is_valid(_struct);

  // Return type: bool
  return _retval ? true : false;
}

NO_SANITIZE("cfi-icall")
CefRefPtr<CefBrowser> CefV8ContextCToCpp::GetBrowser() {
  auto* _struct = GetStruct();
  if (!_struct->get_browser) {
    return nullptr;
  }

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  auto* _retval = _struct->get_browser(_struct);

  // Return type: refptr_same
  return CefBrowserCToCpp_Wrap(_retval);
}

NO_SANITIZE("cfi-icall") CefRefPtr<CefFrame> CefV8ContextCToCpp::GetFrame() {
  auto* _struct = GetStruct();
  if (!_struct->get_frame) {
    return nullptr;
  }

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  auto* _retval = _struct->get_frame(_struct);

  // Return type: refptr_same
  return CefFrameCToCpp_Wrap(_retval);
}

NO_SANITIZE("cfi-icall") CefRefPtr<CefV8Value> CefV8ContextCToCpp::GetGlobal() {
  auto* _struct = GetStruct();
  if (!_struct->get_global) {
    return nullptr;
  }

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  auto* _retval = _struct->get_global(_struct);

  // Return type: refptr_same
  return CefV8ValueCToCpp_Wrap(_retval);
}

NO_SANITIZE("cfi-icall") bool CefV8ContextCToCpp::Enter() {
  auto* _struct = GetStruct();
  if (!_struct->enter) {
    return false;
  }

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  int _retval = _struct->enter(_struct);

  // Return type: bool
  return _retval ? true : false;
}

NO_SANITIZE("cfi-icall") bool CefV8ContextCToCpp::Exit() {
  auto* _struct = GetStruct();
  if (!_struct->exit) {
    return false;
  }

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  int _retval = _struct->exit(_struct);

  // Return type: bool
  return _retval ? true : false;
}

NO_SANITIZE("cfi-icall")
bool CefV8ContextCToCpp::IsSame(CefRefPtr<CefV8Context> that) {
  auto* _struct = GetStruct();
  if (!_struct->is_same) {
    return false;
  }

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Verify param: that; type: refptr_same
  DCHECK(that.get());
  if (!that.get()) {
    return false;
  }

  // Execute
  int _retval = _struct->is_same(_struct, CefV8ContextCToCpp_Unwrap(that));

  // Return type: bool
  return _retval ? true : false;
}

NO_SANITIZE("cfi-icall")
bool CefV8ContextCToCpp::Eval(const CefString& code,
                              const CefString& script_url,
                              int start_line,
                              CefRefPtr<CefV8Value>& retval,
                              CefRefPtr<CefV8Exception>& exception) {
  auto* _struct = GetStruct();
  if (!_struct->eval) {
    return false;
  }

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Verify param: code; type: string_byref_const
  DCHECK(!code.empty());
  if (code.empty()) {
    return false;
  }
  // Unverified params: script_url

  // Translate param: retval; type: refptr_same_byref
  cef_v8_value_t* retvalStruct = NULL;
  if (retval.get()) {
    retvalStruct = CefV8ValueCToCpp_Unwrap(retval);
  }
  auto* retvalOrig = retvalStruct;
  // Translate param: exception; type: refptr_same_byref
  cef_v8_exception_t* exceptionStruct = NULL;
  if (exception.get()) {
    exceptionStruct = CefV8ExceptionCToCpp_Unwrap(exception);
  }
  auto* exceptionOrig = exceptionStruct;

  // Execute
  int _retval = _struct->eval(_struct, code.GetStruct(), script_url.GetStruct(),
                              start_line, &retvalStruct, &exceptionStruct);

  // Restore param:retval; type: refptr_same_byref
  if (retvalStruct) {
    if (retvalStruct != retvalOrig) {
      retval = CefV8ValueCToCpp_Wrap(retvalStruct);
    }
  } else {
    retval = nullptr;
  }
  // Restore param:exception; type: refptr_same_byref
  if (exceptionStruct) {
    if (exceptionStruct != exceptionOrig) {
      exception = CefV8ExceptionCToCpp_Wrap(exceptionStruct);
    }
  } else {
    exception = nullptr;
  }

  // Return type: bool
  return _retval ? true : false;
}

// CONSTRUCTOR - Do not edit by hand.

CefV8ContextCToCpp::CefV8ContextCToCpp() {}

// DESTRUCTOR - Do not edit by hand.

CefV8ContextCToCpp::~CefV8ContextCToCpp() {}

template <>
cef_v8_context_t*
CefCToCppRefCounted<CefV8ContextCToCpp, CefV8Context, cef_v8_context_t>::
    UnwrapDerived(CefWrapperType type, CefV8Context* c) {
  CHECK(false) << __func__ << " called with unexpected class type " << type;
  return nullptr;
}

template <>
CefWrapperType CefCToCppRefCounted<CefV8ContextCToCpp,
                                   CefV8Context,
                                   cef_v8_context_t>::kWrapperType =
    WT_V8_CONTEXT;
