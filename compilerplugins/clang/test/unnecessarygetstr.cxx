/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4; fill-column: 100 -*- */
/*
 * This file is part of the LibreOffice project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <sal/config.h>

#include <ostream>
#include <string_view>

#include <rtl/strbuf.hxx>
#include <rtl/string.hxx>
#include <rtl/ustrbuf.hxx>
#include <rtl/ustring.hxx>
#include <sal/log.hxx>

void f1(bool, const OString& s);
struct Foo
{
    void f1(bool, const OString& s);
};
void test1(Foo& foo)
{
    OString s;
    // expected-error@+1 {{unnecessary call to 'getStr' when passing to OString arg [loplugin:unnecessarygetstr]}}
    f1(true, s.getStr());
    // expected-error@+1 {{unnecessary call to 'getStr' when passing to OString arg [loplugin:unnecessarygetstr]}}
    foo.f1(true, s.getStr());

    // avoid false +
    OString aVal = "xx";
    OUString aCompText
        = "xx" + OUString(aVal.getStr(), aVal.getLength(), RTL_TEXTENCODING_ASCII_US);
    (void)aCompText;
}

// call to param that takes string_view
void f2(bool, std::string_view);
struct Foo2
{
    void f2(bool, std::string_view);
};
void test2(Foo2& foo)
{
    OString s;
    // expected-error@+1 {{unnecessary call to 'getStr' when passing to string_view arg [loplugin:unnecessarygetstr]}}
    f2(true, s.getStr());
    // expected-error@+1 {{unnecessary call to 'getStr' when passing to string_view arg [loplugin:unnecessarygetstr]}}
    foo.f2(true, s.getStr());
}
/* vim:set shiftwidth=4 softtabstop=4 expandtab cinoptions=b1,g0,N-s cinkeys+=0=break: */
