/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This file is part of the LibreOffice project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <fasttokenhandler.hxx>

#include <xmloff/token/tokens.hxx>

namespace xmloff {

namespace {
// include auto-generated Perfect_Hash
#if defined __clang__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wimplicit-fallthrough"
#if __has_warning("-Wdeprecated-register")
#pragma GCC diagnostic ignored "-Wdeprecated-register"
#endif
#endif
#include <tokenhash.inc>
#if defined __clang__
#pragma GCC diagnostic pop
#endif
} // namespace

namespace token {

using namespace css;

TokenMap& StaticTokenMap()
{
    static TokenMap SINGLETON;
    return SINGLETON;
}

const css::uno::Sequence< sal_Int8 > TokenMap::EMPTY_BYTE_SEQ;
const OUString TokenMap::EMPTY_STRING;

TokenMap::TokenMap() :
    maTokenNamesUtf8( static_cast< size_t >( XML_TOKEN_COUNT ) ),
    maTokenNames( static_cast< size_t >( XML_TOKEN_COUNT ) )
{
    static const char* sppcTokenNames[] =
    {
#include <tokennames.inc>
        ""
    };

    const char* const* ppcTokenName = sppcTokenNames;
    int i = 0;
    for( auto& rTokenName : maTokenNamesUtf8 )
    {
        std::string_view pStr = *ppcTokenName;
        rTokenName = uno::Sequence< sal_Int8 >(
                    reinterpret_cast< const sal_Int8* >( pStr.data() ), pStr.length() );
        maTokenNames[i++] = OStringToOUString( pStr, RTL_TEXTENCODING_UTF8 );
        ++ppcTokenName;
    }
}

TokenMap::~TokenMap()
{
}

sal_Int32 TokenMap::getTokenPerfectHash( const char *pStr, sal_Int32 nLength )
{
    const struct xmltoken *pToken = Perfect_Hash::in_word_set( pStr, nLength );
    return pToken ? pToken->nToken : xmloff::XML_TOKEN_INVALID;
}

FastTokenHandler::FastTokenHandler() :
    mrTokenMap( StaticTokenMap() )
{
}

FastTokenHandler::~FastTokenHandler()
{
}

// XFastTokenHandler
uno::Sequence< sal_Int8 > FastTokenHandler::getUTF8Identifier( sal_Int32 nToken )
{
    return mrTokenMap.getUtf8TokenName( nToken );
}

const OUString& FastTokenHandler::getIdentifier( sal_Int32 nToken ) const
{
    return mrTokenMap.getTokenName( nToken );
}

sal_Int32 FastTokenHandler::getTokenFromUTF8( const uno::Sequence< sal_Int8 >& rIdentifier )
{
    return TokenMap::getTokenFromUtf8( rIdentifier );
}

// Much faster direct C++ shortcut
sal_Int32 FastTokenHandler::getTokenDirect( const char* pToken, sal_Int32 nLength ) const
{
    return TokenMap::getTokenFromUTF8( pToken, nLength );
}

} // namespace token
} // namespace xmloff

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
