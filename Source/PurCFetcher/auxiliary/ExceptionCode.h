/*
 *  Copyright (C) 2006-2017 Apple Inc. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#pragma once

#include <wtf/EnumTraits.h>

namespace PurcFetcher {

enum ExceptionCode {
    // DOMException error names (https://heycam.github.io/webidl/#idl-DOMException-error-names).
    // Those need to be kept in sync with the array in DOMException.cpp.
    IndexSizeError, // Deprecated. Use RangeError instead.
    HierarchyRequestError,
    WrongDocumentError,
    InvalidCharacterError,
    NoModificationAllowedError,
    NotFoundError,
    NotSupportedError,
    InUseAttributeError,
    InvalidStateError,
    SyntaxError,
    InvalidModificationError,
    NamespaceError,
    InvalidAccessError, // Deprecated. use NotAllowedError instead.
    TypeMismatchError, // Deprecated. Use TypeError instead.
    SecurityError,
    NetworkError,
    AbortError,
    URLMismatchError,
    QuotaExceededError,
    TimeoutError,
    InvalidNodeTypeError,
    DataCloneError,
    EncodingError,
    NotReadableError,
    UnknownError,
    ConstraintError,
    DataError,
    TransactionInactiveError,
    ReadonlyError,
    VersionError,
    OperationError,
    NotAllowedError,

    // Simple exceptions (https://heycam.github.io/webidl/#idl-exceptions).
    RangeError,
    TypeError,

    // Non-standard error.
    StackOverflowError,

    // Used to indicate to the bindings that a JS exception was thrown below and it should be propagated.
    ExistingExceptionError,
};

} // namespace PurcFetcher

namespace WTF {

template<> struct EnumTraits<PurcFetcher::ExceptionCode> {
    using values = EnumValues<
    PurcFetcher::ExceptionCode,
    PurcFetcher::ExceptionCode::IndexSizeError,
    PurcFetcher::ExceptionCode::HierarchyRequestError,
    PurcFetcher::ExceptionCode::WrongDocumentError,
    PurcFetcher::ExceptionCode::InvalidCharacterError,
    PurcFetcher::ExceptionCode::NoModificationAllowedError,
    PurcFetcher::ExceptionCode::NotFoundError,
    PurcFetcher::ExceptionCode::NotSupportedError,
    PurcFetcher::ExceptionCode::InUseAttributeError,
    PurcFetcher::ExceptionCode::InvalidStateError,
    PurcFetcher::ExceptionCode::SyntaxError,
    PurcFetcher::ExceptionCode::InvalidModificationError,
    PurcFetcher::ExceptionCode::NamespaceError,
    PurcFetcher::ExceptionCode::InvalidAccessError,
    PurcFetcher::ExceptionCode::TypeMismatchError,
    PurcFetcher::ExceptionCode::SecurityError,
    PurcFetcher::ExceptionCode::NetworkError,
    PurcFetcher::ExceptionCode::AbortError,
    PurcFetcher::ExceptionCode::URLMismatchError,
    PurcFetcher::ExceptionCode::QuotaExceededError,
    PurcFetcher::ExceptionCode::TimeoutError,
    PurcFetcher::ExceptionCode::InvalidNodeTypeError,
    PurcFetcher::ExceptionCode::DataCloneError,
    PurcFetcher::ExceptionCode::EncodingError,
    PurcFetcher::ExceptionCode::NotReadableError,
    PurcFetcher::ExceptionCode::UnknownError,
    PurcFetcher::ExceptionCode::ConstraintError,
    PurcFetcher::ExceptionCode::DataError,
    PurcFetcher::ExceptionCode::TransactionInactiveError,
    PurcFetcher::ExceptionCode::ReadonlyError,
    PurcFetcher::ExceptionCode::VersionError,
    PurcFetcher::ExceptionCode::OperationError,
    PurcFetcher::ExceptionCode::NotAllowedError,
    PurcFetcher::ExceptionCode::RangeError,
    PurcFetcher::ExceptionCode::TypeError,
    PurcFetcher::ExceptionCode::StackOverflowError,
    PurcFetcher::ExceptionCode::ExistingExceptionError
    >;
};

} // namespace WTF
